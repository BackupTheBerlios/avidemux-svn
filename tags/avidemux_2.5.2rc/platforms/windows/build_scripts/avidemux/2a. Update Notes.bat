@echo off

call "Set Avidemux Environment Variables"
if errorlevel 1 goto error

set PATH=%msysDir%\bin;%PATH%
set curDir=%CD%

echo -- Generating Change Log.html --
cd "..\..\installer"
sh genlog.sh

echo -- Generating Package Notes.html --
cd "%curDir%\Tools"
sh gennotes.sh

echo -- Generating Touch Files.html --
sh gentouch.sh

echo -- Generating avidemux.pot --
cd "%sourceDir%\po"
sh update_pot.bash

cd "%curDir%"
goto end

:error
set ERRORLEVEL=1

:end
pause