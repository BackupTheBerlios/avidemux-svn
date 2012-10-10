$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

function PrepareBuildDirectories([string] $sourceDir, [string] $buildDir, [string] $installDir)
{
    if (!(Test-Path $sourceDir))
    {
        throw "Source directory couldn't be determined ($sourceDir)"
    }

    Create-FreshDirectory($buildDir)
    Create-FreshDirectory($installDir)
}

function SetupEnvironment([string] $compiler, [string] $arch, [bool] $debug)
{
    if ($compiler -eq "gcc")
    {
        Setup-MingwEnvironment $arch $debug
    }
    elseif ($compiler -eq "msvc10")
    {
        Setup-Msvc10Environment $arch $debug
    }
    else
    {
        throw "Invalid compiler ($compiler)"
    }
}

function CheckCmakeCache([string] $component, [string] $compiler, [string] $buildDir)
{
    [string] $pipeSeparatedLines = Get-Variable -Name "${component}_${compiler}_CmakeCheck" -valueOnly
    
    if ($pipeSeparatedLines.Trim() -ne "")
    {
        [string[]] $lines = $pipeSeparatedLines.Split("`|")
        [string] $cmakeCache = Get-Content (Join-Path $buildDir "CMakeCache.txt")
        [string] $line = $null

        Foreach ($line in $lines)
        {
            if (!$cmakeCache.Contains($line))
            {
                throw "CMakeCache doesn't contain: $line"
            }
        }
    }
}

function Perform-Make([string] $compiler, [string] $buildDir, [bool] $debug)
{
    if ($compiler -eq "gcc")
    {
        if ((Execute-ProcessToHost $buildDir "$mingwMakePath" "install") -ne 0)
        {
            throw "Make failed"
        }
    }
    elseif ($compiler -eq "msvc10")
    {
        if ((Execute-ProcessToHost $buildDir "msbuild" "INSTALL.vcxproj" (Get-MsvcBuildType $debug) ) -ne 0)
        {
            throw "Make failed"
        }
    }
}

function Build-AdmFFmpeg([string] $sourceDir, [string] $buildDir, [string] $installDir, [string] $arch, [bool] $debug)
{
    Setup-Msvc10Environment $arch $debug    # to generate libs
    Setup-MsysEnvironment $arch $debug
    
    PrepareBuildDirectories $sourceDir $buildDir $installDir
    
    if ((Execute-ProcessToHost `
        $buildDir $cmakePath "-G$cmakeGenerator" "-DCMAKE_INSTALL_PREFIX=$installDir" (Get-CmakeBuildType $debug) "$sourceDir") -ne 0)
    {
        throw "Configure failed"
    }

    if ((Execute-ProcessToHost "$buildDir" (Join-Path $msysDir "bin\sh.exe") "-c" "make.exe install") -ne 0)
    #if ((Execute-ProcessToHost $buildDir (Join-Path $msysDir "bin\make.exe") install) -ne 0)
    {
        throw "Make failed"
    }
}

function Build-AdmCore(`
    [string] $sourceDir, [string] $buildDir, [string] $installDir, [string] $ffmpegInstallDir, `
    [string] $compiler, [string] $arch, [bool] $debug)
{
    PrepareBuildDirectories $sourceDir $buildDir $installDir
    SetupEnvironment $compiler $arch $debug

    if ((Execute-ProcessToHost `
        $buildDir $cmakePath "-G$cmakeGenerator" "-DCMAKE_INSTALL_PREFIX=$installDir" `
        "-DADM_FFMPEG_INCLUDE_DIR:FILEPATH=$ffmpegInstallDir\include\avidemux\$admVersion\ffmpeg" `
        "-DADM_FFMPEG_LIB_DIR:FILEPATH=$ffmpegInstallDir" (Get-CmakeBuildType $debug) "$sourceDir") -ne 0)
    {
        throw "Configure failed"
    }
    
    CheckCmakeCache "admCore" "$compiler" "$buildDir"
    Perform-Make $compiler $buildDir $debug
}

