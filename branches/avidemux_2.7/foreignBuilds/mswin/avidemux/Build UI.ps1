. "..\Common UI.ps1"

[System.Windows.Forms.GroupBox] $componentGroupBox = New-Object System.Windows.Forms.GroupBox
[System.Windows.Forms.CheckBox] $ffmpegCheckBox = New-Object System.Windows.Forms.CheckBox
[System.Windows.Forms.CheckBox] $coreCheckBox = New-Object System.Windows.Forms.CheckBox
[System.Windows.Forms.CheckBox] $qtCheckBox = New-Object System.Windows.Forms.CheckBox
[System.Windows.Forms.CheckBox] $cliCheckBox = New-Object System.Windows.Forms.CheckBox
[System.Windows.Forms.CheckBox] $pluginCheckBox = New-Object System.Windows.Forms.CheckBox

[System.Windows.Forms.GroupBox] $postBuildGroupBox = New-Object System.Windows.Forms.GroupBox
[System.Windows.Forms.CheckBox] $createInstallCheckBox = New-Object System.Windows.Forms.CheckBox
[System.Windows.Forms.CheckBox] $packageInstallCheckBox = New-Object System.Windows.Forms.CheckBox
[System.Windows.Forms.CheckBox] $createSdkCheckBox = New-Object System.Windows.Forms.CheckBox
[System.Windows.Forms.CheckBox] $packageSdkCheckBox = New-Object System.Windows.Forms.CheckBox

# == Build Form ==
$componentGroupBox.Controls.Add($coreCheckBox)
$componentGroupBox.Controls.Add($ffmpegCheckBox)
$componentGroupBox.Controls.Add($pluginCheckBox)
$componentGroupBox.Controls.Add($qtCheckBox)
$componentGroupBox.Controls.Add($cliCheckBox)
$componentGroupBox.Location = New-Object System.Drawing.Point(65, 123)
$componentGroupBox.Size = New-Object System.Drawing.Size(150, 150)
$componentGroupBox.Text = "Components"

$ffmpegCheckBox.AutoSize = $true;
$ffmpegCheckBox.CheckState = [System.Windows.Forms.CheckState]::Checked
$ffmpegCheckBox.Location = New-Object System.Drawing.Point(43, 23)
$ffmpegCheckBox.Size = New-Object System.Drawing.Size(64, 17)
$ffmpegCheckBox.Text = "FFmpeg"

$coreCheckBox.AutoSize = $true
$coreCheckBox.CheckState = [System.Windows.Forms.CheckState]::Checked
$coreCheckBox.Location = New-Object System.Drawing.Point(43, 47)
$coreCheckBox.Size = New-Object System.Drawing.Size(48, 17)
$coreCheckBox.Text = "Core"

$qtCheckBox.AutoSize = $true
$qtCheckBox.CheckState = [System.Windows.Forms.CheckState]::Checked
$qtCheckBox.Location = New-Object System.Drawing.Point(43, 71)
$qtCheckBox.Size = New-Object System.Drawing.Size(51, 17)
$qtCheckBox.Text = "Qt UI"

$cliCheckBox.AutoSize = $true
$cliCheckBox.CheckState = [System.Windows.Forms.CheckState]::Checked
$cliCheckBox.Location = New-Object System.Drawing.Point(43, 95)
$cliCheckBox.Size = New-Object System.Drawing.Size(42, 17)
$cliCheckBox.Text = "CLI"

$pluginCheckBox.AutoSize = $true
$pluginCheckBox.CheckState = [System.Windows.Forms.CheckState]::Checked
$pluginCheckBox.Location = New-Object System.Drawing.Point(43, 119)
$pluginCheckBox.Size = New-Object System.Drawing.Size(60, 17)
$pluginCheckBox.Text = "Plugins"

$postBuildGroupBox.Controls.Add($packageSdkCheckBox)
$postBuildGroupBox.Controls.Add($createSdkCheckBox)
$postBuildGroupBox.Controls.Add($packageInstallCheckBox)
$postBuildGroupBox.Controls.Add($createInstallCheckBox)
$postBuildGroupBox.Location = New-Object System.Drawing.Point(65, 286)
$postBuildGroupBox.Size = New-Object System.Drawing.Size(150, 124)
$postBuildGroupBox.Text = "Post Build"

$createInstallCheckBox.AutoSize = $true
$createInstallCheckBox.CheckState = [System.Windows.Forms.CheckState]::Checked
$createInstallCheckBox.Location = New-Object System.Drawing.Point(14, 23)
$createInstallCheckBox.Size = New-Object System.Drawing.Size(110, 17)
$createInstallCheckBox.Text = "Create Installation"

$createSdkCheckBox.AutoSize = $true
$createSdkCheckBox.CheckState = [System.Windows.Forms.CheckState]::Checked
$createSdkCheckBox.Location = New-Object System.Drawing.Point(14, 47)
$createSdkCheckBox.Size = New-Object System.Drawing.Size(82, 17)
$createSdkCheckBox.Text = "Create SDK"

$packageInstallCheckBox.AutoSize = $true
$packageInstallCheckBox.Location = New-Object System.Drawing.Point(14, 71)
$packageInstallCheckBox.Size = New-Object System.Drawing.Size(122, 17)
$packageInstallCheckBox.Text = "Package Installation"

$packageSdkCheckBox.AutoSize = $true
$packageSdkCheckBox.Location = New-Object System.Drawing.Point(14, 95)
$packageSdkCheckBox.Size = New-Object System.Drawing.Size(94, 17)
$packageSdkCheckBox.Text = "Package SDK"

$buildButton.Location = New-Object System.Drawing.Point(55, 432)
$closeButton.Location = New-Object System.Drawing.Point(151, 432)

$form.Controls.Add($componentGroupBox)
$form.Controls.Add($postBuildGroupBox)

$form.ClientSize = New-Object System.Drawing.Size(281, 465)
$form.Text = "Build Avidemux"

$arch32CheckBox.CheckState = [System.Windows.Forms.CheckState]::Unchecked
$compilerComboBox.Items.AddRange(@("GCC", "MSVC 11"));
$compilerComboBox.SelectedIndex = 0;

LayoutWindows
$form.ShowDialog() | Out-Null