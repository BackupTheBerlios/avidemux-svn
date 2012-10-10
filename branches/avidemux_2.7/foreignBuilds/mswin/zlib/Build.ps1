$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

[string] $libraryName = "zlib"

function Spawn-Build([string] $compiler, [string] $arch, [bool] $debug)
{
	[string] $version = "1.2.7"
	[string] $tarballFile = "zlib-$version.tar.gz"
	[string] $url = "http://zlib.net/$tarballFile"
    [bool] $skipConfigure = $true
    [bool] $skipMake = $true
    [string] $rc = $null

    . "..\Common Build.ps1"

    if ($arch -eq "x86")
    {
        $rc = "windres -F pe-i386"
    }
    elseif ($arch -eq "x86-64")
    {
        $rc = "windres -F pe-x86-64"
    }

    if ((Execute-ProcessToHost `
        "$sourceDir" $mingwMakePath "-f" "win32/Makefile.gcc" "CFLAGS=$env:CFLAGS" "LDFLAGS=$env:LDFLAGS" "RC=$rc") -ne 0)
    {
	    throw "Error building ($arch)"
    }

    Copy-Item (Join-Path $sourceDir "zlib1.dll") (Join-Path "$externalLibDir" "bin") -Force
    Copy-Item (Join-Path $sourceDir "zconf.h") (Join-Path $externalLibDir "include") -Force
    Copy-Item (Join-Path $sourceDir "zlib.h") (Join-Path $externalLibDir "include") -Force
    Copy-Item (Join-Path $sourceDir "libz.dll.a") (Join-Path $externalLibDir "lib") -Force

    Create-MsvcLib (Join-Path $sourceDir "zlib1.dll") (Join-Path $externalLibDir "lib") $arch "zdll"
}

function Start-UI
{
	. "..\Common UI.ps1"

	$null = $compilerComboBox.Items.Add("GCC")
	$compilerComboBox.SelectedIndex = 0;

	$debugCheckBox.Visible = $false

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