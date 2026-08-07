# Script: compile_papyrus.ps1
# Automates compiling Papyrus .psc source files into .pex binaries for Cardinal Weapons Framework

param (
    [string]$SkyrimPath = "C:\Program Files (x86)\Steam\steamapps\common\Skyrim Special Edition",
    [string]$SourceDir = "$PSScriptRoot\source",
    [string]$OutputDir = "$PSScriptRoot\..\dist\staging\Scripts"
)

Write-Host "=====================================================" -ForegroundColor Cyan
Write-Host "    CARDINAL WEAPONS FRAMEWORK - PAPYRUS COMPILER    " -ForegroundColor Cyan
Write-Host "=====================================================" -ForegroundColor Cyan

$CompilerPath = "$SkyrimPath\Papyrus Compiler\PapyrusCompiler.exe"
if (-not (Test-Path $CompilerPath)) {
    $CompilerPath = "$SkyrimPath\PapyrusCompiler.exe"
}

if (-not (Test-Path $CompilerPath)) {
    Write-Host "[WARNING] Creation Kit PapyrusCompiler.exe not found at: $CompilerPath" -ForegroundColor Yellow
    Write-Host "Please ensure Skyrim Special Edition Creation Kit is installed." -ForegroundColor Yellow
    Write-Host "Expected path: '$SkyrimPath\Papyrus Compiler\PapyrusCompiler.exe'" -ForegroundColor Yellow
    exit 1
}

$FlagsFile = "$SkyrimPath\Data\Scripts\Source\TESV_Papyrus_Flags.flg"
if (-not (Test-Path $FlagsFile)) {
    $FlagsFile = "$SkyrimPath\Papyrus Compiler\TESV_Papyrus_Flags.flg"
}

$SkyrimSource = "$SkyrimPath\Data\Scripts\Source"
if (-not (Test-Path $SkyrimSource)) {
    $SkyrimSource = "$SkyrimPath\Data\Source\Scripts"
}

if (-not (Test-Path $OutputDir)) {
    New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null
}

$GameScriptsDir = "$SkyrimPath\Data\Scripts"

Write-Host "[INFO] Compiler:     $CompilerPath" -ForegroundColor Green
Write-Host "[INFO] Source Dir:   $SourceDir" -ForegroundColor Green
Write-Host "[INFO] Output Dir:   $OutputDir" -ForegroundColor Green
Write-Host "[INFO] Skyrim Source:$SkyrimSource" -ForegroundColor Green

$PscFiles = Get-ChildItem -Path $SourceDir -Filter "*.psc" | Where-Object { $_.DirectoryName -notlike "*stubs*" }

$SuccessCount = 0
$FailureCount = 0

foreach ($File in $PscFiles) {
    Write-Host "Compiling $($File.Name)..." -NoNewline
    $Arguments = "`"$($File.FullName)`" -f=`"$FlagsFile`" -i=`"$SourceDir;$SkyrimSource`" -o=`"$OutputDir`" -optimize"
    
    $Process = Start-Process -FilePath $CompilerPath -ArgumentList $Arguments -Wait -NoNewWindow -PassThru
    
    if ($Process.ExitCode -eq 0) {
        Write-Host " SUCCESS" -ForegroundColor Green
        $SuccessCount++
        
        # Copy to active game directory if it exists
        $PexName = [System.IO.Path]::ChangeExtension($File.Name, ".pex")
        $CompiledPex = Join-Path $OutputDir $PexName
        if ((Test-Path $GameScriptsDir) -and (Test-Path $CompiledPex)) {
            Copy-Item -Path $CompiledPex -Destination $GameScriptsDir -Force
        }
    } else {
        Write-Host " FAILED (Exit Code $($Process.ExitCode))" -ForegroundColor Red
        $FailureCount++
    }
}

Write-Host "=====================================================" -ForegroundColor Cyan
Write-Host "  SUMMARY: $SuccessCount Compiled, $FailureCount Failed" -ForegroundColor Cyan
Write-Host "=====================================================" -ForegroundColor Cyan
