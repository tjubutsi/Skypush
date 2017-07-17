$PSScriptRoot = ($MyInvocation.MyCommand.Path | Split-Path | Resolve-Path).ProviderPath
md temp
Copy-Item ..\build\release\Skypush.exe $PSScriptRoot\temp
Copy-Item ..\images\icon.png $PSScriptRoot\temp
Copy-Item ..\images\icon.ico $PSScriptRoot\temp
& 'D:\windows_dev\qt\5.9\msvc2017_64\bin\windeployqt.exe' $PSScriptRoot\temp
Copy-Item .\installer.nsi $PSScriptRoot\temp -force
& 'C:\Program Files (x86)\NSIS\makensis.exe' .\temp\installer.nsi
Move-Item $PSScriptRoot\temp\installer.exe $PSScriptRoot\Skypush_1.0.0.exe -force
Remove-Item -Recurse -Force temp