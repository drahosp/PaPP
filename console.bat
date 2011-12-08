@echo off

echo This script ensures path to toolchain is set correctly for commandline work.

setlocal
set PATH=%~dp0\toolchain\bin;%~dp0\toolchain\bin\OpenCL\x86;%PATH%
start cmd %*
endlocal
