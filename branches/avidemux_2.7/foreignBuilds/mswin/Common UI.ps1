$ErrorActionPreference = "Stop"

[Reflection.Assembly]::LoadWithPartialName("System.Windows.Forms") | Out-Null
[bool] $resizing = $false

Add-Type @"
    using System;
    using System.Runtime.InteropServices;

    public class Interop
    {
        [DllImport("user32.dll")]
        public static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);
     
        [DllImport("kernel32.dll")]
        public static extern IntPtr GetConsoleWindow();
     
        [DllImport("user32.dll")]
        public static extern bool GetWindowRect(IntPtr hWnd, out RECT lpRect);

        [DllImport("user32.dll")]
        public static extern bool SetWindowPos(IntPtr hWnd, IntPtr hWndInsertAfter, int X, int Y, int cx, int cy, uint uFlags);
    }
    
    public struct RECT
    {
        public int Left;
        public int Top;
        public int Right;
        public int Bottom;
    }
"@

function StartBuild
{
    EnableControls $false
    . ".\Build.ps1"
	Build
    EnableControls $true
}

function CheckFormState
{
    $buildButton.Enabled = ($arch32CheckBox.CheckState -eq [System.Windows.Forms.CheckState]::Checked) -or `
        ($arch64CheckBox.CheckState -eq [System.Windows.Forms.CheckState]::Checked)
}

function FormResize
{
    [IntPtr] $powerShellHandle = [Interop]::GetConsoleWindow()

    if ($form.WindowState -eq [System.Windows.Forms.FormWindowState]::Normal)
    {
        [void] [Interop]::ShowWindow($powerShellHandle, 6)
        [void] [Interop]::ShowWindow($powerShellHandle, 1)
        $form.Activate()
    }
    elseif ($form.WindowState -eq [System.Windows.Forms.FormWindowState]::Minimized)
    {
        [void] [Interop]::ShowWindow($powerShellHandle, 6)
    }
}

function LayoutWindows
{
    [System.Management.Automation.Host.Size] $consoleSize = (Get-Host).UI.RawUI.WindowSize
    
    (Get-Host).UI.RawUI.BufferSize = New-Object System.Management.Automation.Host.Size -argumentList 100, 420
    (Get-Host).UI.RawUI.WindowSize = New-Object System.Management.Automation.Host.Size -argumentList 100, 30

    [IntPtr] $powerShellHandle = [Interop]::GetConsoleWindow()
    [RECT] $rect = New-Object RECT    
    
    $null = [Interop]::GetWindowRect($powerShellHandle, [ref] $rect) 
    
    [int] $screenHeight = [System.Windows.Forms.Screen]::PrimaryScreen.WorkingArea.Height
    [int] $screenWidth = [System.Windows.Forms.Screen]::PrimaryScreen.WorkingArea.Width
    [int] $consoleHeight = ($rect.Bottom - $rect.Top)
    [int] $consoleWidth = ($rect.Right - $rect.Left)
    [int] $formY = ($screenHeight - ($form.Size.Height + $consoleHeight)) / 2
    
    $form.StartPosition = [System.Windows.Forms.FormStartPosition]::Manual
    $form.Location = New-Object System.Drawing.Point -argumentList (($screenWidth - $form.Size.Width) / 2), $formY
    
    $null = [Interop]::SetWindowPos($powerShellHandle, 0, ($screenWidth - $consoleWidth) / 2, $formY + $form.Size.Height + 4, 0, 0, 0x0001 -bor 0x0004)
}

function EnableControls([bool] $enabled)
{
    [System.Windows.Forms.Control] $control = $null
    
    Foreach ($control in $form.Controls)
    {
        $control.Enabled = $enabled
    }
    
    [System.Windows.Forms.Application]::DoEvents()
}

function GetCompilerTag
{
    if ($compilerComboBox.SelectedItem -eq "GCC")
    {
        "gcc"
    }
    elseif ($compilerComboBox.SelectedItem -eq "MSVC 11")
    {
        "msvc11"
    }
    else
    {
        throw "Unrecognised compiler"
    }
}

function GetArchitectureTags
{
    [string[]] $arches = @()

    if ($arch32CheckBox.CheckState -eq [System.Windows.Forms.CheckState]::Checked)
    {
        $arches += "x86"
    }

    if ($arch64CheckBox.CheckState -eq [System.Windows.Forms.CheckState]::Checked)
    {
        $arches += "x86-64"
    }

    $arches
}

[System.Windows.Forms.Form] $form = New-Object System.Windows.Forms.Form
[System.Windows.Forms.Label] $compilerLabel = New-Object System.Windows.Forms.Label
[System.Windows.Forms.ComboBox] $compilerComboBox = New-Object System.Windows.Forms.ComboBox
[System.Windows.Forms.CheckBox] $debugCheckBox = New-Object System.Windows.Forms.CheckBox

[System.Windows.Forms.GroupBox] $archGroupBox = New-Object System.Windows.Forms.GroupBox
[System.Windows.Forms.CheckBox] $arch32CheckBox = New-Object System.Windows.Forms.CheckBox
[System.Windows.Forms.CheckBox] $arch64CheckBox = New-Object System.Windows.Forms.CheckBox

[System.Windows.Forms.Button] $buildButton = New-Object System.Windows.Forms.Button
[System.Windows.Forms.Button] $closeButton = New-Object System.Windows.Forms.Button

# == Build Form ==
$compilerLabel.AutoSize = $true
$compilerLabel.Location = New-Object System.Drawing.Point(30, 23)
$compilerLabel.Text = "Compiler:"

$compilerComboBox.DropDownStyle = [System.Windows.Forms.ComboBoxStyle]::DropDownList
$compilerComboBox.Location = New-Object System.Drawing.Point(84, 19)
$compilerComboBox.Size = New-Object System.Drawing.Size(83, 21)

$debugCheckBox.AutoSize = $true
$debugCheckBox.Location = New-Object System.Drawing.Point(192, 21)
$debugCheckBox.Size = New-Object System.Drawing.Size(58, 17)
$debugCheckBox.Text = "Debug"

$archGroupBox.Controls.Add($arch64CheckBox)
$archGroupBox.Controls.Add($arch32CheckBox)
$archGroupBox.Location = New-Object System.Drawing.Point(40, 59)
$archGroupBox.Size = New-Object System.Drawing.Size(200, 51)
$archGroupBox.Text = "Architecture"

$arch32CheckBox.AutoSize = $true
$arch32CheckBox.CheckState = [System.Windows.Forms.CheckState]::Checked
$arch32CheckBox.Location = New-Object System.Drawing.Point(35, 23)
$arch32CheckBox.Size = New-Object System.Drawing.Size(52, 17)
$arch32CheckBox.Text = "32-bit"
$arch32CheckBox.Add_CheckedChanged({CheckFormState})

$arch64CheckBox.AutoSize = $true
$arch64CheckBox.CheckState = [System.Windows.Forms.CheckState]::Checked
$arch64CheckBox.Location = New-Object System.Drawing.Point(113, 23)
$arch64CheckBox.Size = New-Object System.Drawing.Size(52, 17)
$arch64CheckBox.Text = "64-bit"
$arch64CheckBox.Add_CheckedChanged({ CheckFormState })

$buildButton.Location = New-Object System.Drawing.Point(55, 132)
$buildButton.Size = New-Object System.Drawing.Size(75, 23)
$buildButton.Text = "Build"
$buildButton.Add_Click({ StartBuild })

$closeButton.Location = New-Object System.Drawing.Point(151, 132)
$closeButton.Size = New-Object System.Drawing.Size(75, 23)
$closeButton.Text = "Close"

$form.Controls.Add($compilerLabel)
$form.Controls.Add($compilerComboBox)
$form.Controls.Add($debugCheckBox)
$form.Controls.Add($archGroupBox)
$form.Controls.Add($buildButton)
$form.Controls.Add($closeButton)

$form.AcceptButton = $buildButton
$form.CancelButton = $closeButton
$form.ClientSize = New-Object System.Drawing.Size(281, 165)
$form.FormBorderStyle = [System.Windows.Forms.FormBorderStyle]::FixedSingle
$form.MaximizeBox = $false
$form.Text = "Build"
$form.Add_Activated({ FormResize })
$form.Add_Resize({ FormResize })

LayoutWindows