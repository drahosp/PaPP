@echo off
setlocal
set PATH=%~dp0/toolchain/bin;%PATH%
git reset --hard HEAD
git fetch
git merge -s recursive -Xtheirs FETCH_HEAD
endlocal
