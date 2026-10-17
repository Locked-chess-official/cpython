@echo off
@rem
@rem Regenerate PC\pyconfig.h from PC\configure.toml.
@rem
@rem This is the Windows counterpart to the POSIX `configure` script: instead of
@rem running autoconf on configure.ac, it runs PC\configure.py on PC\configure.toml
@rem and splices the result into the PC\pyconfig.h.in template.
@rem
@rem Unlike PCbuild\build.bat (which goes through MSBuild + PlatformToolset and so
@rem never needs the compiler environment itself), this script invokes cl.exe
@rem directly.  It therefore initializes the Visual Studio build environment
@rem (vcvarsall.bat) first, exactly like PCbuild\env.bat and
@rem PCbuild\prepare_libffi.bat do, so that cl.exe finds its INCLUDE/LIB paths.
@rem
@rem Usage:
@rem   configure.bat                  (x64, auto-detect cl.exe / clang-cl.exe)
@rem   configure.bat x86              (32-bit build)
@rem   configure.bat arm64            (ARM64 build)
@rem   configure.bat x64 cl           (explicit compiler driver: cl)
@rem   configure.bat x64 clang-cl     (explicit compiler driver: clang-cl)
@rem   configure.bat x64 --python "C:\Python311\python.exe"
@rem
@rem The first argument is the target architecture (default: x64) and is passed
@rem to vcvarsall.bat to select the matching toolset. The second (optional)
@rem argument is the compiler driver (default: auto-detect).

setlocal
@set "_Py_Root=%~dp0.."
@set "_Py_PC=%_Py_Root%\PC"
@set "_Py_Arch=x64"
@set "_Py_Compiler="
@set "_Py_Python="
@set "_Py_ArchSet="

@rem ---------------------------------------------------------------------------
@rem Parse arguments: [arch] [compiler]
@rem ---------------------------------------------------------------------------
:ParseArgs
@if "%~1"=="" goto :ArgsDone
@if /I "%~1"=="-h" goto :Usage
@if /I "%~1"=="--help" goto :Usage
@if /I "%~1"=="-?" goto :Usage
@if /I "%~1"=="--python" (
    @set "_Py_Python=%~2"
    @shift
    @shift
    @goto :ParseArgs
)
@if not defined _Py_ArchSet (
    @set "_Py_Arch=%~1"
    @set "_Py_ArchSet=1"
    @shift
    @goto :ParseArgs
)
@set "_Py_Compiler=%~1"
@shift
@goto :ParseArgs

:ArgsDone

@rem Normalize a few common MSBuild-style platform spellings to vcvarsall args.
@if /I "%_Py_Arch%"=="Win32" set "_Py_Arch=x86"
@if /I "%_Py_Arch%"=="ARM"   set "_Py_Arch=arm"
@if /I "%_Py_Arch%"=="ARM64" set "_Py_Arch=arm64"

@rem ---------------------------------------------------------------------------
@rem Locate vcvarsall.bat. Mirror PCbuild\env.bat: flat goto-based flow, and the
@rem vswhere loop is NOT wrapped in a parenthesized block, because the `(x86)`
@rem inside %ProgramFiles(x86)% would otherwise terminate the block early.
@rem ---------------------------------------------------------------------------
@if not exist "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" goto :skip_vswhere
@set "VSTOOLS="
@for /F "tokens=*" %%i in ('"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -property installationPath -latest -prerelease -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64') DO @(set "VSTOOLS=%%i\VC\Auxiliary\Build\vcvarsall.bat")
@if defined VSTOOLS goto :have_vstools
:skip_vswhere
@rem Older VS / no vswhere: fall back to the VSxxxCOMNTOOLS environment variables.
@if defined VS170COMNTOOLS set "VSTOOLS=%VS170COMNTOOLS%..\..\VC\vcvarsall.bat"
@if defined VS160COMNTOOLS set "VSTOOLS=%VS160COMNTOOLS%..\..\VC\vcvarsall.bat"
@if defined VS150COMNTOOLS set "VSTOOLS=%VS150COMNTOOLS%..\..\VC\vcvarsall.bat"
@if defined VS143COMNTOOLS set "VSTOOLS=%VS143COMNTOOLS%..\..\VC\vcvarsall.bat"
@if defined VS142COMNTOOLS set "VSTOOLS=%VS142COMNTOOLS%..\..\VC\vcvarsall.bat"
@if defined VS141COMNTOOLS set "VSTOOLS=%VS141COMNTOOLS%..\..\VC\vcvarsall.bat"

