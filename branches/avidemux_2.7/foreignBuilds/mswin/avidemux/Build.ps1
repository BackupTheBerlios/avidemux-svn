. "..\Common Functions.ps1"

# == Functions ==
function Get-AdmInstallDir([string] $compiler, [string] $arch, [bool] $debug)
{
    Get-InstallDir $admInstallDirPattern $compiler $arch $debug
}

function Get-AdmSdkInstallDir([string] $compiler, [string] $arch, [bool] $debug)
{
    Get-InstallDir $admSdkInstallDirPattern $compiler $arch $debug
}

function Get-InstallDir([string] $pattern, [string] $compiler, [string] $arch, [bool] $debug)
{
    [string] $installDir = $pattern.Replace("`${compiler}", "$compiler").Replace("`${arch}", "$arch")

    if ($debug)
    {
        $installDir = $installDir.Replace("`${debug}", "-dbg")
    }
    else
    {
        $installDir = $installDir.Replace("`${debug}", "")
    }

    Force-Resolve-Path $installDir
}

function Get-BuildFolderName([string] $arch, [bool] $debug)
{
    [string] $folderName = "build-$arch"
    
    if ($debug)
    {
        $folderName += "-dbg"
    }
    
    $folderName
}

function Get-PackageFileName([string] $pattern, [string] $arch, [string] $revision)
{
    [int] $archBits = Get-BitArchitecture $arch

    $pattern.Replace("`${arch}", "$arch").Replace("`${revision}", "$revision").Replace("`${bits}", "$archBits")
}

function Get-ComponentPath([string] $component, [string] $rootDir, [string] $compiler, [bool] $debug)
{
    if ($debug)
    {
        Join-Path $rootDir "$compiler-install"
    }
    else
    {
        Join-Path $rootDir "$compiler-$component-install"
    }
}

