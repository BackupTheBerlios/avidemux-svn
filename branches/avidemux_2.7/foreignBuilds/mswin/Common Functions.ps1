. "..\Common Variables.ps1"
Import-Module "..\InvokeBatchFile.psm1"

function Create-FreshDirectory([string] $directory)
{
    if (Test-Path $directory)
    {
        Remove-Item -Path $directory -Recurse -Force
    }

    $null = New-Item $directory -Type Directory
}

function Force-Resolve-Path([string] $filename)
{
    $filename = Resolve-Path $filename -ErrorAction SilentlyContinue -ErrorVariable frpError
    
    if (!$filename)
    {
        $frpError[0].TargetObject
    }
    else
    {
        $filename
    }
}

function Execute-ProcessToHost([string] $workingDirectory, [string] $cmd)
{
    [string] $currentDirectory = Get-Location
    [int] $exitCode = 0

    try
    {
        Set-Location $workingDirectory    

        & $cmd $args | Out-Host

        $exitCode = $LastExitCode
    }
    finally
    {
        Set-Location $currentDirectory
    }

    $exitCode
}

function Execute-ProcessToFile([string] $outputFile, [string] $workingDirectory, [string] $cmd)
{
    [string] $currentDirectory = Get-Location

    Set-Location $workingDirectory    

    & $cmd $args | Out-File $outputFile -encoding "UTF8"

    [int] $exitCode = $LastExitCode

    Set-Location $currentDirectory

    $exitCode
}

function Execute-ProcessToString([ref] $output, [string] $workingDirectory, [string] $cmd)
{
    [string] $currentDirectory = Get-Location

    Set-Location $workingDirectory    

    $output.Value = (& $cmd $args) | Out-String

    [int] $exitCode = $LastExitCode

    Set-Location $currentDirectory

    $exitCode
}

function Convert-XmlWithXslt([xml] $xml, [string] $xsltPath)
{
    [System.Xml.Xsl.XslCompiledTransform] $xslt = New-Object System.Xml.Xsl.XslCompiledTransform
    [System.Text.StringBuilder] $builder = New-Object System.Text.StringBuilder
    [System.IO.StringWriter] $writer = New-Object System.IO.StringWriter -ArgumentList $builder
    
    $xslt.Load($xsltPath)
    $xslt.Transform($xml, $null, $writer)

    $writer.Close()
    $builder.ToString()
}

function Convert-XmlFileWithXslt([string] $xmlPath, [string] $xsltPath)
{
    [System.Xml.Xsl.XslCompiledTransform] $xslt = New-Object System.Xml.Xsl.XslCompiledTransform
    [System.Text.StringBuilder] $builder = New-Object System.Text.StringBuilder
    [System.IO.StringWriter] $writer = New-Object System.IO.StringWriter -ArgumentList $builder
    
    $xslt.Load($xsltPath)
    $xslt.Transform($xmlPath, $null, $writer)

    $writer.Close()
    $builder.ToString()
}

function Create-Zip([string] $sourcePath, [string] $outputZipPath)
{
    [Reflection.Assembly]::LoadWithPartialName("System.IO.Compression") | Out-Null
    [Reflection.Assembly]::LoadWithPartialName("System.IO.Compression.FileSystem") | Out-Null

    if (Test-Path -Path $outputZipPath)
    {
        Remove-Item $outputZipPath -Recurse -Force
    }

    [System.IO.Compression.ZipFile]::CreateFromDirectory(`
        $sourcePath, $outputZipPath, [System.IO.Compression.CompressionLevel]::Optimal, $false)

    if ($advZipPath -ne "" -and `
        (Execute-ProcessToHost (Split-Path -Path $outputZipPath) $advZipPath "-z" "-4" $outputZipPath) -ne 0)
    {
        throw "Failed executing advzip"
    }
}

function Extract-Zip([string] $sourcePath, [string] $destinationDir)
{
    [Reflection.Assembly]::LoadWithPartialName("System.IO.Compression") | Out-Null
    [Reflection.Assembly]::LoadWithPartialName("System.IO.Compression.FileSystem") | Out-Null

    if (Test-Path -Path $destinationDir)
    {
        Remove-Item $destinationDir -Recurse -Force
    }

    [System.IO.Compression.ZipFile]::ExtractToDirectory($sourcePath, $destinationDir)
}

