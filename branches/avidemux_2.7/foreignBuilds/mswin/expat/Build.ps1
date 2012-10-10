$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

[string] $libraryName = "expat"

function Spawn-Build([string] $compiler, [string] $arch, [bool] $debug)
{
	[string] $version = "2.1.0"
	[string] $tarballFile = "expat-$version.tar.gz"
	[string] $url = "http://sourceforge.net/projects/expat/files/expat/$version/$tarballFile/download"
    [string] $makeParams = "install"

    $env:CFLAGS = "$env:CFLAGS -O3".Trim()

    . "..\Common Build.ps1"

    Strip-File (Join-Path $externalLibDir "bin\libexpat-1.dll")
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