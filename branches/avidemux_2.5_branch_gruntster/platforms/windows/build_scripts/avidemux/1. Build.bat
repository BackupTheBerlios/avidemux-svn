@echo off

:start
echo MinGW build for Avidemux
echo ========================
echo 1. 32-bit build
echo 2. 64-bit build
echo 3. Debug build
echo X. Exit
echo.

choice /c 123x

if errorlevel 1 set BuildBits=32
if errorlevel 2 set BuildBits=64
if errorlevel 3 (
	set DebugFlags=-DCMAKE_BUILD_TYPE=Debug
	echo.
	echo -- Debug mode set --
	echo.
	goto :start	)

if errorlevel 4 goto end

verify >nul
echo.

call "1a. Prepare Build"

:end