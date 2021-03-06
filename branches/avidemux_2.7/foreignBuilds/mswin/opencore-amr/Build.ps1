$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

[string] $libraryName = "opencore-amr"

function Spawn-Build([string] $compiler, [string] $arch, [bool] $debug)
{
	[string] $version = "0.1.3"
	[string] $tarballFile = "opencore-amr-$version.tar.gz"
	[string] $url = "http://sourceforge.net/projects/opencore-amr/files/opencore-amr/$tarballFile/download"

    . "..\Common Build.ps1"

    Create-MsvcLib (Join-Path $externalLibDir "bin\libopencore-amrnb-0.dll") (Join-Path $externalLibDir "lib") $arch "opencore-amrnb"
    Create-MsvcLib (Join-Path $externalLibDir "bin\libopencore-amrwb-0.dll") (Join-Path $externalLibDir "lib") $arch "opencore-amrwb"
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