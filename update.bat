@echo off

echo This simple script will update the distribution. All tracked files will be reset and overwriten!

setlocal
set PATH=%~dp0\toolchain\bin;%PATH%
git reset --hard HEAD
git fetch git://github.com/drahosp/PaPP.git HEAD
git merge -s ours FETCH_HEAD
endlocal

