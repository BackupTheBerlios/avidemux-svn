$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

[string] $libraryName = "faac"

function Spawn-Build([string] $compiler, [string] $arch, [bool] $debug)
{
	[string] $version = "1.28"
	[string] $tarballFile = "faac-$version.tar.bz2"
	[string] $url = "http://sourceforge.net/projects/faac/files/faac-src/faac-$version/$tarballFile/download"
    [bool] $skipConfigure = $true
    [bool] $skipMake = $true

    . "..\Common Build.ps1"

    $sourceDir = Join-Path $sourceDir "libfaac"

    Copy-Item ".\makefile" "$sourceDir"

    if ((Execute-ProcessToHost "$sourceDir" "make") -ne 0)
	{
		throw "Error building ($arch)"
	}

    Copy-Item (Join-Path $sourceDir "libfaac.dll") (Join-Path $externalLibDir "bin")
    Copy-Item (Join-Path $sourceDir "libfaac.dll.a") (Join-Path $externalLibDir "lib")
    Copy-Item (Join-Path $sourceDir "..\include\*.h") (Join-Path $externalLibDir "include")

    Create-MsvcLib (Join-Path $externalLibDir "bin\libfaac.dll") (Join-Path $externalLibDir "lib") $arch "faac"
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