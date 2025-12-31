$ErrorActionPreference = "Stop"
$scriptPath = $PSScriptRoot
$projectRoot = Split-Path $scriptPath -Parent
$incDir = Join-Path $projectRoot "inc"
$docsDir = Join-Path $projectRoot "docs"
$outFile = Join-Path $docsDir "05_API_Reference.md"

if (-not (Test-Path $docsDir)) { New-Item -ItemType Directory -Path $docsDir | Out-Null }

$sb = New-Object System.Text.StringBuilder
[void]$sb.AppendLine("# API 参考 (API Reference)")
[void]$sb.AppendLine("> 生成时间: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')")
[void]$sb.AppendLine("")

$files = Get-ChildItem -Path $incDir -Filter "*.h"

foreach ($file in $files) {
    $content = Get-Content $file.FullName -Raw
    
    # Find class names
    $classMatches = [regex]::Matches($content, 'class\s+(\w+)\s*(?::\s*public\s+\w+(?:,\s*public\s+\w+)*)?\s*\{')
    
    if ($classMatches.Count -gt 0) {
        [void]$sb.AppendLine("## 文件: $($file.Name)")
        
        foreach ($match in $classMatches) {
            $className = $match.Groups[1].Value
            [void]$sb.AppendLine("### Class: ``$className``")
        }
        
        [void]$sb.AppendLine("")
        [void]$sb.AppendLine("| 返回类型 | 方法名 | 参数 |")
        [void]$sb.AppendLine("| :--- | :--- | :--- |")
        
        # Heuristic: Find method-like signatures in the file
        # This regex matches: (virtual) ReturnType Name(Args) ... ;
        $methodPattern = '^\s*(?:virtual\s+)?(\w+(?:<[^>]+>)?[\w:\*&]*)\s+(\w+)\s*\(([^)]*)\)\s*(?:const)?\s*;'
        $methodMatches = [regex]::Matches($content, $methodPattern, [System.Text.RegularExpressions.RegexOptions]::Multiline)
        
        foreach ($m in $methodMatches) {
             # Skip some common false positives or clutter
            $ret = $m.Groups[1].Value.Trim()
            $name = $m.Groups[2].Value.Trim()
            $args = $m.Groups[3].Value.Trim() -replace "`r`n", " " -replace "\s+", " "
            
            # Simple filter to avoid macros or weird parsing
            if ($name -match "^DECLARE_") { continue }
            
            [void]$sb.AppendLine("| ``$ret`` | **$name** | ``$args`` |")
        }
        [void]$sb.AppendLine("")
        [void]$sb.AppendLine("---")
    }
}

$sb.ToString() | Out-File $outFile -Encoding utf8
Write-Host "API Reference generated at $outFile"
