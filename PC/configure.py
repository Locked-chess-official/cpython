#!/usr/bin/env python3
"""Generate PC/pyconfig.h from PC/configure.toml.

Prototype for the "configure.toml drives pyconfig.h" idea.

Reads a TOML description of the configurable macros, compiles and runs tiny C
probes with the active toolchain, and splices the resulting macro definitions
into the static template (PC/pyconfig.h.in) to produce PC/pyconfig.h.

Probe modes (the idea's three modes) — these compile+run `code` with `header`
and `libs`, and decide the macro value from the result:

  [[analyse.bool]]     success => `#define MACRO 1`;  failure => `/* #undef MACRO */`
  [[analyse.number]]   success => `#define MACRO <stdout-of-probe>`
  [[analyse.string]]   success => `#define MACRO "<stdout-of-probe>"`

Literal mode (no probing) — used for Windows-known constants that don't need
a probe, so every macro in the configurable section is represented:

  [[analyse.literal]]
    macro  (str, required)
    value  (bool|int|str, required)  see below
    comment (str)
    guard  (str)

  value semantics for [[analyse.literal]]:
    false                     ->  /* #undef MACRO */
    true                      ->  #define MACRO 1
    <int>                     ->  #define MACRO <int>
    ""                        ->  #define MACRO              (bare, no value)
    "token"                   ->  #define MACRO token        (bare token)
    '"quoted string"'         ->  #define MACRO "quoted string"  (verbatim)

Common fields for probe entries:
  macro     (str, required)
  comment   (str)             descriptive comment emitted above the macro
  header    (list[str])       headers to #include into the probe
  pre       (list[str])       raw lines emitted BEFORE the #include block, for
                              platform-conditional includes (e.g. #ifdef _WIN32)
  code      (str)             body of `int main(void) { ... }`; if omitted for a
                              bool probe, it is a bare header/compile check
  libs      (list[str])       extra libraries to link against
  guard     (str)             wrap the emission in `#if <guard>` ... `#endif`
  run       (bool)            bool only: run the probe (success = exit code 0),
                              instead of just compile+link. Needed for run-time
                              checks such as IEEE754 double byte order.
  invert    (bool)            bool only: flip the meaning — define the macro when
                              the probe FAILS. This mirrors autoconf's
                              "define fallback only if the feature is missing"
                              pattern (e.g. AC_TYPE_PID_T defines pid_t only when
                              the system does not).
  value     (str)             bool only: the token emitted after the macro name
                              when it is defined. Default "1"; use "" for a bare
                              define, or e.g. "int" for a type fallback.

Requires Python 3.11+ (uses the stdlib `tomllib` module).
"""

from __future__ import annotations

import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile
import tomllib
from dataclasses import dataclass, field
from pathlib import Path

MARKER = "@PYCONFIG_PROBES@"
UNDEF_PREFIX = "/* #undef "

PROBE_MODES = ("bool", "number", "string")

# Forced definitions shared by every probe. These mirror the static head of
# PC/pyconfig.h (the _CRT_* deprecation silencers) so that Windows probes see
# the same CRT name-space the real build does. Harmless on non-MSVC toolchains.
PROBE_PRELUDE = [
    "#define _CRT_NONSTDC_NO_DEPRECATE",
    "#define _CRT_SECURE_NO_DEPRECATE",
]


class ConfigureError(Exception):
    pass


@dataclass
class Entry:
    mode: str                       # "bool" | "number" | "string" | "literal"
    macro: str
    comment: str = ""
    header: list[str] = field(default_factory=list)
    code: str = ""
    pre: list[str] = field(default_factory=list)
    libs: list[str] = field(default_factory=list)
    unix_libs: list[str] | None = None
    value: object = None
    guard: str = ""
    run: bool = False
    invert: bool = False


