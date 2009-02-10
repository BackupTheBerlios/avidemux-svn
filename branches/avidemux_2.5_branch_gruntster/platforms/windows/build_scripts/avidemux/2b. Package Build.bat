@echo off

call "Set Avidemux Environment Variables"

cd Tools
call "Get Revision Number"
cd ..

set curDir=%CD%
set zipFile=avidemux_2.5_r%revisionNo%_win32.zip
set exeFile=avidemux_2.5_r%revisionNo%_win32.exe
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

echo -- Generating %exeFile% --
cd %buildDir%
"%nsisDir%\makensis" /V2 /NOCD /DNSIDIR="%curDir%\..\..\installer" /DEXEDIR="%packageDir%" "%curDir%\..\..\installer\avidemux.nsi"

pause
