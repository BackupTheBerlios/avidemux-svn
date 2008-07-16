@echo off

call "Set Avidemux 2.4 Environment Variables"

cd "%sourceDir%\build"

cmake -DUSE_LATE_BINDING=1 -G"MinGW Makefiles" ..

if errorlevel 1 goto error

pause
mingw32-make avidemux2_gtk
mingw32-make avidemux2_qt4
mingw32-make mofiles
mingw32-make qmfiles

copy /y .\avidemux\avidemux2_gtk.exe "%buildDir%"
copy /y .\avidemux\avidemux2_qt4.exe "%buildDir%"

cd ..\buildcli

cmake -DUSE_LATE_BINDING=1 -G"MinGW Makefiles" -DNO_SDL=1 ..
mingw32-make avidemux2_cli

copy /y .\avidemux\avidemux2_cli.exe "%buildDir%"

echo Finished!
goto end

:error
set ERRORLEVEL=1

:end
pause