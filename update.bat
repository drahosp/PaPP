@echo off

echo This simple script will update the distribution. All tracked files will be reset and overwriten!

setlocal
set PATH=%~dp0\toolchain\bin;%PATH%
git reset --hard HEAD
git fetch
git merge -s recursive -Xtheirs FETCH_HEAD
endlocal
