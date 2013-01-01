$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

[string] $libraryName = "winpthread"

function Spawn-Build([string] $compiler, [string] $arch, [bool] $debug)
{
    [bool] $skipConfigure = $true
    [bool] $skipMake = $true

    . "..\Common Build.ps1"

    Create-MsvcLib (Join-Path $mingwBinDir "libwinpthread-1.dll") (Join-Path (Get-ExternalLibPrefix $arch) "lib") $arch "pthread"

    [string] $mingwLibDir = $null

    if ($arch -eq "x86-64")
    {
        $mingwLibDir = "$mingwDir\i686-w64-mingw32\lib64"
    }
    elseif ($arch -eq "x86")
    {
        $mingwLibDir = "$mingwDir\i686-w64-mingw32\lib"
    }

    Copy-Item -Path "$mingwLibDir\libpthread.dll.a" -Destination (Join-Path (Get-ExternalLibPrefix $arch) "lib") -Force
    Copy-Item -Path "$mingwIncludeDir\pthread*.h" -Destination (Join-Path (Get-ExternalLibPrefix $arch) "include") -Force
}

function Start-UI
{
	. "..\Common UI.ps1"

	$null = $compilerComboBox.Items.Add("MSVC 11")
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