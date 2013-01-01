$ErrorActionPreference = "Stop"

. "..\Common Functions.ps1"

[string] $libraryName = "avsproxy"

function Spawn-Build([string] $compiler, [string] $arch, [bool] $debug)
{
    [string] $sourceDir = Join-Path $devDir "$libraryName-$admVersion-$compiler-$arch"

    Setup-Msvc11Environment $arch $debug
    Create-FreshDirectory $sourceDir
    Copy-Item "..\..\..\addons\avisynthproxy\*" "$sourceDir"

    if ((Execute-ProcessToHost "$sourceDir" "vcupgrade.exe" "-overwrite" "-nologo" "avsproxy.vcproj") -ne 0)
	{
		throw "Error upgrading project ($arch)"
	}

    if ((Execute-ProcessToHost "$sourceDir" "msbuild.exe" "avsproxy.vcxproj" "/p:Configuration=Release" "/t:rebuild") -ne 0)
	{
		throw "Error building ($arch)"
	}
}

function Start-UI
{
	. "..\Common UI.ps1"

	$null = $compilerComboBox.Items.Add("MSVC 11")
	$compilerComboBox.SelectedIndex = 0;

    $arch64CheckBox.CheckState = [System.Windows.Forms.CheckState]::Unchecked
    $arch64CheckBox.Visible = $false
	$debugCheckBox.Visible = $false

	$form.Text = "Build $libraryName"
	$form.ShowDialog() | Out-Null
}

function Build
{
    [string] $compiler = GetCompilerTag
    [string[]] $arches = GetArchitectureTags
    [bool] $debug = ($debugCheckBox.CheckState -eq $checked)
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