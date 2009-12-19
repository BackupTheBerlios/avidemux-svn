@echo off

set curDir=%CD%
call "Set Avidemux Environment Variables"

if errorlevel 1 goto error

cd "%sourceDir%"
echo Cleaning build directory (%CD%\%BuildFolder%)
rmdir /s /q %buildFolder% 2> NUL

cd "%sourceDir%\plugins"
rmdir /s /q %buildFolder% 2> NUL

if exist %buildFolder% goto removalFailure
cd "%sourceDir%"
if exist %buildFolder% goto removalFailure

mkdir %buildFolder%
mkdir plugins\%buildFolder%

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
pause

:end
