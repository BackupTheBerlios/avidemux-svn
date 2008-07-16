@echo off

set curDir=%CD%
call "Set Avidemux 2.4 Environment Variables"

if errorlevel 1 goto error

cd "%sourceDir%"
echo Cleaning build directories
rmdir /s /q build 2> NULL
rmdir /s /q buildcli 2> NULL

if exist build goto removalFailure
if exist buildcli goto removalFailure

mkdir buildcli
mkdir build

echo Performing Subversion update
svn up .

cd %curDir%
call "1b. Continue Build"

goto end

:removalFailure
echo ERROR - build or buildcli could not be fully deleted.
echo Aborting.
pause

:error
set ERRORLEVEL=1

:end
