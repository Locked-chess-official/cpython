import os
import re
import datetime
import sys
from pathlib import Path

def is_valid_macro_name(macro_name: str):
    """
    Determine whether a macro name is valid using Python's standard library methods.

    Args:
        macro_name: The macro name to check.

    Returns:
        bool: True if it's a valid Python identifier, False otherwise.
    """
    # Empty string is invalid
    if not macro_name:
        return False

    # Use str.isidentifier() to check for valid identifier syntax
    return macro_name.isidentifier()

def extract_macro_name(line: str):
    """Extract the macro name from a #define line (handles spaces between # and define)."""
    line = line.strip()

    # Match '#', optional spaces, 'define', spaces, and the macro name
    match = re.match(r'^\s*#\s*define\s+([A-Za-z_][A-Za-z0-9_]*)', line)
    if not match:
        return None

    candidate = match.group(1)

    # Validate with standard identifier rules
    if candidate and is_valid_macro_name(candidate):
        return candidate
    return None

def extract_macro_name_and_value(line: str):
    """Extract the macro name and value from a #define line."""
    line = line.strip()

    # Match '#', optional spaces, 'define', spaces, and the macro name
    match = re.match(r'^\s*#\s*define\s+([A-Za-z_][A-Za-z0-9_]*)\s+(.*)', line)
    if not match:
        macro_name = extract_macro_name(line)
        return macro_name, None
    macro, value = match.groups()
    return macro, value

def is_standard_python_macro(macro_name: str):
    """
    Check whether a macro follows Python's standard naming conventions.
    Rules: Starts with Py, PY, _Py, _PY, or ends with _H.
    """
    standard_prefixes = ('Py', 'PY', '_Py', '_PY')
    return macro_name.startswith(standard_prefixes)

def generate_undef_code(macro_name: str):
    """Generate the code to undefine a macro."""
    return f"""#undef {macro_name}
#ifdef _Py_FORWARD_DEFINE_{macro_name}
#undef _Py_FORWARD_DEFINE_{macro_name}
#pragma pop_macro("{macro_name}")
#endif

"""

def generate_keep_now_macro_code(macro_name: str):
    """
    Generate a Python keep now macro for a given macro name.
    """
    return f"""#ifdef {macro_name}
#define _Py_FORWARD_DEFINE_{macro_name}
#pragma push_macro("{macro_name}")
#undef {macro_name}
#endif

"""

def generate_warning_macro_code(macro_name: str, value: str):
    """
    Generate a Python warning macro for a given macro name and value.
    """
    if value:
        return f"""#ifdef {macro_name}
#undef {macro_name}
#define {macro_name} _Py_Config_Macro_Warning("macro {macro_name} shouldn't be used") {value}
#endif

"""
    else:
        return f"""#ifdef {macro_name}
#undef {macro_name}
#define {macro_name} _Pragma("Warning \\"macro {macro_name} shouldn't be used\\"")
#endif

"""

