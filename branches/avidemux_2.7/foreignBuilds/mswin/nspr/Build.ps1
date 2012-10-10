$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

[string] $libraryName = "nspr"

function Spawn-Build([string] $compiler, [string] $arch, [bool] $debug)
{
	[string] $version = "4.9.2"
	[string] $tarballFile = "nspr-$version.tar.gz"
	[string] $url = "ftp://ftp.mozilla.org/pub/mozilla.org/nspr/releases/v$version/src/$tarballFile"
    [bool] $skipConfigure = $true
    [bool] $skipMake = $true

    . "..\Common Build.ps1"

    [string] $sourceDir = Join-Path "$sourceDir" "mozilla\nsprpub"

    if ((Execute-ProcessToHost `
        "$sourceDir" (Join-Path $msysDir "bin\sh.exe") "./configure" "--prefix=`"$externalLibDir`"" `
        "--enable-strip" "--enable-win32-target=WIN95" "--enable-optimize=-O3" "--disable-debug") -ne 0)
	{
		throw "Error configuring ($arch)"
	}

    [string] $rc = $null

    if ($arch -eq "x86")
    {
        $rc = "windres -F pe-i386"
    }
    elseif ($arch -eq "x86-64")
    {
        $rc = "windres -F pe-x86-64"
    }

    if ((Execute-ProcessToHost "$sourceDir" (Join-Path $msysDir "bin\sh.exe") "-c" "make.exe RC='$rc'") -ne 0)
    {
	    throw "Error building ($arch)"
    }

    if ((Execute-ProcessToHost "$sourceDir" (Join-Path $msysDir "bin\sh.exe") "-c" "make.exe install") -ne 0)
    {
	    throw "Error building ($arch)"
    }

    Move-Item (Join-Path $externalLibDir "lib\nspr4.dll") (Join-Path $externalLibDir "bin") -Force
    Move-Item (Join-Path $externalLibDir "lib\nspr4.a") (Join-Path $externalLibDir "lib\nspr4.dll.a") -Force
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