@echo off
setlocal
set PATH=%~dp0/toolchain/bin;%PATH%
start cmd %*
endlocal