function Extract-GzipBzip([string] $packagePath, [string] $outputDir, [string] $outputSubDir)
{
    Create-FreshDirectory $outputDir

	if ((Execute-ProcessToHost "$outputDir" "$sevenZipPath" "x" $packagePath) -ne 0)
	{
		throw "Error extracting $packagePath to $outputDir"
	}

	[string] $tarPath = Join-Path $outputDir ([System.IO.Path]::GetFileNameWithoutExtension($packagePath))

	if ((Execute-ProcessToHost "$outputDir" "$sevenZipPath" "x" $tarPath) -ne 0)
	{
		throw "Error extracting $tarPath to $outputDir"
	}

	Remove-Item -Path $tarPath

    if ("." -ne $outputSubDir)
    {
        if ("" -eq $outputSubDir)
        {
            $outputSubDir = Join-Path $outputDir ([System.IO.Path]::GetFileNameWithoutExtension($tarPath))
        }
        else
        {
            $outputSubDir = Join-Path $outputDir $outputSubDir
        }

        Move-Item -Path ($outputSubDir + "\*") $outputDir
        Remove-Item -Path $outputSubDir
    }
}

function Download-File([string] $url, [string] $outputPath, [bool] $preserveExistingFile)
{
    [System.Net.WebClient] $webClient = New-Object System.Net.WebClient
    [bool] $performDownload = $true

    if ($preserveExistingFile -and (Test-Path $outputPath))
    {
        $performDownload = $false
    }

    if ($performDownload)
    {
        Write-Host "Downloading $url"

        $webClient.DownloadFile($url, $outputPath)
    }
}

function Strip-File([string] $path)
{
    Write-Host "Stripping $path"

    if ((Execute-ProcessToHost "." (Join-Path $mingwBinDir "strip.exe") $path) -ne 0)
	{
		throw "Error stripping $path"
	}
}

function Patch-File([string] $workingDir, [string] $patchPath)
{
    if ((Execute-ProcessToHost "$workingDir" (Join-Path $externalToolsDir "patch.exe") "-p0" "-i" $patchPath) -ne 0)
	{
		throw "Error applying patch $patchPath"
	}
}

function Get-BitArchitecture([string] $arch)
{
    if ($arch -eq "x86")
    {
        32
    }
    elseif ($arch -eq "x86-64")
    {
        64
    }
    else
    {
        throw "Unrecognised architecture"
    }
}

function Setup-MingwEnvironment([string] $arch, [bool] $debug)
{
    [string] $global:cmakeGenerator = "MinGW Makefiles"
    [string] $externalLibPrefix = Get-ExternalLibPrefix $arch
    [string] $mingwLibPath = Get-MingwLibPath $arch

    if ($arch -eq "x86")
    {
        $env:CFLAGS = "$env:CFLAGS -m32".Trim()
        $env:CXXFLAGS = "$env:CXXFLAGS -m32".Trim()
        $env:LDFLAGS = "$env:LDFLAGS -m32".Trim()
    }
    elseif ($arch -eq "x86-64")
    {
        $env:CFLAGS = "$env:CFLAGS -m64".Trim()
        $env:CXXFLAGS = "$env:CXXFLAGS -m64".Trim()
        $env:LDFLAGS = "$env:LDFLAGS -m64".Trim()
    }

    $env:PATH = "$mingwLibPath;" + (Join-Path $externalLibPrefix "bin") + ";$mingwBinDir;$env:PATH"
    $env:CMAKE_INCLUDE_PATH = (Join-Path $externalLibPrefix "include") + ";$mingwIncludeDir;$env:CMAKE_INCLUDE_PATH"
    $env:CMAKE_LIBRARY_PATH = (Join-Path $externalLibPrefix "lib") + ";$env:CMAKE_LIBRARY_PATH"
    $env:PKG_CONFIG_PATH = (Join-Path $externalLibPrefix "lib\pkgconfig") + ";$env:PKG_CONFIG_PATH"    

    if ($spiderMonkey_useSystemVersion)
    {
        $env:CMAKE_INCLUDE_PATH = "$env:CMAKE_INCLUDE_PATH;" + (Get-Variable -Name "spiderMonkey_includeDir_$arch".Replace("-", "_") -valueOnly)
        $env:CMAKE_LIBRARY_PATH = "$env:CMAKE_LIBRARY_PATH;" + (Get-Variable -Name "spiderMonkey_libDir_$arch".Replace("-", "_") -valueOnly)
    }

    $env:LDFLAGS = "$env:LDFLAGS -shared-libgcc"
    $env:CMAKE_PROGRAM_PATH = "$env:CMAKE_PROGRAM_PATH;$externalToolsDir"
    
    $env:QTDIR = (Get-QtDirectory "gcc" $arch $debug)
}

