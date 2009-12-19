@echo off

call "Set Avidemux Environment Variables"

cd Tools
call "Get Revision Number"
cd ..

set curDir=%CD%
set zipFile=avidemux_2.5_r%revisionNo%_full_win32.zip
set packageDir=%CD%\%revisionNo%

echo %packageDir%
mkdir %packageDir%

copy "%buildDir%\Change Log.html" "%packageDir%"
move "Tools\Package Notes.html" "%packageDir%"
copy "%sourceDir%\po\avidemux.pot" "%packageDir%"

cd %buildDir%
zip -r "%packageDir%\%zipFile%" *
cd %curDir%
advzip -z -4 "%packageDir%\%zipFile%"

cd %buildDir%
echo -- Generating GTK+ Installer --
"%nsisDir%\makensis" /V2 /NOCD /DINST_GTK /DNSIDIR="%curDir%\..\..\installer" /DEXEDIR="%packageDir%" "%curDir%\..\..\installer\avidemux.nsi"
echo -- Generating Qt Installer --
"%nsisDir%\makensis" /V2 /NOCD /DINST_QT /DNSIDIR="%curDir%\..\..\installer" /DEXEDIR="%packageDir%" "%curDir%\..\..\installer\avidemux.nsi"

pause
