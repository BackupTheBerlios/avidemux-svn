$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

[string] $libraryName = "qt"

function Spawn-Build([string] $compiler, [string] $arch, [bool] $debug)
{
	[string] $version = "4.8.3"
	[string] $tarballFile = "qt-everywhere-opensource-src-$version.tar.gz"
	[string] $url = "http://releases.qt-project.org/qt4/source/$tarballFile"
    [bool] $skipConfigure = $true
    [bool] $skipMake = $true
    [string] $buildType = $null
    [string] $platform = $null
    [string[]] $patchFiles = @("Platform.h.patch")

    if ($debug)
    {
        $buildType = "debug"
    }
    else
    {
        $buildType = "release"
    }

    if ($compiler -eq "gcc")
    {
        $platform = "win32-g++-4.6"

        if ($arch -eq "x86")
        {
            $patchFiles += "qmake32.conf.patch"
        }
        elseif ($arch -eq "x86-64")
        {
            $patchFiles += "qmake64.conf.patch"
        }
    }
    elseif ($compiler -eq "msvc10")
    {
        $platform = "win32-msvc2010"
    }

    . "..\Common Build.ps1"

    [string] $extraIncludeDir = (Join-Path $externalLibDir "include")
    [string] $extraLibDir = (Join-Path $externalLibDir "lib")

    if ((Execute-ProcessToHost `
        "$sourceDir" (Join-Path $sourceDir "configure.exe") "-confirm-license" "-opensource" "-$buildType" "-system-zlib" `
        "-no-stl" "-no-qt3support" "-no-phonon" "-no-webkit" "-no-multimedia" "-no-declarative" "-no-style-cleanlooks" `
        "-no-style-plastique" "-no-style-motif" "-no-style-cde" "-qt-style-windowsxp" "-qt-style-windowsvista" `
        "-no-xmlpatterns" "-nomake" "demos" "-nomake" "examples" "-platform" "$platform" "-mmx" "-sse" "-sse2" "-3dnow" `
        "-I" "$extraIncludeDir" "-L" "$extraLibDir") -ne 0)
    {
	    throw "Error building ($arch)"
    }

    if ($compiler -eq "gcc")
    {
        if ((Execute-ProcessToHost "$sourceDir" $mingwMakePath) -ne 0)
        {
	        throw "Error building ($arch)"
        }
    }
    elseif ($compiler -eq "msvc10")
    {
        if ((Execute-ProcessToHost "$sourceDir" "nmake.exe") -ne 0)
        {
	        throw "Error building ($arch)"
        }
    }
}

function Start-UI
{
	. "..\Common UI.ps1"

	$null = $compilerComboBox.Items.AddRange(@("GCC", "MSVC 10"))
	$compilerComboBox.SelectedIndex = 0;

	$form.Text = "Build $libraryName"
	$form.ShowDialog() | Out-Null
}

function Build
{
    [string] $compiler = GetCompilerTag
    [string[]] $arches = GetArchitectureTags
    [bool] $debug = ($debugCheckBox.CheckState -eq [System.Windows.Forms.CheckState]::Checked)
    [string] $arch = $null

    Foreach ($arch in $arches)
    {
    	if ((Execute-ProcessToHost "." "powershell" -command "& { . '.\Build.ps1'; Spawn-Build '$compiler' '$arch' `$$debug }") -ne 0)
		{
			throw "Error spawning build ($arch)"
		}
    }

    Write-Host "Finished" -ForegroundColor Green
}