$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

[string] $libraryName = "sqlite"

function Spawn-Build([string] $compiler, [string] $arch, [bool] $debug)
{
	[string] $version = "3.7.14"
	[string] $tarballFile = "sqlite-autoconf-3071400.tar.gz"
	[string] $url = "http://www.sqlite.org/$tarballFile"
    [string] $makeParams = "install"

    . "..\Common Build.ps1"

    [string] $dllPath = Join-Path $externalLibDir "bin\libsqlite3-0.dll"

    Strip-File $dllPath
    Create-MsvcLib $dllPath (Join-Path $externalLibDir "lib") $arch "sqlite3"
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