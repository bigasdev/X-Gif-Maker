!include "MUI2.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "C:\Users\Escritorio2\Documents\.code\twitter-gif-maker-fortress\res\icon\icon.ico"
!define MUI_UNICON "C:\Users\Escritorio2\Documents\.code\twitter-gif-maker-fortress\res\icon\icon.ico"
!define MUI_UNINSTALLER
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_PAGE_FINISH

!define MUI_FINISHPAGE_RADIOBUTTONGROUP 1 
!define MUI_FINISHPAGE_CHECKBOXGROUP 1  

!define MUI_FINISHPAGE_CHECKBOX MyCheckBox  
!define MUI_FINISHPAGE_CHECKBOX_TEXT "Launch Twitter Gif Maker" 
!define MUI_FINISHPAGE_CHECKBOX_STATE $LAUNCH_APP

!insertmacro MUI_LANGUAGE English

; Default installation directory
InstallDir "$PROGRAMFILES\Twitter-Gif-Maker"

; Application shortcut
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "StartMenuFolder"
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "Twitter-Gif-Maker"

; Application uninstaller
!define MUI_UNPAGE_CONFIRM
!define MUI_UNPAGE_DESCRIPTION_TEXT "Remove Twitter-Gif-Maker from your computer."

; Installer sections
Section "Twitter-Gif-Maker" SEC01
  ; Set output path to the installation directory
  SetOutPath $INSTDIR

  ; Add files to be installed
  File /r "C:\Users\Escritorio2\Documents\.code\twitter-gif-maker-fortress\.release\*.*"

  ; Create a shortcut in the Start Menu folder
  CreateShortCut "$SMPROGRAMS\Twitter-Gif-Maker.lnk" "$INSTDIR\Twitter-Gif-Maker.exe"

  ; Create a desktop shortcut (optional)
  CreateShortCut "$DESKTOP\Twitter-Gif-Maker.lnk" "$INSTDIR\Twitter-Gif-Maker.exe"

  ; Write uninstaller script
  WriteUninstaller "Uninstall"
SectionEnd

; Uninstaller section
Section "Uninstall"
  ; Remove installed files and shortcuts
  Delete "$INSTDIR\Twitter-Gif-Maker.exe"
  Delete "$SMPROGRAMS\Twitter-Gif-Maker.lnk"
  Delete "$DESKTOP\Twitter-Gif-Maker.lnk"

  ; Remove installation directory
  RMDir "$INSTDIR"
SectionEnd

; Launch application after Finish page (optional)
!ifdef LAUNCH_APP
  Exec "$INSTDIR\Twitter-Gif-Maker.exe"
!endif