:have_vstools
@if not defined VSTOOLS (
    @echo error: could not locate vcvarsall.bat. Install the Visual Studio C++ 1>&2
    @echo        toolset, or run this from a "Developer Command Prompt for VS". 1>&2
    @exit /b 1
)
@if not exist "%VSTOOLS%" (
    @echo error: vcvarsall.bat not found at "%VSTOOLS%" 1>&2
    @exit /b 1
)

@echo Initializing %_Py_Arch% build environment...
@call "%VSTOOLS%" %_Py_Arch%
@if errorlevel 1 (
    @echo error: vcvarsall.bat failed for architecture %_Py_Arch% 1>&2
    @exit /b 1
)

@rem ---------------------------------------------------------------------------
@rem Locate a Python 3.11+ interpreter (required for tomllib).
@rem If build.bat passed one via --python, use it directly; otherwise reuse
@rem PCbuild\find_python.bat, then fall back to the `py` launcher.
@rem ---------------------------------------------------------------------------
@set "PYTHON="
@if not "%_Py_Python%"=="" (
    @if exist "%_Py_Python%" (
        @set PYTHON="%_Py_Python%"
    ) else (
        @set "PYTHON=%_Py_Python%"
    )
)

@if "%PYTHON%"=="" if exist "%_Py_Root%\PCbuild\find_python.bat" (
    @call "%_Py_Root%\PCbuild\find_python.bat" -q > "%TEMP%\configure_find_python.tmp"
    @set /P PYTHON= < "%TEMP%\configure_find_python.tmp"
    @del "%TEMP%\configure_find_python.tmp" >nul 2>nul
)

@rem Fall back to the `py` launcher, newest first, requiring tomllib (3.11+).
@if "%PYTHON%"=="" (
    @for %%p in (3.15 3.14 3.13 3.12 3.11) do @(py -%%p -c "import tomllib" >nul 2>nul) && (set "PYTHON=py -%%p") && goto :have_python
)
@goto :have_python

:have_python
@if "%PYTHON%"=="" (
    @echo error: no Python 3.11+ interpreter found [tomllib is required] 1>&2
    @exit /b 1
)

@rem ---------------------------------------------------------------------------
@rem Run the generator. configure.py auto-detects cl.exe / clang-cl.exe; pass
@rem --compiler only if the user requested a specific driver.
@rem ---------------------------------------------------------------------------
@if "%_Py_Compiler%"=="" (
    @%PYTHON% -E "%_Py_PC%\configure.py" --template "%_Py_PC%\pyconfig.h.in" --config "%_Py_PC%\configure.toml" --output "%_Py_PC%\pyconfig.h"
) else (
    @%PYTHON% -E "%_Py_PC%\configure.py" --compiler "%_Py_Compiler%" --template "%_Py_PC%\pyconfig.h.in" --config "%_Py_PC%\configure.toml" --output "%_Py_PC%\pyconfig.h"
)

@if errorlevel 1 (
    @echo error: configure.py failed 1>&2
    @exit /b 1
)

@echo Regenerated "%_Py_PC%\pyconfig.h"
@exit /b 0

:Usage
@echo.%~nx0 [arch] [compiler] [--python PATH]
@echo.
@echo.Regenerate PC\pyconfig.h from PC\configure.toml.
@echo.
@echo.  arch       Target architecture for vcvarsall.bat: x64 (default), x86,
@echo.            arm64, arm, amd64, Win32, ARM64, ARM.
@echo.  compiler   Optional C compiler driver: cl (default, auto-detected) or
@echo.            clang-cl.
@echo.  --python   Use this Python interpreter (3.11+, for tomllib) instead of
@echo.            auto-detecting one.
@echo.
@echo.Requires Python 3.11+ and the Visual Studio C++ toolset.
@exit /b 127
