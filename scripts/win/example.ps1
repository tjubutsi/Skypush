$PSScriptRoot = ($MyInvocation.MyCommand.Path | Split-Path | Resolve-Path).ProviderPath
md temp
Copy-Item ..\Skypush-build-static\release\Skypush.exe $PSScriptRoot\temp
Copy-Item ..\Skypush\src\images\icon.ico $PSScriptRoot\temp
Copy-Item VC_redist.x64.exe $PSScriptRoot\temp
Copy-Item .\installer.nsi $PSScriptRoot\temp -force
& 'C:\Program Files (x86)\NSIS\makensis.exe' .\temp\installer.nsi
Move-Item $PSScriptRoot\temp\installer.exe $PSScriptRoot\Skypush_1.0.0.exe -force
Remove-Item -Recurse -Force temp