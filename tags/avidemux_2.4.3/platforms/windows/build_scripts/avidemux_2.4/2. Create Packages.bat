@call "2a. Update Notes.bat"

if errorlevel 1 goto error

@call "2b. Package Build.bat"

goto end

:error
set ERRORLEVEL 1

:end