@echo off
startlocal
set PATH=%~dp0/toolchain/bin;%PATH%
start cmd %*
endlocal
