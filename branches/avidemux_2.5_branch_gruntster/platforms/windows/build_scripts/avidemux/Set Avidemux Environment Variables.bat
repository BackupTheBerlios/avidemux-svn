call "..\Set Common Environment Variables"

if errorlevel 1 goto error

zip > NUL 2> NUL
if errorlevel 1 (
	echo Info-ZIP could not be found in the PATH.  Please download from http://www.info-zip.org
	goto error
)

advzip > NUL 2> NUL
if errorlevel 1 (
	echo AdvanceCOMP could not be found in the PATH.  Please download from http://advancemame.sourceforge.net
	goto error
)

svn help > NUL 2> NUL
if errorlevel 1 (
	echo Subversion could not be found in the PATH.  Please download from http://subversion.tigris.org/
	goto error
)

xsltproc --version > NUL 2> NUL
if errorlevel 1 (
	echo xsltproc could not be found in the PATH.  Please download from http://www.zlatkovic.com
	goto error
)

if "%BuildBits%" == "32" (set buildFolder=build)
if "%BuildBits%" == "64" (set buildFolder=build64)

set buildDir=%devDir%\avidemux_2.5_%buildFolder%
set curDir=%CD%
cd ..\..\..\..
set sourceDir=%CD%
cd "%curDir%"

if not exist "%sourceDir%" (
	echo Source directory could not be found at "%sourceDir%".
	goto error
)

if "%BuildBits%" == "32" (set jsFolder=js)
if "%BuildBits%" == "64" (set jsFolder=js-64)

set SpiderMonkeySourceDir=%devDir%\%jsFolder%\src
set SpiderMonkeyLibDir=%devDir%\%jsFolder%\src\WINNT6.0_OPT.OBJ

goto end

:error
set ERRORLEVEL=1

:end