def load_entries(path: Path) -> list[Entry]:
    with open(path, "rb") as f:
        data = tomllib.load(f)

    entries: list[Entry] = []
    analyse = data.get("analyse", {})

    for mode in PROBE_MODES:
        for item in analyse.get(mode, []):
            entries.append(Entry(
                mode=mode,
                macro=item["macro"],
                comment=item.get("comment", ""),
                header=item.get("header", []),
                pre=item.get("pre", []),
                code=item.get("code", ""),
                libs=item.get("libs", []),
                unix_libs=item.get("unix_libs"),
                guard=item.get("guard", ""),
                run=bool(item.get("run", False)),
                invert=bool(item.get("invert", False)),
                value=item.get("value"),
            ))

    for item in analyse.get("literal", []):
        entries.append(Entry(
            mode="literal",
            macro=item["macro"],
            comment=item.get("comment", ""),
            value=item["value"],
            guard=item.get("guard", ""),
        ))

    _check_duplicates(entries)
    return entries


def _check_duplicates(entries: list[Entry]) -> None:
    seen: dict[str, Entry] = {}
    for e in entries:
        if e.macro in seen:
            raise ConfigureError(
                f"macro {e.macro!r} appears more than once in configure.toml"
            )
        seen[e.macro] = e


def detect_compiler(explicit: str | None) -> str:
    if explicit:
        return explicit
    for cand in ("cl", "clang-cl"):
        if shutil.which(cand):
            return cand
    return os.environ.get("CC") or "cc"


def _msvc_environment_ready(compiler: str) -> bool:
    """Return True if the MSVC toolchain appears initialized.

    cl.exe can be on PATH without the environment being set up (no INCLUDE/LIB),
    which makes every probe fail with C1034 ("include path set is empty").
    Detect that early and give a clear error instead of silently failing all
    probes.  We only treat it as a hard requirement when no other usable
    compiler exists.
    """
    if Path(compiler).name.lower().startswith("cl"):
        return bool(os.environ.get("INCLUDE")) or bool(os.environ.get("LIB"))
    return True


class Toolchain:
    def __init__(self, compiler: str):
        self.compiler = compiler
        self.is_msvc = Path(compiler).name.lower().startswith("cl")

    def compile_cmd(self, src: str, exe: str, entry: "Entry") -> list[str]:
        if self.is_msvc:
            # /TC treats every following file as C source, so link libraries must
            # be passed through /link, otherwise cl tries to open them as source.
            cmd = [self.compiler, "/nologo", "/TC", src, f"/Fe:{exe}"]
            libs = [lib if lib.endswith(".lib") else f"{lib}.lib" for lib in entry.libs]
            if libs:
                cmd += ["/link"] + libs
        else:
            cmd = [self.compiler, "-o", exe, src]
            libs = entry.libs if entry.unix_libs is None else entry.unix_libs
            cmd += [f"-l{lib}" for lib in libs]
        return cmd


def run_probe(entry: Entry, tc: Toolchain, workdir: str) -> tuple[bool, str]:
    src = Path(workdir) / f"{entry.macro}.c"
    exe = Path(workdir) / f"{entry.macro}.exe"

    lines = [f"/* probe for {entry.macro} */"]
    lines.extend(PROBE_PRELUDE)
    for p in entry.pre:
        lines.append(p)
    for h in entry.header:
        lines.append(f"#include <{h}>")
    if entry.code:
        body = entry.code.strip()
        if not re.search(r"\bmain\s*\(", body):
            body = f"int main(void) {{\n{body}\nreturn 0;\n}}"
    else:
        body = "int main(void) { return 0; }"
    lines.append(body)
    src.write_text("\n".join(lines) + "\n", encoding="utf-8")

    cmd = tc.compile_cmd(str(src), str(exe), entry)
    try:
        comp = subprocess.run(cmd, capture_output=True, text=True, timeout=120,
                              cwd=workdir)
    except (subprocess.TimeoutExpired, OSError) as e:
        return False, f"compile error: {e}"
    if comp.returncode != 0:
        return False, comp.stderr.strip()[:400]

    if entry.mode == "bool":
        if not entry.run:
            return True, ""
        try:
            run = subprocess.run([str(exe)], capture_output=True, text=True, timeout=30)
        except (subprocess.TimeoutExpired, OSError) as e:
            return False, f"run error: {e}"
        return run.returncode == 0, run.stderr.strip()[:400]

    try:
        run = subprocess.run([str(exe)], capture_output=True, text=True, timeout=30)
    except (subprocess.TimeoutExpired, OSError) as e:
        return False, f"run error: {e}"
    if run.returncode != 0:
        return False, run.stderr.strip()[:400]
    return True, run.stdout.rstrip("\r\n")


