@echo off

set ProgramFiles32=%ProgramFiles%
if "%PROCESSOR_ARCHITECTURE%" == "AMD64" for /D %%d in ("%ProgramFiles(x86)%") do set ProgramFiles32=%%~fsd
set nsisDir=%ProgramFiles32%\NSIS

set devDir=E:\Dev

if "%BuildBits%" == "32" (
	set mingwDir=%devDir%\MinGW
	set msysDir=E:/Dev/MSYS
	set qtDir=%devDir%\Qt
	goto :setVars )

if "%BuildBits%" == "64" (
	set mingwDir=%devDir%\MinGW-w64
	set msysDir=E:/Dev/MSYS-64
	set qtDir=%devDir%\Qt-64
	goto :setVars )

echo Error - BuildBits variable not set
goto error

:setVars
set usrLocalDir=%msysDir%/local
set CMAKE_INCLUDE_PATH=%usrLocalDir%/include
set CMAKE_LIBRARY_PATH=%usrLocalDir%/lib
set PKG_CONFIG_PATH=%usrLocalDir%\lib\pkgconfig
set SDLDIR=%usrLocalDir%
set CFLAGS=-I%CMAKE_INCLUDE_PATH% -L%CMAKE_LIBRARY_PATH%
set CXXFLAGS=-I%CMAKE_INCLUDE_PATH% -L%CMAKE_LIBRARY_PATH%
set LDFLAGS=-L%CMAKE_LIBRARY_PATH%

if exist "%qtDir%" (
	for /f %%d in ('dir /b /ad /on %qtDir%') do set qtVer=%%d
) else (
	echo Qt 4 could not be found at "%qtDir%".  Please download from http://www.trolltech.com
	goto error
)

set qtDir=%qtDir%\%qtVer%

if exist "%devDir%\CMake 2.6\bin" (
	set cmakeDir=%devDir%\CMake 2.6\bin
	goto foundCMake
)

if exist "%ProgramFiles32%\CMake 2.6\bin" (
	set cmakeDir=%ProgramFiles32%\CMake 2.6\bin
) else (
	echo CMake could not be found.  Please download from http://www.cmake.org
	goto error
)

:foundCMake
if not exist "%mingwDir%" (
	echo MinGW could not be found at "%mingwDir%".  Please download from http://www.mingw.org
	goto error
)

if not exist "%CMAKE_INCLUDE_PATH%" (
	echo Include path could not be found at "%CMAKE_INCLUDE_PATH%".
	goto error
)

if not exist "%CMAKE_LIBRARY_PATH%" (
	echo Library path could not be found at "%CMAKE_LIBRARY_PATH%".
	goto error
)

if not exist "%nsisDir%" (
	echo NSIS could not be found at "%nsisDir%".  Please download from http://nsis.sourceforge.net
	goto error
)

set PATH=%PATH%;%cmakeDir%;%mingwDir%\bin;%usrLocalDir%\bin;%msysDir%\bin;%msysDir%\local32\bin;%qtDir%\bin

goto end

:error
set ERRORLEVEL=1

:end