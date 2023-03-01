@echo off

set TOP_DIR=%~dp0
echo %TOP_DIR%

set Bit=%1%

::if not set 'Bit' Env, Use 32
if "%Bit%" == "32" (
	set Bit=32
	set AARCH="Win32" 
	set BuildPath=%TOP_DIR%\build32
) else (
	set Bit=64
	set AARCH="x64"
	set BuildPath=%TOP_DIR%\build64
)

if "%Bit%" == "64" (
    call "%VS2019_HOME%\VC\Auxiliary\Build\vcvarsall.bat" x64
    IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%
) else (
    call "%VS2019_HOME%\VC\Auxiliary\Build\vcvarsall.bat" x86
    IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%
)
echo "Build Windows SDK with 2019"

rmdir /q /s %TOP_DIR%\windows\
if not exist %TOP_DIR%windows md %TOP_DIR%windows
rmdir /q /s %BuildPath%
if not exist %BuildPath% md %BuildPath%

cmake -G "Visual Studio 16 2019"  -A %AARCH% -B%BuildPath% -DCMAKE_INSTALL_PREFIX=%TOP_DIR%/windows -H%TOP_DIR%
cd %BuildPath%
cmake --build . --config Release --target install -- /p:DebugType=pdbonly

goto :eof

