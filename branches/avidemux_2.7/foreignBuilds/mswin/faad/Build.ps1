$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

[string] $libraryName = "faad2"

function Spawn-Build([string] $compiler, [string] $arch, [bool] $debug)
{
	[string] $version = "2.7"
	[string] $tarballFile = "faad2-$version.tar.bz2"
	[string] $url = "http://sourceforge.net/projects/faac/files/faad2-src/faad2-$version/$tarballFile/download"
    [bool] $skipConfigure = $true
    [bool] $skipMake = $true

    . "..\Common Build.ps1"

    $sourceDir = Join-Path $sourceDir "libfaad"

    Copy-Item ".\makefile" "$sourceDir"

    if ((Execute-ProcessToHost "$sourceDir" "make") -ne 0)
	{
		throw "Error building ($arch)"
	}

    Copy-Item (Join-Path $sourceDir "libfaad2.dll") (Join-Path $externalLibDir "bin")
    Copy-Item (Join-Path $sourceDir "libfaad.dll.a") (Join-Path $externalLibDir "lib")
    Copy-Item (Join-Path $sourceDir "..\include\*.h") (Join-Path $externalLibDir "include")

    Create-MsvcLib (Join-Path $externalLibDir "bin\libfaad2.dll") (Join-Path $externalLibDir "lib") $arch "faad"
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