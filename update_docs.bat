@echo off
echo Generating documentation using PowerShell...
powershell -ExecutionPolicy Bypass -File "scripts\doc_gen.ps1"
echo Done.
IF %ERRORLEVEL% NEQ 0 (
  echo Error generating documentation.
  pause
)
