!define APPNAME "Skypush"
!define COMPANYNAME "Skyweb"
!define DESCRIPTION "A lightweight cross platform screenshot tool"
!define VERSIONMAJOR 1
!define VERSIONMINOR 0
!define VERSIONBUILD 0
!define HELPURL "https://google.com/"
!define UPDATEURL "https://skyweb.nu/"
!define ABOUTURL "https://github.com/tjubutsi/Skypush"
!define INSTALLSIZE 17501
 
RequestExecutionLevel admin
 
InstallDir "$PROGRAMFILES\${APPNAME}"

Name "${APPNAME}"
Icon "icon.ico"
outFile "installer.exe"
 
!include LogicLib.nsh

page directory
Page instfiles
 
!macro VerifyUserIsAdmin
	UserInfo::GetAccountType
	pop $0
	${If} $0 != "admin"
		messageBox mb_iconstop "Administrator rights required!"
		setErrorLevel 740
		quit
	${EndIf}
!macroend
 
function .onInit
	setShellVarContext all
	!insertmacro VerifyUserIsAdmin
functionEnd
 
section "install"
	setOutPath $TEMP
	file VC_redist.x64.exe
	ExecWait "$TEMP\VC_redist.x64.exe /passive /norestart"
	Delete "$TEMP\vcredist_x64.exe"
	setOutPath $INSTDIR
	
	file icon.ico
	file Skypush.exe
 
	writeUninstaller "$INSTDIR\uninstall.exe"
 
	createDirectory "$SMPROGRAMS\${APPNAME}"
	createShortCut "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk" "$INSTDIR\Skypush.exe" "" "$INSTDIR\Skypush.exe"
 
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "InstallLocation" "$\"$INSTDIR$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayIcon" "$\"$INSTDIR\icon.ico$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "Publisher" "${COMPANYNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "HelpLink" "$\"${HELPURL}$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "URLUpdateInfo" "$\"${UPDATEURL}$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "URLInfoAbout" "$\"${ABOUTURL}$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayVersion" "$\"${VERSIONMAJOR}.${VERSIONMINOR}.${VERSIONBUILD}$\""
	WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "Skypush" "$INSTDIR\Skypush.exe"
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "VersionMajor" ${VERSIONMAJOR}
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "VersionMinor" ${VERSIONMINOR}
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "NoRepair" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "EstimatedSize" ${INSTALLSIZE}
	
sectionEnd
 
function un.onInit
	SetShellVarContext all
	MessageBox MB_OKCANCEL "Permanantly remove ${APPNAME}?" IDOK next
		Abort
	next:
	!insertmacro VerifyUserIsAdmin
functionEnd
 
section "uninstall"
	delete "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk"
	rmDir "$SMPROGRAMS\${APPNAME}"
 
	delete $INSTDIR\VC_redist.x64.exe
	delete $INSTDIR\icon.ico
	delete $INSTDIR\Skypush.exe
 
	delete $INSTDIR\uninstall.exe
	rmDir /r $INSTDIR
 
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
sectionEnd