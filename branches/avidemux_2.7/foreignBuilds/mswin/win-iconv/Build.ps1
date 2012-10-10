$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

[string] $libraryName = "win-iconv"

function Spawn-Build([string] $compiler, [string] $arch, [bool] $debug)
{
	[string] $version = "20100912"
	[string] $zipFile = "win-iconv-dev_tml-${version}_win64.zip"
	[string] $url = "http://ftp.gnome.org/pub/gnome/binaries/win64/dependencies/$zipFile"
    [string] $sourceDir = Join-Path $devDir "$libraryName-$version-$compiler-$arch"
    [string] $externalLibDir = Get-ExternalLibPrefix $arch

    Setup-MingwEnvironment $arch $debug
    Download-File $url $zipFile $true
    Extract-Zip $zipFile $sourceDir

    if ((Execute-ProcessToHost `
        "$sourceDir" (Join-Path $mingwBinDir "gcc.exe") "$env:CFLAGS" "-O3" "-o" "win_iconv.win64.o" "-c" "src\tml\win_iconv\win_iconv.c") -ne 0)
    {
	    throw "Error building ($arch)"
    }

    if ((Execute-ProcessToHost `
        "$sourceDir" (Join-Path $mingwBinDir "ar.exe") "crv" (Join-Path $externalLibDir "lib\libiconv.a") "win_iconv.win64.o") -ne 0)
    {
	    throw "Error generating library ($arch)"
    }

    Copy-Item (Join-Path $sourceDir "src\tml\win_iconv\iconv.h") (Join-Path "$externalLibDir" "include") -Force
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