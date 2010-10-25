@echo off

echo This script ensures path to toolchain is set correctly for commandline work.

setlocal
set PATH=%~dp0/toolchain/bin;%PATH%
start cmd %*
endlocal
