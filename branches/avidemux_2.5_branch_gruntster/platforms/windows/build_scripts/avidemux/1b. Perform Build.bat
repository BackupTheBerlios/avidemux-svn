cd "%sourceDir%\%buildFolder%"
cmake -G"MSYS Makefiles" -DCMAKE_INSTALL_PREFIX="%buildDir%" -DCMAKE_EXE_LINKER_FLAGS="-shared-libgcc" -DCMAKE_SHARED_LINKER_FLAGS="-shared-libgcc" -DUSE_SYSTEM_SPIDERMONKEY=ON -DCMAKE_INCLUDE_PATH="%SpiderMonkeySourceDir%" -DCMAKE_LIBRARY_PATH="%SpiderMonkeyLibDir%" %DebugFlags% ..

if errorlevel 1 goto error
pause

set msysSourceDir=%sourceDir:\=/%
cd "%sourceDir%\%buildPluginFolder%"
cmake -G"MSYS Makefiles" -DCMAKE_INSTALL_PREFIX="%buildDir%" -DAVIDEMUX_CORECONFIG_DIR="%msysSourceDir%/%buildFolder%/config" -DAVIDEMUX_INSTALL_PREFIX="%buildDir%" -DAVIDEMUX_SOURCE_DIR="%msysSourceDir%" -DCMAKE_EXE_LINKER_FLAGS="-shared-libgcc" -DCMAKE_SHARED_LINKER_FLAGS="-shared-libgcc" %DebugFlags% ../plugins

if errorlevel 1 goto error
pause

cd "%sourceDir%\%buildFolder%"
make install

if errorlevel 1 goto error

cd "%sourceDir%\%buildPluginFolder%"
rmdir /s /q "%buildDir%\plugins" 2> NUL
make install

if errorlevel 1 goto error
goto :EOF

:error
exit /b 1