def generate_python_undef_header(pyconfig_path: str, output_path: str|None=None, generate="undef"):
    """
    Generate the Python_undef.h header file.

    Args:
        pyconfig_path: Path to pyconfig.h
        output_path: Output file path, defaults to Python_undef.h in the current directory.
    """
    if output_path is None:
        file_dir = os.path.dirname(os.path.abspath(__file__))
        include_dir = Path(file_dir) / 'include'
        output_path = str(include_dir / 'Python_undef.h')
        if not include_dir.exists():
            try:
                os.makedirs(f'{file_dir}/include')
            except Exception as e:
                print(f"Error creating include directory: {e}", file=sys.stderr)
                return False

    # Read pyconfig.h
    try:
        with open(pyconfig_path, 'r', encoding='utf-8') as f:
            lines = f.readlines()
    except FileNotFoundError:
        print(f"Error: File not found {pyconfig_path}", file=sys.stderr)
        return False
    except Exception as e:
        print(f"Error reading file: {e}", file=sys.stderr)
        return False

    # Collect macros
    macros_to_undef = []
    all_macros = []
    invalid_macros = []
    warning_macro_values = {}

    print("Analyzing pyconfig.h...")

    for i, line in enumerate(lines, 1):
        macro_name = extract_macro_name(line)
        if macro_name:
            all_macros.append(macro_name)

            # Rule: any macro not starting with Py/PY/_Py/_PY and not ending with _H is considered non-standard
            if not is_standard_python_macro(macro_name):
                macros_to_undef.append(macro_name)
                macro_value = extract_macro_name_and_value(line)[1]
                if macro_value is not None:
                    warning_macro_values[macro_name] = macro_value
                else:
                    warning_macro_values[macro_name] = ""
                print(f"Line {i:4d}: Found non-standard macro '{macro_name}'")
        else:
            # Check if line looks like a define but has invalid name
            line = line.strip()
            if line.startswith('#'):
                m = re.match(r'^\s*#\s*define\s+(\S+)', line)
                if m:
                    candidate = m.group(1)
                    if candidate and not is_valid_macro_name(candidate):
                        invalid_macros.append((i, candidate))

    # Deduplicate and sort
    macros_to_undef = sorted(set(macros_to_undef))

    # Header section
    pyconfig_undef_header = f"""/*
 * pyconfig_undef.h - Automatically generated macro undefinition header
 *
 * This file is automatically generated from {os.path.basename(pyconfig_path)}
 * Contains macros that may need to be undefined to avoid conflicts with other libraries.
 *
 * WARNING: This is an automatically generated file. Do not edit manually.
 *
 * To preserve specific macros, define before including this header:
 *   #define Py_DONOTUNDEF_MACRO_NAME
 *
 * Generation rules:
 *   - Macros starting with Py, PY, _Py, _PY are preserved (Python standard)
 *   - All other macros are undefined
 *   - Macro name validation uses Python's standard identifier checking
 *
 * Generated from: {os.path.abspath(pyconfig_path)}
 * Generated at: {datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')}
 * Total valid macros found: {len(all_macros)}
 * Macros to undef: {len(macros_to_undef)}
 * Invalid macro names skipped: {len(invalid_macros)}
 */

#ifndef Py_PYCONFIG_UNDEF_H
#define Py_PYCONFIG_UNDEF_H

/*
 * Platform Note:
 * - The COMPILER macro is primarily defined in pyconfig.h on Windows
 * - Other platforms define compiler info in Python/getcompiler.c
 * - This macro and others can conflict with libraries such as V8
 */

"""

    pyconfig_keep_now_header = f"""/*
 * Python_keepon.h - Automatically generated macro keep now header
 *
 * This file is automatically generated from {os.path.basename(pyconfig_path)}
 * Contains macros that are kept during the build process.
 *
 * WARNING: This is an automatically generated file. Do not edit manually.
 *
 * Generated from: {os.path.abspath(pyconfig_path)}
 * Generated at: {datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')}
 */

#ifndef Py_PYCONFIG_KEEPON_NOW_H
#define Py_PYCONFIG_KEEPON_NOW_H

"""

    pyconfig_warning_header = f"""/*
 * Python_warning.h - Automatically generated macro warning header
 *
 * This file is automatically generated from {os.path.basename(pyconfig_path)}
 * Contains macros that are kept during the build process.
 *
 * WARNING: This is an automatically generated file. Do not edit manually.
 *
 * Generated from: {os.path.abspath(pyconfig_path)}
 * Generated at: {datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')}
 */

#ifndef Py_PYCONFIG_WARNING_H
#define Py_PYCONFIG_WARNING_H

#if defined(__GNUC__) || defined(__clang__)
#  define _Py_Config_Macro_Warning(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
#  define _Py_Config_Macro_Warning(msg) __declspec(deprecated(msg))
#else
#  define _Py_Config_Macro_Warning(msg)
#endif

"""

    # Generate undef code sections
    undef_sections = []
    keep_now_name_sections = []
    warning_sections = []
    for macro_name in macros_to_undef:
        undef_sections.append(generate_undef_code(macro_name))
        keep_now_name_sections.append(generate_keep_now_macro_code(macro_name))
        if macro_name in warning_macro_values:
            warning_sections.append(generate_warning_macro_code(macro_name, warning_macro_values[macro_name]))

    # Footer
    python_undef_footer = """#endif /* Py_PYCONFIG_UNDEF_H */
"""
    pyconfig_keep_now_footer = """#endif /* Py_PYCONFIG_KEEPON_NOW_H */
"""
    pyconfig_warning_footer = """#endif /* Py_PYCONFIG_WARNING_H */
"""

    # Write output
    try:
        with open(output_path, 'w', encoding='utf-8', newline='\n') as f:
            if generate == "undef":
                f.write(pyconfig_undef_header)
                f.writelines(undef_sections)
                f.write(python_undef_footer)
            elif generate == "keep_now":
                f.write(pyconfig_keep_now_header)
                f.writelines(keep_now_name_sections)
                f.write(pyconfig_keep_now_footer)
            elif generate == "warning":
                if sys.platform != "win32":
                    f.write(pyconfig_warning_header)
                    f.writelines(warning_sections)
                    f.write(pyconfig_warning_footer)
                else:
                    print(f"Warning generation is not supported on Windows", file=sys.stderr)
                    return False
            else:
                print(f"Invalid generate option: {generate}", file=sys.stderr)
                return False

        print(f"\n{'='*60}")
        print(f"Successfully generated: {output_path}")
        print(f"{'='*60}")
        print("Summary:")
        print(f"  - Total valid macro definitions: {len(all_macros)}")
        print(f"  - Macros to undefine: {len(macros_to_undef)}")
        print(f"  - Preserved standard macros: {len(all_macros) - len(macros_to_undef)}")
        print(f"  - Invalid macro names skipped: {len(invalid_macros)}")

        if invalid_macros:
            print(f"\nSkipped invalid macro names:")
            for line_num, invalid_macro in invalid_macros[:10]:  # show only first 10
                print(f"  Line {line_num:4d}: '{invalid_macro}'")
            if len(invalid_macros) > 10:
                print(f"  ... and {len(invalid_macros) - 10} more")

        if macros_to_undef:
            print(f"\nMacros to undefine (first 50):")
            for i, macro in enumerate(macros_to_undef[:50], 1):
                print(f"  {i:3d}. {macro}")
            if len(macros_to_undef) > 50:
                print(f"  ... and {len(macros_to_undef) - 50} more")

        return True

    except Exception as e:
        print(f"Error writing file: {e}", file=sys.stderr)
        return False

