@echo off

call "../Set Common Environment Variables"

if errorlevel 1 goto error

set PATH=%PATH%;%devDir%\Git\bin

rm "%usrLocalDir%/include/x264.h"
cd "%devDir%"

rm -r -f x264

echo Downloading from git
git clone git://git.videolan.org/x264.git
if errorlevel 1 goto end

cd "%devDir%/x264"
sh ./configure --prefix=/usr/local --enable-shared
if errorlevel 1 goto end

make install
if errorlevel 1 goto end

copy "%usrLocalDir%/bin/libx264-*.dll" "%devDir%\avidemux_2.5_build"

:end
pause