function Setup-MsysEnvironment([string] $arch, [bool] $debug)
{
    Setup-MingwEnvironment($arch)
    
    [string] $global:cmakeGenerator = "MSYS Makefiles"
    
    $env:PATH = "$msysDir\bin;$externalToolsDir;$env:PATH"
}

function Setup-Msvc11Environment([string] $arch, [bool] $debug)
{
    [string] $externalLibPrefix = Get-ExternalLibPrefix $arch
    [string] $mingwLibPath = Get-MingwLibPath $arch

    if ($arch -eq "x86-64")
    {
        [string] $global:cmakeGenerator = "Visual Studio 11 Win64"

        Invoke-BatchFile "$programFiles32\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" "amd64"
    }
    elseif ($arch -eq "x86")
    {
        [string] $global:cmakeGenerator = "Visual Studio 11"

        Invoke-BatchFile "$programFiles32\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" "x86"
    }

    $env:PATH = (Join-Path $externalLibPrefix "bin") + ";$env:PATH;$mingwLibPath"
    $env:CMAKE_PROGRAM_PATH = "$externalToolsDir"
    $env:CMAKE_INCLUDE_PATH = (Join-Path $externalLibPrefix "include") + ";$env:CMAKE_INCLUDE_PATH"
    $env:CMAKE_LIBRARY_PATH = (Join-Path $externalLibPrefix "lib") + ";$env:CMAKE_LIBRARY_PATH"
    $env:QTDIR = (Get-QtDirectory "msvc11" $arch $debug)

    if ($spiderMonkey_useSystemVersion)
    {
        $env:CMAKE_INCLUDE_PATH = "$env:CMAKE_INCLUDE_PATH;" + (Get-Variable -Name "spiderMonkey_includeDir_$arch".Replace("-", "_") -valueOnly)
        $env:CMAKE_LIBRARY_PATH = "$env:CMAKE_LIBRARY_PATH;" + (Get-Variable -Name "spiderMonkey_libDir_$arch".Replace("-", "_") -valueOnly)
    }
}

function Get-CmakeBuildType([bool] $debug)
{
    if ($debug)
    {
        "-DCMAKE_BUILD_TYPE=Debug"
    }
    else
    {
        "-DCMAKE_BUILD_TYPE=Release"
    }
}

function Get-MsvcBuildType([bool] $debug)
{
    if ($debug)
    {
        "/p:Configuration=Debug"
    }
    else
    {
        "/p:Configuration=Release"
    }
}

function Get-QtDirectory([string] $compiler, [string] $arch, [bool] $debug)
{
    [string] $variableName = "qtPrefix_${compiler}_$arch".Replace("-", "_")
    
    if ($debug)
    {
        $variableName += "_dbg"
    }
    
    Get-Variable -Name $variableName -valueOnly
}

function Get-ExternalLibPrefix([string] $arch)
{
    if ($arch -eq "x86-64")
    {
        $externalLib64Prefix
    }
    elseif ($arch -eq "x86")
    {
        $externalLib32Prefix
    }
    else
    {
        throw "Invalid architecture ($arch)"
    }
}

function Get-MingwLibPath([string] $arch)
{
    if ($arch -eq "x86-64")
    {
        $mingwLib64Dir
    }
    elseif ($arch -eq "x86")
    {
        $mingwLib32Dir
    }
    else
    {
        throw "Invalid architecture ($arch)"
    }
}

function Get-SvnRevision([string] $sourceDir)
{
    [string] $output = $null

    if ((Execute-ProcessToString ([ref] $output) $sourceDir $svnPath "info" ".") -ne 0)
    {
        throw "Unable to retrieve SVN revision"
    }

    [System.Text.RegularExpressions.Regex] $regex = New-Object System.Text.RegularExpressions.Regex -ArgumentList "(.*\n)?Revision: ([^\r\n]+).*"
    [System.Text.RegularExpressions.Match] $match = $regex.Match($output)

    if (!$match.Success -or $match.Groups.Count -ne 3)
    {
        throw "Invalid SVN output: $output"
    }
    
    $match.Groups[2].Value
}