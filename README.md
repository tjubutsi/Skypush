# Skypush

## Building windows installer
Requires:
- QT
- NSIS
- QHotkey

Steps:
1. Build release mode
2. Edit extra/windows installer.ps1 to point to build/release directory, windeployqt.exe and makensis.exe
3. Run extra/windows installer.ps1

