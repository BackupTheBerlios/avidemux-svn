@echo off

echo Package Avidemux
echo ================
echo 1. 32-bit package
echo 2. 64-bit package
echo X. Exit
echo.

choice /c 12x

if errorlevel 1 set BuildBits=32
if errorlevel 2 set BuildBits=64
if errorlevel 3 goto end

verify >nul
echo.

call "2a. Update Notes.bat"

if errorlevel 1 goto error

call "2b. Package Build.bat"

goto end

:error
set ERRORLEVEL 1

:end