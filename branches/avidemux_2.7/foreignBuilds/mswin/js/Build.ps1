$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

[string] $libraryName = "js"

function Spawn-Build([string] $compiler, [string] $arch, [bool] $debug)
{
	[string] $version = "1.7.0"
	[string] $tarballFile = "js-$version.tar.gz"
    [string] $tarOutputSubDir = "js"
	[string] $url = "http://ftp.mozilla.org/pub/mozilla.org/js/$tarballFile"
    [string[]] $patchFiles = @("Makefile.ref.patch", "jsnum.c.patch")
    [bool] $skipConfigure = $true
    [bool] $skipMake = $true
    [string] $rcType = $null

    . "..\Common Build.ps1"

    Copy-Item ".\WINNT6.1.mk" (Join-Path $sourceDir "src\config")

    if ($arch -eq "x86")
    {
        $rcType = "pe-i386"
    }
    elseif ($arch -eq "x86-64")
    {
        $rcType = "pe-x86-64"
    }

    if ((Execute-ProcessToHost `
        (Join-Path $sourceDir "src") (Join-Path $mingwBinDir "windres") "-i" "js3240.rc" "-o" "jsres.o" "-O" "coff" "-F" "$rcType") -ne 0)
	{
		throw "Error executing windres ($arch)"
	}

    $env:CFLAGS = "$env:CFLAGS -I`"$externalLibDir/include/nspr`""

    if ((Execute-ProcessToHost `
        (Join-Path $sourceDir "src") (Join-Path $msysDir "bin\sh.exe") "-c" `
        "make.exe -f Makefile.ref JS_DIST=`"$externalLibDir`" BUILD_OPT=1 JS_HAS_FILE_OBJECT=1 XLDFLAGS='$env:LDFLAGS jsres.o -L$externalLibDir/lib -lnspr4'") -ne 0)
	{
		throw "Error building ($arch)"
	}

	[string] $dllPath = Join-Path $sourceDir "src\WINNT6.1_OPT.OBJ\libjs.dll"

    Strip-File $dllPath
	Create-MsvcLib $dllPath (Join-Path $sourceDir "src\WINNT6.1_OPT.OBJ") $arch "js"
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