function Build
{
	# == Perform Build ==
	[string] $compiler = GetCompilerTag
	[string[]] $arches = GetArchitectureTags
	[System.Windows.Forms.CheckState] $checked = [System.Windows.Forms.CheckState]::Checked
	[bool] $debug = ($debugCheckBox.CheckState -eq $checked)
	[string] $rootSourceDir = Resolve-Path "..\..\.."

	if (!(Test-Path (Join-Path $rootSourceDir "README.")))
	{
		throw "Source directory couldn't be determined ($rootSourceDir)"
	}

	# == Prepare Build Directories ==
	[string] $arch = $null
	[string[]] $rootBuildDirs = @()

	Foreach ($arch in $arches)
	{
		[string] $rootBuildDir = Join-Path $rootSourceDir (Get-BuildFolderName $arch $debug)

		if (!(Test-Path $rootBuildDir))
		{
			$null = New-Item $rootBuildDir -Type Directory
		}

		$rootBuildDirs += $rootBuildDir
    
		if ($createInstallCheckBox.CheckState -eq $checked -and !$debug)
		{
			[string] $mainInstallDir = Get-AdmInstallDir $compiler $arch $debug
        
			Create-FreshDirectory($mainInstallDir)
		}
    
		if ($createSdkCheckBox.CheckState -eq $checked -and !$debug)
		{
			[string] $sdkInstallDir = Get-AdmSdkInstallDir $compiler $arch $debug
        
			Create-FreshDirectory($sdkInstallDir)
		}
	}

	# == Build & Install FFmpeg ==
	[int] $archIndex = 0
	[bool] $success = $true

	[string] $ffmpegSourceDir = Join-Path $rootSourceDir "avidemux_ffmpeg"

	For ($archIndex = 0; $archIndex -lt $arches.length; $archIndex++)
	{
		$arch = $arches[$archIndex]

		[string] $rootBuildDir = $rootBuildDirs[$archIndex]
		[string] $ffmpegBuildDir = Join-Path $rootBuildDir "$compiler-ffmpeg"
		[string] $ffmpegInstallDir = Get-ComponentPath "ffmpeg" $rootBuildDir $compiler $debug
		[string] $mainInstallDir = Get-AdmInstallDir $compiler $arch $debug
		[string] $sdkInstallDir = Get-AdmSdkInstallDir $compiler $arch $debug

		if (($ffmpegCheckBox.CheckState -eq $checked) -and $success)
		{
			if ((Execute-ProcessToHost `
				"." "powershell" -command "& { . '.\Build Components.ps1'; Build-AdmFFmpeg '$ffmpegSourceDir' '$ffmpegBuildDir' '$ffmpegInstallDir' '$arch' `$$debug }") -ne 0)
			{
				Write-Host "Failed building FFmpeg ($arch)" -foregroundcolor Red
				$success = $false
				break
			}
		}

		if ($success -and !$debug -and `
			(($createInstallCheckBox.CheckState -eq $checked -and (Execute-ProcessToHost "." "powershell" -command "& { . '.\Build Components.ps1'; Install-AdmComponent '$ffmpegInstallDir' '$mainInstallDir' }") -ne 0) -or `
			($createSdkCheckBox.CheckState -eq $checked -and (Execute-ProcessToHost "." "powershell" -command "& { . '.\Build Components.ps1'; Install-AdmSdk '$ffmpegInstallDir' '$sdkInstallDir' 'ffmpeg' }") -ne 0)))
		{
			Write-Host "Failed installing FFmpeg ($arch)" -foregroundcolor Red
			$success = $false
			break
		}
	}

	if ($success)
	{
		$ffmpegCheckBox.CheckState = [System.Windows.Forms.CheckState]::Unchecked
		[System.Windows.Forms.Application]::DoEvents()
	}

	# == Build & Install Core ==
	[string] $coreSourceDir = Join-Path $rootSourceDir "avidemux_core"

	For ($archIndex = 0; $archIndex -lt $arches.length; $archIndex++)
	{
		$arch = $arches[$archIndex]

		[string] $rootBuildDir = $rootBuildDirs[$archIndex]
		[string] $ffmpegInstallDir = Get-ComponentPath "ffmpeg" $rootBuildDir $compiler $debug
		[string] $coreBuildDir = Join-Path $rootBuildDir "$compiler-core"
		[string] $coreInstallDir = Get-ComponentPath "core" $rootBuildDir $compiler $debug
		[string] $mainInstallDir = Get-AdmInstallDir $compiler $arch $debug
		[string] $sdkInstallDir = Get-AdmSdkInstallDir $compiler $arch $debug

		if (($coreCheckBox.CheckState -eq $checked) -and $success)
		{
			if ((Execute-ProcessToHost `
				"." "powershell" -command "& { . '.\Build Components.ps1'; Build-AdmCore '$coreSourceDir' '$coreBuildDir' '$coreInstallDir' '$ffmpegInstallDir' '$compiler' '$arch' `$$debug }") -ne 0)
			{
				Write-Host "Failed building Core ($arch)" -foregroundcolor Red
				$success = $false
				break
			}
		}
    
		if ($success -and !$debug -and `
			(($createInstallCheckBox.CheckState -eq $checked -and (Execute-ProcessToHost "." "powershell" -command "& { . '.\Build Components.ps1'; Install-AdmComponent '$coreInstallDir' '$mainInstallDir' }") -ne 0) -or `
			($createSdkCheckBox.CheckState -eq $checked -and (Execute-ProcessToHost "." "powershell" -command "& { . '.\Build Components.ps1'; Install-AdmSdk '$coreInstallDir' '$sdkInstallDir' 'core' }") -ne 0)))
		{
			Write-Host "Failed installing Core ($arch)" -foregroundcolor Red
			$success = $false
			break
		}
	}

	if ($success)
	{
		$coreCheckBox.CheckState = [System.Windows.Forms.CheckState]::Unchecked
		[System.Windows.Forms.Application]::DoEvents()
	}

	# == Build & Install Qt ==
	[string] $qtSourceDir = Join-Path $rootSourceDir "avidemux\qt4"

	For ($archIndex = 0; $archIndex -lt $arches.length; $archIndex++)
	{
		$arch = $arches[$archIndex]

		[string] $rootBuildDir = $rootBuildDirs[$archIndex]
		[string] $ffmpegInstallDir = Get-ComponentPath "ffmpeg" $rootBuildDir $compiler $debug
		[string] $coreInstallDir = Get-ComponentPath "core" $rootBuildDir $compiler $debug
		[string] $qtBuildDir = Join-Path $rootBuildDir "$compiler-qt"
		[string] $qtInstallDir = Get-ComponentPath "qt" $rootBuildDir $compiler $debug
		[string] $mainInstallDir = Get-AdmInstallDir $compiler $arch $debug
		[string] $sdkInstallDir = Get-AdmSdkInstallDir $compiler $arch $debug

		if (($qtCheckBox.CheckState -eq $checked) -and $success)
		{
			if ((Execute-ProcessToHost `
				"." "powershell" -command "& { . '.\Build Components.ps1'; Build-AdmUI '$qtSourceDir' '$qtBuildDir' '$qtInstallDir' '$ffmpegInstallDir' '$coreInstallDir' '$compiler' '$arch' `$$debug }") -ne 0)
			{
				Write-Host "Failed building Qt UI ($arch)" -foregroundcolor Red
				$success = $false
				break
			}
		}
    
		if ($success -and !$debug -and `
			(($createInstallCheckBox.CheckState -eq $checked -and (Execute-ProcessToHost "." "powershell" -command "& { . '.\Build Components.ps1'; Install-AdmComponent '$qtInstallDir' '$mainInstallDir' }") -ne 0) -or `
			($createSdkCheckBox.CheckState -eq $checked -and (Execute-ProcessToHost "." "powershell" -command "& { . '.\Build Components.ps1'; Install-AdmSdk '$qtInstallDir' '$sdkInstallDir' 'qt' }") -ne 0)))
		{
			Write-Host "Failed installing Qt UI ($arch)" -foregroundcolor Red
			$success = $false
			break
		}
	}

	if ($success)
	{
		$qtCheckBox.CheckState = [System.Windows.Forms.CheckState]::Unchecked
		[System.Windows.Forms.Application]::DoEvents()
	}

	# == Build & Install CLI ==
	[string] $cliSourceDir = Join-Path $rootSourceDir "avidemux\cli"

	For ($archIndex = 0; $archIndex -lt $arches.length; $archIndex++)
	{
		$arch = $arches[$archIndex]

		[string] $rootBuildDir = $rootBuildDirs[$archIndex]
		[string] $ffmpegInstallDir = Get-ComponentPath "ffmpeg" $rootBuildDir $compiler $debug
		[string] $coreInstallDir = Get-ComponentPath "core" $rootBuildDir $compiler $debug
		[string] $cliBuildDir = Join-Path $rootBuildDir "$compiler-cli"
		[string] $cliInstallDir = Get-ComponentPath "cli" $rootBuildDir $compiler $debug
		[string] $mainInstallDir = Get-AdmInstallDir $compiler $arch $debug
		[string] $sdkInstallDir = Get-AdmSdkInstallDir $compiler $arch $debug

		if (($cliCheckBox.CheckState -eq $checked) -and $success)
		{
			if ((Execute-ProcessToHost `
				"." "powershell" -command "& { . '.\Build Components.ps1'; Build-AdmUI '$cliSourceDir' '$cliBuildDir' '$cliInstallDir' '$ffmpegInstallDir' '$coreInstallDir' '$compiler' '$arch' `$$debug }") -ne 0)
			{
				Write-Host "Failed building CLI ($arch)" -foregroundcolor Red
				$success = $false
				break
			}
		}
    
		if ($success -and !$debug -and `
			(($createInstallCheckBox.CheckState -eq $checked -and (Execute-ProcessToHost "." "powershell" -command "& { . '.\Build Components.ps1'; Install-AdmComponent '$cliInstallDir' '$mainInstallDir' }") -ne 0) -or `
			($createSdkCheckBox.CheckState -eq $checked -and (Execute-ProcessToHost "." "powershell" -command "& { . '.\Build Components.ps1'; Install-AdmSdk '$cliInstallDir' '$sdkInstallDir' 'cli' }") -ne 0)))
		{
			Write-Host "Failed installing CLI ($arch)" -foregroundcolor Red
			$success = $false
			break
		}
	}

	if ($success)
	{
		$cliCheckBox.CheckState = [System.Windows.Forms.CheckState]::Unchecked
		[System.Windows.Forms.Application]::DoEvents()
	}

	# == Build & Install Plugins ==
	[string] $pluginSourceDir = Join-Path $rootSourceDir "avidemux_plugins"

	For ($archIndex = 0; $archIndex -lt $arches.length; $archIndex++)
	{
		$arch = $arches[$archIndex]

		[string] $rootBuildDir = $rootBuildDirs[$archIndex]
		[string] $ffmpegInstallDir = Get-ComponentPath "ffmpeg" $rootBuildDir $compiler $debug
		[string] $coreInstallDir = Get-ComponentPath "core" $rootBuildDir $compiler $debug
		[string] $qtInstallDir = Get-ComponentPath "qt" $rootBuildDir $compiler $debug
		[string] $cliInstallDir = Get-ComponentPath "cli" $rootBuildDir $compiler $debug
		[string] $pluginBuildDir = Join-Path $rootBuildDir "$compiler-plugin"
		[string] $pluginInstallDir = Get-ComponentPath "plugin" $rootBuildDir $compiler $debug
		[string] $mainInstallDir = Get-AdmInstallDir $compiler $arch $debug

		if (($pluginCheckBox.CheckState -eq $checked) -and $success)
		{
			if ((Execute-ProcessToHost `
				"." "powershell" -command "& { . '.\Build Components.ps1'; Build-AdmPlugins '$pluginSourceDir' '$pluginBuildDir' '$pluginInstallDir' '$ffmpegInstallDir' '$coreInstallDir' '$qtInstallDir' '$cliInstallDir' '$compiler' '$arch' `$$debug }") -ne 0)
			{
				Write-Host "Failed building Plugins ($arch)" -foregroundcolor Red
				$success = $false
				break
			}
		}
    
		if ($success -and !$debug -and `
			($createInstallCheckBox.CheckState -eq $checked -and (Execute-ProcessToHost "." "powershell" -command "& { . '.\Build Components.ps1'; Install-AdmComponent '$pluginInstallDir' '$mainInstallDir' }") -ne 0))
		{
			Write-Host "Failed installing Plugins ($arch)" -foregroundcolor Red
			$success = $false
			break
		}
	}

	if ($success)
	{
		$pluginCheckBox.CheckState = [System.Windows.Forms.CheckState]::Unchecked
		[System.Windows.Forms.Application]::DoEvents()
	}
    
	if ($success -and 
		($createSdkCheckBox.CheckState -eq $checked -and (Execute-ProcessToHost "." "powershell" -command "& { . '.\Build Components.ps1'; Install-ExternalSdk '$sdkInstallDir' '$arch' }") -ne 0))
	{
		Write-Host "Failed installing external SDK components ($arch)" -foregroundcolor Red
		$success = $false
		break
	}


	# == Install dependencies ==
	if ($createInstallCheckBox.CheckState -eq $checked -and $success)
	{
		For ($archIndex = 0; $archIndex -lt $arches.length; $archIndex++)
		{
			$arch = $arches[$archIndex]
        
			[string] $mainInstallDir = $null

            if ($debug)
            {
                $mainInstallDir = Get-ComponentPath "" $rootBuildDirs[$archIndex] $compiler $debug
            }
            else
            {
                $mainInstallDir = Get-AdmInstallDir $compiler $arch $debug
            }

			if ((Execute-ProcessToHost `
				"." "powershell" -command "& { . '.\Build Components.ps1'; Install-AdmDependencies '$rootSourceDir' '$mainInstallDir' '$compiler' '$arch' `$$debug }") -ne 0)
			{
				Write-Host "Failed installing dependencies ($arch)" -foregroundcolor Red
				$success = $false
				break
			}

			Set-Content -Path (Join-Path $mainInstallDir "Build Info.txt") -Value ""
		}
	}

	if ($success)
	{
		$createInstallCheckBox.CheckState = [System.Windows.Forms.CheckState]::Unchecked
		$createSdkCheckBox.CheckState = [System.Windows.Forms.CheckState]::Unchecked

		[System.Windows.Forms.Application]::DoEvents()
	}

	# == Package Avidemux ==
	if ($packageInstallCheckBox.CheckState -eq $checked -and $success)
	{
		[string] $svnXmlLog = $null

		Write-Host "Retrieving SVN log"

		if ((Execute-ProcessToString ([ref] $svnXmlLog) "." "$svnPath" "log" "--stop-on-copy" "--xml" "$rootSourceDir") -ne 0)
		{
			Write-Host "Failed executing SVN" -foregroundcolor Red
			$success = $false
		}

		[string] $changeLog = Convert-XmlWithXslt $svnXmlLog (Join-Path $rootSourceDir "avidemux\winInstaller\svnlog.xslt")
		[string] $packageLog = Convert-XmlFileWithXslt "Package Notes.xml" (Resolve-Path -Path ".\Tools\package_notes.xslt")
		[string] $svnRevision = Get-SvnRevision $rootSourceDir
		[string] $packageDir = Force-Resolve-Path $admPackageDir

		if (!(Test-Path -path $packageDir))
		{
			$null = New-Item -Path $packageDir -ItemType Directory
		}

		Set-Content -Path (Join-Path $packageDir "Change Log.html") -Value $changeLog
		Set-Content -Path (Join-Path $packageDir "Package Notes.html") -Value $packageLog

		For ($archIndex = 0; $archIndex -lt $arches.length; $archIndex++)
		{
			$arch = $arches[$archIndex]

			[string] $mainInstallDir = Get-AdmInstallDir $compiler $arch $debug
			[string] $winInstallerDir = Join-Path $rootSourceDir "avidemux\winInstaller"
			[string] $packageName = Get-PackageFileName $admPackagePattern $arch $svnRevision
        
			Set-Content -Path (Join-Path $mainInstallDir "Change Log.html") -Value $changeLog

			if ((Execute-ProcessToHost `
				"." "powershell" -command "& { . '.\Build Components.ps1'; Create-Installer '$mainInstallDir' '$winInstallerDir' '$packageDir' '$packageName' '$svnRevision' '$arch' }") -ne 0)
			{
				Write-Host "Failed packaging ($arch)" -foregroundcolor Red
				$success = $false
				break
			}
		}
	}

	if ($success)
	{
		$packageInstallCheckBox.CheckState = [System.Windows.Forms.CheckState]::Unchecked
		[System.Windows.Forms.Application]::DoEvents()
	}

	# == Package Avidemux SDK ==
	if ($packageSdkCheckBox.CheckState -eq $checked -and $success)
	{
		[string] $packageDir = Force-Resolve-Path $admPackageDir

		if (!(Test-Path -path $packageDir))
		{
			$null = New-Item -Path $packageDir -ItemType Directory
		}

		For ($archIndex = 0; $archIndex -lt $arches.length; $archIndex++)
		{
			$arch = $arches[$archIndex]

			[string] $sdkInstallDir = Get-AdmSdkInstallDir $compiler $arch $debug
			[string] $svnRevision = Get-SvnRevision $rootSourceDir
			[string] $packageName = Get-PackageFileName $admSdkPackagePattern $arch $svnRevision

			Create-Zip $sdkInstallDir (Join-Path $packageDir ($packageName + ".zip"))
		}
	}

	if ($success)
	{
		$packageSdkCheckBox.CheckState = [System.Windows.Forms.CheckState]::Unchecked
		Write-Host "Finished" -foregroundcolor Green
	}
}