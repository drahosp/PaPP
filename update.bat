@echo off
startlocal
set PATH=%~dp0/toolchain/bin;%PATH%
git fetch
git merge FETCH_HEAD
endlocal