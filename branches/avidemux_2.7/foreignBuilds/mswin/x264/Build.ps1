$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

[string] $libraryName = "x264"

function Spawn-Build([string] $compiler, [string] $arch, [bool] $debug)
{
	[string] $version = "git"
	[string] $gitUrl = "git://git.videolan.org/x264.git"  # http://repo.or.cz/r/x264.git
    [string] $configureParams = "`"--enable-shared`" `"--enable-win32thread`""
    [string] $makeParams = "install"
    [string] $externalLibDir = Get-ExternalLibPrefix $arch

    $env:PATH = "$env:PATH;" + (Split-Path $gitPath)

    if ($arch -eq "x86")
    {
        $env:CFLAGS = "-mfpmath=387 $env:CFLAGS"
    }
    elseif ($arch -eq "x86-64")
    {
        $configureParams += " `"--host=x86_64-pc-mingw32`""
    }

    Remove-Item (Join-Path "$externalLibDir" "bin\libx264-*.dll")

    if (Test-Path (Join-Path "$externalLibDir" "include\x264.h"))
    {
        Remove-Item (Join-Path "$externalLibDir" "include\x264.h")
    }

    . "..\Common Build.ps1"

    Create-MsvcLib (Get-Item -Path (Join-Path $externalLibDir "bin\libx264-*.dll")) (Join-Path $externalLibDir "lib") $arch "x264"
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