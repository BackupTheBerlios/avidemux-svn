$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

[string] $libraryName = "xvid"

function Spawn-Build([string] $compiler, [string] $arch, [bool] $debug)
{
	[string] $version = "1.3.2"
	[string] $tarballFile = "xvidcore-$version.tar.gz"
    [string] $tarOutputSubDir = "xvidcore"
	[string] $url = "http://downloads.xvid.org/downloads/$tarballFile"
    [string[]] $patchFiles = @("configure.patch", "nasm.inc.patch")
    [bool] $skipConfigure = $true
    [bool] $skipMake = $true
    [string] $buildType = $null

    . "..\Common Build.ps1"

    $sourceDir = Join-Path $sourceDir "build\generic"
    $env:LDFLAGS = ($env:LDFLAGS + " -Wl,-s").Trim()

    if ($arch -eq "x86")
    {
        $buildType = "i686-pc-mingw32"
    }
    elseif ($arch -eq "x86-64")
    {
        $buildType = "x86_64-pc-mingw32"
    }

    if ((Execute-ProcessToHost `
        $sourceDir (Join-Path $msysDir "bin\sh.exe") "./configure" "--prefix=`"$externalLibDir`"" "--build=$buildType") -ne 0)
	{
		throw "Error configuring ($arch)"
	}

    if ((Execute-ProcessToHost "$sourceDir" (Join-Path $msysDir "bin\sh.exe") "-c" "make.exe") -ne 0)
    {
	    throw "Error building ($arch)"
    }

    if ((Execute-ProcessToHost "$sourceDir" (Join-Path $msysDir "bin\sh.exe") "-c" "make.exe install") -ne 0)
    {
	    throw "Error installing ($arch)"
    }

    Move-Item (Join-Path $externalLibDir "lib\xvidcore.dll") (Join-Path $externalLibDir "bin") -Force
    Copy-Item (Join-Path $sourceDir "=build\xvidcore.dll.a") (Join-Path $externalLibDir "lib") -Force

    #Strip-File (Join-Path $externalLibDir "bin\libfribidi-0.dll")
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