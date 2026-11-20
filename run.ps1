[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][string]$Lesson,
    [ValidateSet('test', 'demo', 'check')][string]$Mode = 'test',
    [switch]$BuildOnly,
    [ValidateRange(1, 120)][int]$TimeoutSeconds = 10
)
$ErrorActionPreference = 'Stop'
$compiler = if ($env:CXX) { $env:CXX } else { 'g++' }
if (-not (Get-Command $compiler -ErrorAction SilentlyContinue)) {
    Write-Error 'C++ compiler missing. Add g++ to PATH, or set CXX to its executable path.'
    exit 1
}
$folders = @(Get-ChildItem -LiteralPath $PSScriptRoot -Directory | Where-Object { $_.Name -match '^\d{2}_' -and $_.Name -notmatch '^11_' } | Sort-Object Name)
if ($Lesson -ne 'all') {
    $folders = @($folders | Where-Object { $_.Name -eq $Lesson -or $_.Name.StartsWith("${Lesson}_") })
}
if ($folders.Count -eq 0) { throw 'Choose 01..10, a full folder name, or all. See lesson 11 README for CMake.' }
$buildPath = Join-Path $PSScriptRoot 'build'
New-Item -ItemType Directory -Force -Path $buildPath | Out-Null
$overall = 0
foreach ($folder in $folders) {
    $entry = if ($Mode -eq 'demo') { 'main.cpp' } else { 'test.cpp' }
    $binary = Join-Path $buildPath ($folder.Name + '_' + $Mode + '.exe')
    Write-Host "BUILD $($folder.Name) $Mode"
    & $compiler '-std=c++17' '-Wall' '-Wextra' '-Werror' '-pthread' (Join-Path $folder.FullName 'lesson.cpp') (Join-Path $folder.FullName $entry) '-o' $binary
    if ($LASTEXITCODE -ne 0) { exit 1 }
    if ($BuildOnly) { continue }
    $info = New-Object System.Diagnostics.ProcessStartInfo
    $info.FileName = $binary
    $info.WorkingDirectory = $folder.FullName
    $info.UseShellExecute = $false
    $info.CreateNoWindow = $true
    $info.RedirectStandardOutput = $true
    $info.RedirectStandardError = $true
    $child = New-Object System.Diagnostics.Process
    $child.StartInfo = $info
    try {
        [void]$child.Start()
        $stdoutTask = $child.StandardOutput.ReadToEndAsync()
        $stderrTask = $child.StandardError.ReadToEndAsync()
        if (-not $child.WaitForExit($TimeoutSeconds * 1000)) {
            $child.Kill()
            $child.WaitForExit()
            Write-Host '[FAIL] TIMEOUT: possible deadlock; process stopped.'
            exit 124
        }
        $stdout = $stdoutTask.GetAwaiter().GetResult()
        $stderr = $stderrTask.GetAwaiter().GetResult()
        Write-Host $stdout -NoNewline
        if ($stderr) { Write-Host $stderr -NoNewline }
        $code = $child.ExitCode
    } finally { $child.Dispose() }
    if ($Mode -eq 'check') {
        if ($code -ne 2 -or $stdout -notmatch '\[TODO\]' -or $stdout -match '\[FAIL\]') {
            Write-Host '[FAIL] Expected an untouched TODO scaffold, not a finished exercise.'
            exit 1
        }
        Write-Host 'SCAFFOLD OK (exercise remains TODO)'
    } elseif ($code -eq 1 -or ($code -ne 0 -and $code -ne 2)) {
        $overall = 1
    } elseif ($code -eq 2 -and $overall -eq 0) { $overall = 2 }
}
exit $overall
