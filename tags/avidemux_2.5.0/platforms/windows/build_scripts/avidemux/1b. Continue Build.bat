@echo off

call "Set Avidemux Environment Variables"
if errorlevel 1 goto error

cd "%sourceDir%\build"
cmake -G"MSYS Makefiles" -DCMAKE_INSTALL_PREFIX="%buildDir%" -DCMAKE_EXE_LINKER_FLAGS="-shared-libgcc" -DCMAKE_SHARED_LINKER_FLAGS="-shared-libgcc" -DUSE_SYSTEM_SPIDERMONKEY=ON -DCMAKE_INCLUDE_PATH="%SpiderMonkeySourceDir%" -DCMAKE_LIBRARY_PATH="%SpiderMonkeyLibDir%" ..

if errorlevel 1 goto error
pause

set msysSourceDir=%sourceDir:\=/%
cd "%sourceDir%\plugins\build"
cmake -G"MSYS Makefiles" -DCMAKE_INSTALL_PREFIX="%buildDir%" -DAVIDEMUX_CORECONFIG_DIR="%msysSourceDir%/build/config" -DAVIDEMUX_INSTALL_PREFIX="%buildDir%" -DAVIDEMUX_SOURCE_DIR="%msysSourceDir%" -DCMAKE_EXE_LINKER_FLAGS="-shared-libgcc" -DCMAKE_SHARED_LINKER_FLAGS="-shared-libgcc" ..

if errorlevel 1 goto error
pause

cd "%sourceDir%\build"
make install

cd "%sourceDir%\plugins\build"
make install

del /s "%buildDir%\*.a"
del "%buildDir%\plugins\audioDecoder\libADM_ad_dca.dll"
del "%buildDir%\plugins\audioDecoder\libADM_ad_amrnb.dll"

echo Finished!
goto end

:error
set ERRORLEVEL=1

:end
pause