function Build-AdmUI( `
    [string] $sourceDir, [string] $buildDir, [string] $installDir, [string] $ffmpegInstallDir, `
    [string] $coreInstallDir, [string] $compiler, [string] $arch, [bool] $debug)
{
    PrepareBuildDirectories $sourceDir $buildDir $installDir
    SetupEnvironment $compiler $arch $debug

    if ((Execute-ProcessToHost `
        $buildDir $cmakePath "-G$cmakeGenerator" "-DCMAKE_INSTALL_PREFIX=$installDir" `
        "-DADM_FFMPEG_INCLUDE_DIR:FILEPATH=$ffmpegInstallDir\include\avidemux\$admVersion\ffmpeg" `
        "-DADM_FFMPEG_LIB_DIR:FILEPATH=$ffmpegInstallDir" `
        "-DADM_CORE_INCLUDE_DIR:FILEPATH=$coreInstallDir\include\avidemux\$admVersion" `
        "-DADM_CORE_LIB_DIR:FILEPATH=$coreInstallDir" (Get-CmakeBuildType $debug) "$sourceDir") -ne 0)
    {
        throw "Configure failed"
    }
    
    if ($sourceDir.EndsWith("\cli"))
    {
        CheckCmakeCache "admCli" "$compiler" "$buildDir"
    }
    elseif ($sourceDir.EndsWith("\qt4"))
    {
        CheckCmakeCache "admQt" "$compiler" "$buildDir"
    }
    
    Perform-Make $compiler $buildDir $debug
}

function Build-AdmPlugins( `
    [string] $sourceDir, [string] $buildDir, [string] $installDir, [string] $ffmpegInstallDir, `
    [string] $coreInstallDir, [string] $qtInstallDir, [string] $cliInstallDir, [string] $compiler, `
    [string] $arch, [bool] $debug)
{
    PrepareBuildDirectories $sourceDir $buildDir $installDir
    SetupEnvironment $compiler $arch $debug

    [string] $avidemuxSourceDir = Resolve-Path (Join-Path $sourceDir "..")
    [string] $spiderMonkeyParameter = ""
    
    if (Get-Variable -Name "spiderMonkey_useSystemVersion_$compiler" -valueOnly)
    {
        $spiderMonkeyParameter = "-DUSE_SYSTEM_SPIDERMONKEY:BOOL=ON"
    }

    if ((Execute-ProcessToHost `
        $buildDir $cmakePath "-G$cmakeGenerator" "-DCMAKE_INSTALL_PREFIX=$installDir" `
        "-DAVIDEMUX_SOURCE_DIR:FILEPATH=$avidemuxSourceDir" "-DPLUGIN_UI=ALL" `
        "-DADM_FFMPEG_INCLUDE_DIR:FILEPATH=$ffmpegInstallDir\include\avidemux\$admVersion\ffmpeg" `
        "-DADM_FFMPEG_LIB_DIR:FILEPATH=$ffmpegInstallDir" `
        "-DADM_CORE_INCLUDE_DIR:FILEPATH=$coreInstallDir\include\avidemux\$admVersion" `
        "-DADM_CORE_LIB_DIR:FILEPATH=$coreInstallDir" `
        "-DADM_CLI_INCLUDE_DIR:FILEPATH=$cliInstallDir\include\avidemux\$admVersion\cli" `
        "-DADM_CLI_LIB_DIR:FILEPATH=$cliInstallDir" `
        "-DADM_QT_INCLUDE_DIR:FILEPATH=$qtInstallDir\include\avidemux\$admVersion\qt4" `
        "-DADM_QT_LIB_DIR:FILEPATH=$qtInstallDir" "$spiderMonkeyParameter" (Get-CmakeBuildType $debug) "$sourceDir") -ne 0)
    {
        throw "Configure failed"
    }

    CheckCmakeCache "admPlugin" "$compiler" "$buildDir"
    Perform-Make $compiler $buildDir $debug
}

function Install-AdmComponent([string] $componentInstallDir, [string] $mainInstallDir)
{
    if (!(Test-Path $mainInstallDir))
    {
        $null = New-Item $mainInstallDir -Type Directory
    }

    Get-ChildItem $componentInstallDir -Recurse -Exclude @('*.dll.a', '*.lib') | Where {$_ -notmatch 'include'} | Copy-Item -Destination {Join-Path $mainInstallDir $_.FullName.Substring($componentInstallDir.length)} -Force
}

function Install-AdmSdk([string] $componentInstallDir, [string] $sdkInstallDir)
{
    if (!(Test-Path $sdkInstallDir))
    {
        $null = New-Item $sdkInstallDir -Type Directory
    }

    Copy-Item -Path "$componentInstallDir\*" -Destination "$sdkInstallDir" -Force -Recurse -Include @('include', '*.dll.a', '*.lib')
}


function Install-AdmDependencies([string] $sourceDir, [string] $mainInstallDir, [string] $compiler, [string] $arch, [bool] $debug)
{
    [string] $mingwLibDir = Get-MingwLibPath $arch
    [string] $externalBinDir = Join-Path (Get-ExternalLibPrefix $arch) "bin"
    [string] $externalEtcDir = Join-Path (Get-ExternalLibPrefix $arch) "etc"
    [string] $qtBinDir = Join-Path (Get-QtDirectory $compiler $arch $debug) "bin"

    Copy-Item -Path (Join-Path $sourceDir "AUTHORS") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $sourceDir "COPYING") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $sourceDir "README") -Destination $mainInstallDir -Force

    if ($compiler -eq "gcc")
    {
        Copy-Item -Path (Join-Path $mingwLibDir "libgcc_s_sjlj-1.dll") -Destination $mainInstallDir -Force
        Copy-Item -Path (Join-Path $mingwLibDir "libstdc++-6.dll") -Destination $mainInstallDir -Force
    }
    
    Copy-Item -Path (Join-Path $mingwLibDir "libwinpthread-1.dll") -Destination $mainInstallDir -Force

    Copy-Item -Path (Join-Path $externalBinDir "aften.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalBinDir "libexpat-1.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalBinDir "libfaac.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalBinDir "libfaad2.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalBinDir "libfontconfig-1.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalBinDir "libfreetype-6.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalBinDir "libfribidi-0.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalBinDir "libmp3lame-0.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalBinDir "libogg-0.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalBinDir "libopencore-amrnb-0.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalBinDir "libopencore-amrwb-0.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalBinDir "libsqlite3-0.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalBinDir "libtwolame-0.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalBinDir "libvorbis-0.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalBinDir "libvorbisenc-2.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalBinDir "libx264-*.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalBinDir "nspr4.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalBinDir "xvidcore.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalBinDir "zlib1.dll") -Destination $mainInstallDir -Force
    
    Copy-Item -Path (Join-Path $externalEtcDir "fonts") -Destination (Join-Path $mainInstallDir "etc\fonts") -Recurse -Force
    
    [string] $qtFileTag = ""

    if ($debug)
    {
        $qtFileTag = "d"
    }

    Copy-Item -Path (Join-Path $qtBinDir "QtCore${qtFileTag}4.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $qtBinDir "QtGui${qtFileTag}4.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $qtBinDir "QtOpenGL${qtFileTag}4.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $qtBinDir "QtScript${qtFileTag}4.dll") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $qtBinDir "QtScriptTools${qtFileTag}4.dll") -Destination $mainInstallDir -Force

    Copy-Item -Path (Join-Path $avsProxyDir "avsproxy.exe") -Destination $mainInstallDir -Force
    Copy-Item -Path (Join-Path $externalToolsDir "avsproxy_gui.exe") -Destination $mainInstallDir -Force

    if (($compiler -eq "gcc") -and $spiderMonkey_useSystemVersion_gcc)
    {
        Copy-Item -Path (Join-Path (Get-Variable -Name "spiderMonkey_libDir_gcc_$arch".Replace("-", "_") -valueOnly) "libjs.dll") -Destination $mainInstallDir -Force
    }
}

function Create-Installer(`
    [string] $mainInstallDir, [string] $installScriptDir, [string] $packagePath, [string] $packageName, `
    [string] $svnRevision, [string] $arch)
{
    [int] $buildBits = Get-BitArchitecture $arch

    Set-Content -Path (Join-Path $installScriptDir "revision.nsh") -Value ""

    Write-Host "Generating $arch installer"

    if ((Execute-ProcessToHost `
        "$mainInstallDir" "$nsisPath" "/V2" "/NOCD" "/DINST_QT" "/DBUILD_BITS=$buildBits" "/DREVISION=$svnRevision" `
        "/DNSIDIR=`"$installScriptDir`"" "/DEXEDIR=`"$packagePath`"" "`"$installScriptDir\avidemux.nsi`"") -ne 0)
    {
        throw "Failed creating installer ($arch)"
    }

    [string] $tempDir = Join-Path ([System.IO.Path]::GetTempPath()) ([System.Guid]::NewGuid().ToString())

    $null = New-Item $tempDir -Type Directory

    Write-Host "Generating $arch zip"

    if ((Execute-ProcessToHost "$tempDir" "$sevenZipPath" "x" (Join-Path $packagePath "$packageName.exe")) -ne 0)
    {
        throw "Failed extracting ($arch)"
    }

    Remove-Item -Path (Join-Path $tempDir "`$PLUGINSDIR") -Recurse
    Copy-Item -Path (Join-Path $mainInstallDir "etc\fonts\conf.d") (Join-Path $tempDir "etc\fonts") -Recurse
    Move-Item -Path (Join-Path $tempDir "`$_OUTDIR\conf.avail") (Join-Path $tempDir "etc\fonts")
    Move-Item -Path (Join-Path $tempDir "`$_OUTDIR\search") (Join-Path $tempDir "help\QtScript")
    Move-Item -Path (Join-Path $tempDir "`$_OUTDIR\lib") (Join-Path $tempDir "plugins\autoScripts")
    $null = New-Item (Join-Path $tempDir "plugins\pluginSettings\x264") -ItemType Directory
    Move-Item -Path (Join-Path $tempDir "`$_OUTDIR\1") (Join-Path $tempDir "plugins\pluginSettings\x264")
    Remove-Item -Path (Join-Path $tempDir "`$_OUTDIR") -Recurse

    Create-Zip $tempDir (Join-Path $packagePath "$packageName.zip")

    Remove-Item -Path $tempDir -Recurse
}