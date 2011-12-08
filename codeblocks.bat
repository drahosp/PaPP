@echo off

echo This is a simple startup script for codeblocks that will make sure user settings remain untouched after update. Running codeblocks directly will works too but your settings will be reset on update.

setlocal
set PATH=%~dp0\toolchain\bin;%~dp0\toolchain\bin\OpenCL\x86;%PATH%
set APPDATA=%~dp0\apps\codeblocks\settings
mkdir %APPDATA%
mkdir %APPDATA%\codeblocks

echo Copy the default settings into the roaming APPDATA profile. This will prevent update from overwriting it.
echo n | copy /-Y %~dp0\apps\codeblocks\default.conf %APPDATA%\codeblocks\

start %~dp0\apps\codeblocks\codeblocks.exe "%~dp0\examples\examples.workspace" %*
endlocal
