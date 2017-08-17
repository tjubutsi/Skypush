$PSScriptRoot = ($MyInvocation.MyCommand.Path | Split-Path | Resolve-Path).ProviderPath
md temp
Copy-Item ..\src\build\release\Skypush.exe $PSScriptRoot\temp
Copy-Item ..\src\Skypush\images\icon.png $PSScriptRoot\temp
Copy-Item ..\src\Skypush\images\icon.ico $PSScriptRoot\temp
Copy-Item libeay32.dll $PSScriptRoot\temp
Copy-Item ssleay32.dll $PSScriptRoot\temp
Copy-Item VC_redist.x64.exe $PSScriptRoot\temp
& 'D:\windows_dev\qt-5.9.1\5.9.1\msvc2017_64\bin\windeployqt.exe' $PSScriptRoot\temp
Copy-Item .\installer.nsi $PSScriptRoot\temp -force
& 'C:\Program Files (x86)\NSIS\makensis.exe' .\temp\example.nsi
Move-Item $PSScriptRoot\temp\installer.exe $PSScriptRoot\Skypush_1.0.0.exe -force
Remove-Item -Recurse -Force temp