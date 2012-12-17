# == Build environment ==
if (${env:PROCESSOR_ARCHITECTURE} -eq "AMD64")
{
    $programFiles32 = ${env:ProgramFiles(x86)}
}
else
{
    $programFiles32 = $env:ProgramFiles
}

[string] $devDir = (Get-Location).Drive.Name + ":\admDev"

[string] $msysDir = Join-Path $devDir "msys"

[string] $mingwDir = Join-Path $devDir "mingw"
[string] $mingwBinDir = Join-Path $mingwDir "bin"
[string] $mingwMakePath = Join-Path $mingwBinDir "mingw32-make.exe"
[string] $mingwIncludeDir = Join-Path $mingwDir "i686-w64-mingw32\include"
[string] $mingwLib32Dir = $mingwBinDir
[string] $mingwLib64Dir = Join-Path $mingwDir "i686-w64-mingw32\lib64"

[string] $externalLib32Prefix = Join-Path $devDir "sharedLibs32"
[string] $externalLib64Prefix = Join-Path $devDir "sharedLibs64"
[string] $externalToolsDir = Join-Path $devDir "sharedTools"   # yasm, nasm, etc needed for compilation

[string] $cmakePath = Join-Path $devDir "cmake\bin\cmake.exe"
[string] $gitPath = Join-Path $devDir "git\bin\git.exe"    # required for Aften & x264
[string] $sevenZipPath = Join-Path $env:ProgramFiles "7-Zip\7z.exe"  # required to extract tar files

# == Packaging tools ==
[string] $svnPath = Join-Path $env:ProgramFiles "TortoiseSVN\bin\svn.exe"
[string] $nsisPath = Join-Path $programFiles32 "NSIS\makensis.exe"
[string] $advZipPath = Join-Path $externalToolsDir "advzip.exe"

# == Qt directories ==
[string] $qtVersion = "4.8.4"
[string] $qtPrefix_gcc_x86 = Join-Path $devDir "qt-$qtVersion-gcc-x86"
[string] $qtPrefix_gcc_x86_dbg = Join-Path $devDir "qt-$qtVersion-gcc-x86-dbg"
[string] $qtPrefix_gcc_x86_64 = Join-Path $devDir "qt-$qtVersion-gcc-x86-64"
[string] $qtPrefix_gcc_x86_64_dbg = Join-Path $devDir "qt-$qtVersion-gcc-x86-64-dbg"

[string] $qtPrefix_msvc10_x86 = Join-Path $devDir "qt-$qtVersion-msvc10-x86"
[string] $qtPrefix_msvc10_x86_dbg = Join-Path $devDir "qt-$qtVersion-msvc10-x86-dbg"
[string] $qtPrefix_msvc10_x86_64 = Join-Path $devDir "qt-$qtVersion-msvc10-x86-64"
[string] $qtPrefix_msvc10_x86_64_dbg = Join-Path $devDir "qt-$qtVersion-msvc10-x86-64-dbg"

# == SpiderMonkey ==
[bool] $spiderMonkey_useSystemVersion = $true

[string] $spiderMonkey_includeDir_x86 = Join-Path $devDir "js-1.7.0-gcc-x86\src"
[string] $spiderMonkey_libDir_x86 = Join-Path $spiderMonkey_includeDir_x86 "WINNT6.1_OPT.OBJ"

[string] $spiderMonkey_includeDir_x86_64 = Join-Path $devDir "js-1.7.0-gcc-x86-64\src"
[string] $spiderMonkey_libDir_x86_64 = Join-Path $spiderMonkey_includeDir_x86_64 "WINNT6.1_OPT.OBJ"

# == Avidemux CMake checks ==
[string] $admCore_gcc_CmakeCheck = "SUMMARY_Miscellaneous:INTERNAL=gettext=;SDL="
[string] $admQt_gcc_CmakeCheck = "SUMMARY_Miscellaneous:INTERNAL=gettext=;SDL=;OpenGL=1"
[string] $admCli_gcc_CmakeCheck = "SUMMARY_Miscellaneous:INTERNAL=gettext=;SDL="
[string] $admPlugin_gcc_CmakeCheck =
    "SUMMARY_Audio_Decoder:INTERNAL=FAAD=1;Vorbis=1;libdca=;opencore-amrwb=1;opencore-amrnb=1;libvpx=1`|" +`
    "SUMMARY_Audio_Encoder:INTERNAL=Aften=1;TWOLAME=1;LAME=1;DCAENC=;FAAC=1;Vorbis=1`|" +`
    "SUMMARY_Miscellaneous:INTERNAL=gettext=;OpenGL=1;FreeType2=1;FRIBIDI=1;FontConfig=1`|" +`
    "SUMMARY_Scripting:INTERNAL=QtScript=1;QtScript Debugger=1;SpiderMonkey=1;TinyPy=1`|" +`
    "SUMMARY_User_Interface:INTERNAL=Common=1;GTK+=0;Qt 4=1;CLI=1`|" +`
    "SUMMARY_Video_Encoder:INTERNAL=Xvid=1;x264=1`|" +`
    "USE_SYSTEM_SPIDERMONKEY:BOOL=ON"

[string] $admCore_msvc10_CmakeCheck = "SUMMARY_Miscellaneous:INTERNAL=gettext=;SDL="
[string] $admQt_msvc10_CmakeCheck = "SUMMARY_Miscellaneous:INTERNAL=gettext=;SDL=;OpenGL=1"
[string] $admCli_msvc10_CmakeCheck = "SUMMARY_Miscellaneous:INTERNAL=gettext=;SDL="
[string] $admPlugin_msvc10_CmakeCheck =
    "SUMMARY_Audio_Decoder:INTERNAL=FAAD=1;Vorbis=1;libdca=;opencore-amrwb=1;opencore-amrnb=1;libvpx=0`|" +`
    "SUMMARY_Audio_Encoder:INTERNAL=Aften=1;TWOLAME=1;LAME=1;DCAENC=;FAAC=1;Vorbis=1`|" +`
    "SUMMARY_Miscellaneous:INTERNAL=gettext=;OpenGL=1`|" +`
    "SUMMARY_Scripting:INTERNAL=QtScript=1;QtScript Debugger=1;SpiderMonkey=1;TinyPy=1`|" +`
    "SUMMARY_User_Interface:INTERNAL=Common=1;GTK+=0;Qt 4=1;CLI=1`|" +`
    "SUMMARY_Video_Encoder:INTERNAL=Xvid=1;x264=1`|" +`
    "USE_SYSTEM_SPIDERMONKEY:BOOL=ON"

# == Avidemux installation directory ==
[string] $admVersion = "2.7"

[string] $admInstallDirPattern = "..\..\..\install_`${compiler}_`${arch}`${debug}"
[string] $admSdkInstallDirPattern = "..\..\..\install_`${compiler}_`${arch}`${debug}_sdk"

[string] $admPackageDir = ".\Packages"
[string] $admPackagePattern = "avidemux_${admVersion}_r`${revision}_win`${bits}"
[string] $admSdkPackagePattern = "avidemux_sdk_${admVersion}_r`${revision}_win`${bits}"

[string] $avsProxyDir = Join-Path $devDir "avsproxy-$admVersion-msvc10-x86"