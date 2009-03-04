@echo off

set curDir=%CD%
call "Set Avidemux Environment Variables"

if errorlevel 1 goto error

cd "%sourceDir%"
echo Cleaning build directories
rmdir /s /q build 2> NUL

cd "%sourceDir%\plugins"
rmdir /s /q build 2> NUL

if exist build goto removalFailure
cd "%sourceDir%"
if exist build goto removalFailure

mkdir build
mkdir plugins\build

echo Performing Subversion update
svn up .

cd %curDir%
call "1b. Continue Build"

goto end

:removalFailure
echo ERROR - build directories could not be fully deleted.
echo Aborting.

:error
set ERRORLEVEL=1

:end
pause