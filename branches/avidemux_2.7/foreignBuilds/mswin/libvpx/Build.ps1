$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

[string] $libraryName = "libvpx"

function Spawn-Build([string] $compiler, [string] $arch, [bool] $debug)
{
	[string] $version = "1.1.0"
	[string] $tarballFile = "libvpx-v$version.tar.bz2"
	[string] $url = "http://webm.googlecode.com/files/$tarballFile"
    [string] $configureParams = "`"--disable-vp8-encoder`" `"--disable-examples`""
    [string] $makeParams = "install"

    if ($arch -eq "x86")
    {
        $configureParams += " --target=x86-win32-gcc"
    }
    elseif ($arch -eq "x86-64")
    {
        $configureParams += " `"--target=x86_64-win64-gcc`""
    }

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