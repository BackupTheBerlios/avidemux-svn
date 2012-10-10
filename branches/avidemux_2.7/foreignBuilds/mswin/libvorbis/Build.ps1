$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

[string] $libraryName = "libvorbis"

function Spawn-Build([string] $compiler, [string] $arch, [bool] $debug)
{
	[string] $version = "1.3.3"
	[string] $tarballFile = "libvorbis-$version.tar.gz"
	[string] $url = "http://downloads.xiph.org/releases/vorbis/$tarballFile"
    [string] $configureParams = "`"--disable-static`" `"--disable-examples`" `"--disable-docs`""

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