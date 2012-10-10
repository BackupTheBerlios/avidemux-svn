function Get-SourceDir([string] $arch, [bool] $debug)
{
    [string] $path = Join-Path $devDir "$libraryName-$version-$compiler-$arch"

    if ($debug)
    {
        $path += "-dbg"
    }

    $path
}

function Create-MsvcLib([string] $dllPath, [string] $outputDir, [string] $arch, [string] $libName)
{
    Setup-Msvc10Environment $arch $false

    [string] $basePath = Join-Path $outputDir ([System.IO.Path]::GetFileNameWithoutExtension($dllPath))
    [string] $outputLibName = $null

    if ("" -eq $libName)
    {
        $outputLibName = $basePath
    }
    else
    {
        $outputLibName = Join-Path $outputDir $libName
    }

    if ((Execute-ProcessToHost "$outputDir" (Join-Path "$externalToolsDir" "gendef.exe") "$dllPath" ) -ne 0)
	{
		throw "Error generating Visual C++ library ($arch)"
	}

    if ((Execute-ProcessToHost "$outputDir" "lib.exe" "/def:$basePath.def" "/out:$outputLibName.lib") -ne 0)
	{
		throw "Error generating Visual C++ library ($arch)"
	}

    Remove-Item "$basePath.def"
}

if ($compiler -eq "gcc")
{
    Setup-MsysEnvironment $arch $debug
}
elseif ($compiler -eq "msvc10")
{
    Setup-Msvc10Environment $arch $debug
}

[string] $sourceDir = Get-SourceDir $arch $debug
[string] $externalLibDir = (Get-ExternalLibPrefix $arch).Replace("\", "/")

if ($null -ne $url)
{
	Download-File $url $tarballFile $true
}

if ($null -ne $tarballFile)
{
    Extract-GzipBzip (Resolve-Path (Join-Path "." $tarballFile)) $sourceDir $tarOutputSubDir
}

if ($null -ne $gitUrl)
{
    Create-FreshDirectory $sourceDir

	if ((Execute-ProcessToHost "$devDir" "$gitPath" "clone" "$gitUrl" "$sourceDir") -ne 0)
	{
		throw "Error with git clone ($arch)"
	}
}

if ($null -ne $gitStartPoint)
{
	if ((Execute-ProcessToHost "$sourceDir" "$gitPath" "checkout" "$gitStartPoint") -ne 0)
	{
		throw "Error with git checkout ($arch)"
	}
}

if ($null -ne $patchFiles)
{
    [string] $patchFile = $null

    Foreach ($patchFile in $patchFiles)
    {
        Patch-File $sourceDir (Resolve-Path $patchFile)
    }
}

if ($skipConfigure -ne $true)
{
    if ($null -eq $configureParams)
    {
        $configureParams = "--disable-static"
    }

	if ((Execute-ProcessToHost "$sourceDir" (Join-Path $msysDir "bin\sh.exe") "./configure" "--prefix=`"$externalLibDir`"" $configureParams) -ne 0)
	{
		throw "Error configuring ($arch)"
	}
}

if ($skipMake -ne $true)
{
    if ($compiler -eq "gcc")
    {
        if ($null -eq $makeParams)
        {
            $makeParams = "install-strip"
        }

        if ((Execute-ProcessToHost "$sourceDir" (Join-Path $msysDir "bin\sh.exe") "-c" "make.exe $makeParams") -ne 0)
        {
	        throw "Error building ($arch)"
        }
    }
    elseif ($compiler -eq "msvc10")
    {
        if ($null -eq $makeParams)
        {
            $makeParams = "install"
        }

        if ((Execute-ProcessToHost "$sourceDir" "nmake.exe" "$makeParams") -ne 0)
        {
	        throw "Error building ($arch)"
        }
    }
}