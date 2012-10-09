set BuildGenerator=CodeBlocks - MinGW Makefiles

if "%BuildGenerator%" == "CodeBlocks - MinGW Makefiles" copy "%curDir%\Tools\avidemux.workspace" "%SourceDir%\%buildFolder%"

rem ## FFmpeg ##
set _PATH=%PATH%
set _CFLAGS=%CFLAGS%
set _LDFLAGS=%LDFLAGS%

SET PATH=%msysDir%/bin;%PATH%
set CFLAGS=-m%BuildBits%
set LDFLAGS=-shared-libgcc -m%BuildBits%

cd "%sourceDir%\%buildFFmpegFolder%"
cmake -G"MSYS Makefiles" -DCMAKE_INSTALL_PREFIX="%buildDir%" %DebugFlags% ../../avidemux_ffmpeg

if errorlevel 1 goto error
pause

make install
if errorlevel 1 goto error

set PATH=%_PATH%
set CFLAGS=%_CFLAGS%
set LDFLAGS=%_LDFLAGS%

rem ## Core ##
if "%Debug%" EQU "1" (
	set LeakFlags=-DFIND_LEAKS=ON	)

cd "%sourceDir%\%buildCoreFolder%"
cmake -G"%BuildGenerator%" -DCMAKE_INSTALL_PREFIX="%buildDir%" %DebugFlags% %LeakFlags% ../../avidemux_core

if errorlevel 1 goto error
if "%BuildGenerator%" == "CodeBlocks - MinGW Makefiles" copy "%curDir%\Tools\avidemux.layout" admCore.layout
pause

rmdir /s /q "%buildDir%\help" 2> NUL
mingw32-make install
if errorlevel 1 goto error

rem ## Qt4 ##
cd "%sourceDir%\%buildQtFolder%"
cmake -G"%BuildGenerator%" -DCMAKE_INSTALL_PREFIX="%buildDir%" %DebugFlags% ../../avidemux/qt4

if errorlevel 1 goto error
if "%BuildGenerator%" == "CodeBlocks - MinGW Makefiles" (
	copy "%curDir%\Tools\avidemux.layout" Avidemux_qt4.layout
	patch -p0 -i "%curDir%\Tools\Avidemux_qt4.cbp.patch"	)
pause

rem ## CLI ##
cd "%sourceDir%\%buildCliFolder%"
cmake -G"%BuildGenerator%" -DCMAKE_INSTALL_PREFIX="%buildDir%" %DebugFlags% ../../avidemux/cli

if errorlevel 1 goto error
if "%BuildGenerator%" == "CodeBlocks - MinGW Makefiles" copy "%curDir%\Tools\avidemux.layout" Avidemux_cli.layout
pause

rem ## GTK ##
rem cd "%sourceDir%\%buildGtkFolder%"
rem cmake -G"%BuildGenerator%" -DCMAKE_INSTALL_PREFIX="%buildDir%" %DebugFlags% ../../avidemux/gtk

rem if errorlevel 1 goto error
rem if "%BuildGenerator%" == "CodeBlocks - MinGW Makefiles" copy "%curDir%\Tools\avidemux.layout" Avidemux_gtk.layout
rem pause

rem ## Qt4 ##
cd "%sourceDir%\%buildQtFolder%"
mingw32-make install
if errorlevel 1 goto error

rem ## CLI ##
cd "%sourceDir%\%buildCliFolder%"
mingw32-make install
if errorlevel 1 goto error

rem ## GTK ##
rem cd "%sourceDir%\%buildGtkFolder%"
rem mingw32-make install
rem if errorlevel 1 goto error

rem ## Plugins ##
set msysSourceDir=%sourceDir:\=/%
cd "%sourceDir%\%buildPluginFolder%"
cmake -G"%BuildGenerator%" -DCMAKE_INSTALL_PREFIX="%buildDir%" -DAVIDEMUX_SOURCE_DIR="%msysSourceDir%" -DPLUGIN_UI=ALL -DUSE_SYSTEM_SPIDERMONKEY=ON -DCMAKE_INCLUDE_PATH="%SpiderMonkeySourceDir%" -DCMAKE_LIBRARY_PATH="%SpiderMonkeyLibDir%" %DebugFlags% ../../avidemux_plugins

if errorlevel 1 goto error
if "%BuildGenerator%" == "CodeBlocks - MinGW Makefiles" copy "%curDir%\Tools\avidemux.layout" "%sourceDir%\%buildPluginFolder%\AdmPlugins.layout"
pause

rem ## Plugins ##
cd "%sourceDir%\%buildPluginFolder%"
rmdir /s /q "%buildDir%\plugins" 2> NUL
mingw32-make install
if errorlevel 1 goto error

goto :EOF

:error
exit /b 1