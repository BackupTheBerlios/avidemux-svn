$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

[string] $libraryName = "twolame"

function Spawn-Build([string] $compiler, [string] $arch, [bool] $debug)
{
	[string] $version = "0.3.13"
	[string] $tarballFile = "twolame-$version.tar.gz"
	[string] $url = "http://sourceforge.net/projects/twolame/files/twolame/$version/$tarballFile/download"

    Setup-MsysEnvironment $arch $debug

    [string] $makeParams = "CFLAGS='$env:CFLAGS -O3 -DLIBTWOLAME_DLL_EXPORTS' LDFLAGS='$env:LDFLAGS -no-undefined' install-strip"

    . "..\Common Build.ps1"
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