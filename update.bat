@echo off
setlocal
set PATH=%~dp0/toolchain/bin;%PATH%
git fetch
git merge -s recursive -Xtheirs FETCH_HEAD
endlocal