def render_literal(macro: str, value: object) -> str | None:
    """Return the macro line, or None for `/* #undef ... */`."""
    if value is False:
        return None
    if value is True:
        return f"#define {macro} 1"
    if isinstance(value, int):
        return f"#define {macro} {value}"
    if value == "":
        return f"#define {macro}"
    s = str(value)
    if len(s) >= 2 and s[0] == '"' and s[-1] == '"':
        return f"#define {macro} {s}"
    return f"#define {macro} {s}"


def resolve_probe(entry: Entry, success: bool, stdout: str) -> str | None:
    if entry.invert:
        success = not success
    if not success:
        return None
    if entry.mode == "bool":
        val = entry.value
        if val is None:
            return f"#define {entry.macro} 1"
        if val == "":
            return f"#define {entry.macro}"
        return f"#define {entry.macro} {val}"
    if entry.mode == "number":
        val = stdout.strip()
        if not re.fullmatch(r"-?\d+", val):
            raise ConfigureError(
                f"probe for {entry.macro} (number) returned non-integer: {val!r}"
            )
        return f"#define {entry.macro} {val}"
    if entry.mode == "string":
        escaped = stdout.replace("\\", "\\\\").replace('"', '\\"')
        return f"#define {entry.macro} \"{escaped}\""
    raise ConfigureError(f"unknown mode {entry.mode!r}")


def render_entry(entry: Entry, result: str | None) -> list[str]:
    out: list[str] = []
    out.append(f"/* {entry.comment} */" if entry.comment else f"/* Define {entry.macro}. */")
    if entry.guard:
        out.append(f"#if {entry.guard}")
    out.append(result if result is not None else f"{UNDEF_PREFIX}{entry.macro} */")
    if entry.guard:
        out.append(f"#endif /* {entry.guard} */")
    return out


def generate(template_path: Path, toml_path: Path, out_path: Path, compiler: str):
    template = template_path.read_text(encoding="utf-8")
    if template.count(MARKER) != 1:
        raise ConfigureError(
            f"template {template_path} must contain exactly one {MARKER!r} marker"
        )

    entries = load_entries(toml_path)
    tc = Toolchain(compiler)

    if not _msvc_environment_ready(compiler):
        print(
            "error: MSVC toolchain detected but not initialized "
            "(INCLUDE/LIB are empty).\n"
            "       Run PCbuild\\configure.bat, or run this from a "
            "\"Developer Command Prompt for VS\",\n"
            "       or call vcvarsall.bat <arch> first.",
            file=sys.stderr,
        )
        return 1

    print(f"[configure] {len(entries)} entries; compiler={compiler}", file=sys.stderr)

    block: list[str] = []
    with tempfile.TemporaryDirectory(prefix="pyconfig-probe-") as tmp:
        for entry in entries:
            if entry.mode == "literal":
                result = render_literal(entry.macro, entry.value)
                print(f"[configure] literal {entry.macro:40s} "
                      f"{'define' if result else 'undef '}", file=sys.stderr)
            else:
                success, stdout = run_probe(entry, tc, tmp)
                result = resolve_probe(entry, success, stdout)
                print(f"[configure] {entry.mode:6s} {entry.macro:40s} "
                      f"{'ok' if success else 'no '}", file=sys.stderr)
            block.extend(render_entry(entry, result))
            block.append("")

    generated = template.replace(MARKER, "\n".join(block))
    out_path.write_text(generated, encoding="utf-8")
    print(f"[configure] wrote {out_path}", file=sys.stderr)


def main(argv: list[str] | None = None) -> int:
    here = Path(__file__).resolve().parent
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--config", default=str(here / "configure.toml"))
    ap.add_argument("--template", default=str(here / "pyconfig.h.in"))
    ap.add_argument("--output", default=str(here / "pyconfig.h"))
    ap.add_argument("--compiler", default=None,
                    help="C compiler driver (default: auto-detect cl/cc)")
    args = ap.parse_args(argv)

    try:
        generate(Path(args.template), Path(args.config),
                 Path(args.output), detect_compiler(args.compiler))
    except ConfigureError as e:
        print(f"error: {e}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
