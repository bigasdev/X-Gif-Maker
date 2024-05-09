; Set the name of your application
Name "Twitter-Gif-Maker"

; Set the default installation directory (e.g., Program Files)
InstallDir "$PROGRAMFILES\Twitter-Gif-Maker"

; Define the components
Section "Twitter-Gif-Maker" SEC01
    ; Set output path to the installation directory
    SetOutPath $INSTDIR

    ; Add all files and subdirectories from your source folder
    File /r "C:\Users\Escritorio2\Documents\.code\twitter-gif-maker-fortress\.release\*.*"

    ; Create a desktop shortcut (optional)
    CreateShortCut "$DESKTOP\Twitter-Gif-Maker.lnk" "$INSTDIR\Twitter-Gif-Maker.exe"

    ; Optional: Add an uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"

    Exec "$INSTDIR\Twitter-Gif-Maker.exe"
SectionEnd

; Optional: Uninstaller section
Section "Uninstall"
    ; Remove installed files and shortcuts
    Delete "$INSTDIR\YourApp.exe"
    Delete "$SMPROGRAMS\YourAppName.lnk"
    Delete "$DESKTOP\YourAppName.lnk"

    ; Remove installation directory and all its contents
    RMDir /r "$INSTDIR"

    ; Remove uninstaller
    Delete "$INSTDIR\Uninstall.exe"
SectionEnd