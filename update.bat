@echo off

echo This simple script will update the distribution. All tracked files will be reset and overwriten!

setlocal
set PATH=%~dp0\toolchain\bin;%PATH%
git remote set-url origin git://github.com/drahosp/PaPP.git
git remote set-url --push origin git@github.com:drahosp/PaPP.git
git checkout master
git reset --hard HEAD
git fetch origin
git merge origin/master
endlocal

