@echo off
setlocal
set PATH=%~dp0/toolchain/bin;%PATH%
start %~dp0/apps/codeblocks/codeblocks.exe %~dp0/examples/examples.workspace %*
endlocal