if __name__ == "__main__":
    file_dir = os.path.dirname(os.path.abspath(__file__))
    git_root = Path(file_dir).parent.parent
    if sys.platform == "win32":
        gitroot = git_root / "PC"
    else:
        gitroot = git_root
    default_pyconfig = gitroot / 'pyconfig.h'
    output_path = gitroot / 'pyconfig_undef.h'
    print(f"{'='*60}")
    print(f"Generating pyconfig_undef.h from: {default_pyconfig}")
    print(f"Output file: {output_path}")
    print(f"{'='*60}")
    if not generate_python_undef_header(str(default_pyconfig), str(output_path)):
        print("Failed to generate pyconfig_undef.h", file=sys.stderr)
        sys.exit(1)
    else:
        print("pyconfig_undef.h generated successfully.")
    output_path = gitroot / 'pyconfig_keep_now_macro.h'
    print()
    print(f"{'='*60}")
    print(f"Generating pyconfig_keep_now_macro.h from: {default_pyconfig}")
    print(f"Output file: {output_path}")
    print(f"{'='*60}")
    if not generate_python_undef_header(str(default_pyconfig), str(output_path), generate="keep_now"):
        print("Failed to generate pyconfig_keep_now_macro.h", file=sys.stderr)
        sys.exit(1)
    else:
        print("pyconfig_keep_now_macro.h generated successfully.")
    if sys.platform != "win32":
        output_path = gitroot / 'pyconfig_warning.h'
        print()
        print(f"{'='*60}")
        print(f"Generating pyconfig_warning.h from: {default_pyconfig}")
        print(f"Output file: {output_path}")
        print(f"{'='*60}")
        if not generate_python_undef_header(str(default_pyconfig), str(output_path), generate="warning"):
            print("Failed to generate pyconfig_warning.h", file=sys.stderr)
            sys.exit(1)
        else:
            print("pyconfig_warning.h generated successfully.")
