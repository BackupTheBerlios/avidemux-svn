cd "%sourceDir%"
echo Cleaning build directory (%CD%\%BuildFolder%)
rmdir /s /q %buildFolder% 2> NUL

cd "%sourceDir%\plugins"
echo Cleaning plugin build directory (%CD%\%BuildFolder%)
rmdir /s /q %buildFolder% 2> NUL

if exist %buildFolder% goto removalFailure
cd "%sourceDir%"
if exist %buildFolder% goto removalFailure

mkdir %buildFolder%
mkdir plugins\%buildFolder%

echo Performing Subversion update
svn up .

goto :EOF

:removalFailure
echo ERROR - build directories could not be fully deleted.
echo Aborting.

:error
set ERRORLEVEL=1