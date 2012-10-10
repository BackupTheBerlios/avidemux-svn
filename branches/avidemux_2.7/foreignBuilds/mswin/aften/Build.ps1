$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

[string] $libraryName = "aften"

function Spawn-Build([string] $compiler, [string] $arch, [bool] $debug)
{
	[string] $version = "git"
	[string] $gitUrl = "git://aften.git.sourceforge.net/gitroot/aften/aften"
    [string] $gitStartPoint = "89aee3d496bb2a89f046025402626ee12a12969f"
    [bool] $skipConfigure = $true
    [bool] $skipMake = $true

    . "..\Common Build.ps1"

    [string] $buildDir = Join-Path $sourceDir "build"
    New-Item $buildDir -ItemType Directory

    if ((Execute-ProcessToHost "$buildDir" "$cmakePath" -G"NMake Makefiles" "-DCMAKE_INSTALL_PREFIX='$externalLibDir'" "-DSHARED=ON" "-DCMAKE_C_FLAGS_RELEASE='/MT /O2 /Ob2 /D NDEBUG'" "..") -ne 0)
	{
		throw "Error configuring ($arch)"
	}

    if ((Execute-ProcessToHost "$buildDir" "nmake.exe" "install") -ne 0)
	{
		throw "Error building ($arch)"
	}

    if ((Execute-ProcessToHost $buildDir (Join-Path "$externalToolsDir" "gendef.exe") "aften.dll" ) -ne 0)
	{
		throw "Error generating library definition ($arch)"
	}

    [string] $dllToolArgs = $null

    if ($arch -eq "x86")
    {
        $dllToolArgs = "`"-f`" `"--32`" `"-m`" `"i386`""
    }
    elseif ($arch -eq "x86-64")
    {
        $dllToolArgs = "`"-f`" `"--64`" `"-m`" `"i386:x86-64`""
    }

    if ((Execute-ProcessToHost $buildDir (Join-Path "$mingwBinDir" "dlltool.exe") "-d" "aften.def" "-l" (Join-Path "$externalLibDir" "lib\aften.dll.a") "$dllToolArgs") -ne 0)
	{
		throw "Error generating import library ($arch)"
	}
}

function Start-UI
{
	. "..\Common UI.ps1"

	$null = $compilerComboBox.Items.Add("MSVC 10")
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