############################################################################################
#      NSIS Installation Script created by NSIS Quick Setup Script Generator v1.09.18
#               Entirely Edited with NullSoft Scriptable Installation System                
#              by Vlasis K. Barkas aka Red Wine red_wine@freemail.gr Sep 2006               
############################################################################################

!define APP_NAME "JetiAppManager"
!define COMP_NAME "Nightflyer88"
!define WEB_SITE "https://github.com/nightflyer88"
!define VERSION "00.00.00.04"
!define COPYRIGHT "M. Lehmann (c) 2018"
!define DESCRIPTION "Application"
!define INSTALLER_NAME "C:\JetiAppManager\JetiAppManager_setup.exe"
!define MAIN_APP_EXE "JetiAppManager.exe"
!define INSTALL_TYPE "SetShellVarContext current"
!define REG_ROOT "HKCU"
!define REG_APP_PATH "Software\Microsoft\Windows\CurrentVersion\App Paths\${MAIN_APP_EXE}"
!define UNINSTALL_PATH "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

!define REG_START_MENU "Start Menu Folder"

var SM_Folder

######################################################################

VIProductVersion  "${VERSION}"
VIAddVersionKey "ProductName"  "${APP_NAME}"
VIAddVersionKey "CompanyName"  "${COMP_NAME}"
VIAddVersionKey "LegalCopyright"  "${COPYRIGHT}"
VIAddVersionKey "FileDescription"  "${DESCRIPTION}"
VIAddVersionKey "FileVersion"  "${VERSION}"

######################################################################

SetCompressor ZLIB
Name "${APP_NAME}"
Caption "${APP_NAME}"
OutFile "${INSTALLER_NAME}"
BrandingText "${APP_NAME}"
XPStyle on
InstallDirRegKey "${REG_ROOT}" "${REG_APP_PATH}" ""
InstallDir "$PROGRAMFILES\Jeti App Manager"

######################################################################

!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_UNABORTWARNING

!insertmacro MUI_PAGE_WELCOME

!ifdef LICENSE_TXT
!insertmacro MUI_PAGE_LICENSE "${LICENSE_TXT}"
!endif

!insertmacro MUI_PAGE_DIRECTORY

!ifdef REG_START_MENU
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "Jeti App Manager"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${REG_ROOT}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${UNINSTALL_PATH}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${REG_START_MENU}"
!insertmacro MUI_PAGE_STARTMENU Application $SM_Folder
!endif

!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_RUN "$INSTDIR\${MAIN_APP_EXE}"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM

!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

######################################################################

Section -MainProgram
${INSTALL_TYPE}
SetOverwrite ifnewer
SetOutPath "$INSTDIR"
File "C:\JetiAppManager\build_win\setup\d3dcompiler_47.dll"
File "C:\JetiAppManager\build_win\setup\JetiAppManager.exe"
File "C:\JetiAppManager\build_win\setup\libeay32.dll"
File "C:\JetiAppManager\build_win\setup\libEGL.dll"
File "C:\JetiAppManager\build_win\setup\libGLESV2.dll"
File "C:\JetiAppManager\build_win\setup\opengl32sw.dll"
File "C:\JetiAppManager\build_win\setup\OpenSSL License.txt"
File "C:\JetiAppManager\build_win\setup\Qt5Core.dll"
File "C:\JetiAppManager\build_win\setup\Qt5Gui.dll"
File "C:\JetiAppManager\build_win\setup\Qt5Network.dll"
File "C:\JetiAppManager\build_win\setup\Qt5Positioning.dll"
File "C:\JetiAppManager\build_win\setup\Qt5PrintSupport.dll"
File "C:\JetiAppManager\build_win\setup\Qt5Qml.dll"
File "C:\JetiAppManager\build_win\setup\Qt5Quick.dll"
File "C:\JetiAppManager\build_win\setup\Qt5QuickWidgets.dll"
File "C:\JetiAppManager\build_win\setup\Qt5SerialPort.dll"
File "C:\JetiAppManager\build_win\setup\Qt5Svg.dll"
File "C:\JetiAppManager\build_win\setup\Qt5WebChannel.dll"
File "C:\JetiAppManager\build_win\setup\Qt5WebEngineCore.dll"
File "C:\JetiAppManager\build_win\setup\Qt5WebEngineWidgets.dll"
File "C:\JetiAppManager\build_win\setup\Qt5Widgets.dll"
File "C:\JetiAppManager\build_win\setup\QtWebEngineProcess.exe"
File "C:\JetiAppManager\build_win\setup\ssleay32.dll"
SetOutPath "$INSTDIR\translations"
File "C:\JetiAppManager\build_win\setup\translations\qt_bg.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_ca.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_cs.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_da.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_de.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_en.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_es.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_fi.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_fr.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_gd.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_he.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_hu.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_it.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_ja.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_ko.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_lv.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_pl.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_ru.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_sk.qm"
File "C:\JetiAppManager\build_win\setup\translations\qt_uk.qm"
SetOutPath "$INSTDIR\translations\qtwebengine_locales"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\am.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\ar.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\bg.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\bn.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\ca.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\cs.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\da.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\de.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\el.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\en-GB.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\en-US.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\es-419.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\es.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\et.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\fa.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\fi.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\fil.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\fr.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\gu.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\he.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\hi.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\hr.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\hu.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\id.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\it.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\ja.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\kn.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\ko.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\lt.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\lv.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\ml.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\mr.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\ms.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\nb.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\nl.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\pl.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\pt-BR.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\pt-PT.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\ro.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\ru.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\sk.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\sl.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\sr.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\sv.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\sw.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\ta.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\te.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\th.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\tr.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\uk.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\vi.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\zh-CN.pak"
File "C:\JetiAppManager\build_win\setup\translations\qtwebengine_locales\zh-TW.pak"
SetOutPath "$INSTDIR\styles"
File "C:\JetiAppManager\build_win\setup\styles\qwindowsvistastyle.dll"
SetOutPath "$INSTDIR\resources"
File "C:\JetiAppManager\build_win\setup\resources\icudtl.dat"
File "C:\JetiAppManager\build_win\setup\resources\qtwebengine_devtools_resources.pak"
File "C:\JetiAppManager\build_win\setup\resources\qtwebengine_resources.pak"
File "C:\JetiAppManager\build_win\setup\resources\qtwebengine_resources_100p.pak"
File "C:\JetiAppManager\build_win\setup\resources\qtwebengine_resources_200p.pak"
SetOutPath "$INSTDIR\printsupport"
File "C:\JetiAppManager\build_win\setup\printsupport\windowsprintersupport.dll"
SetOutPath "$INSTDIR\position"
File "C:\JetiAppManager\build_win\setup\position\qtposition_geoclue.dll"
File "C:\JetiAppManager\build_win\setup\position\qtposition_positionpoll.dll"
File "C:\JetiAppManager\build_win\setup\position\qtposition_serialnmea.dll"
File "C:\JetiAppManager\build_win\setup\position\qtposition_winrt.dll"
SetOutPath "$INSTDIR\platforms"
File "C:\JetiAppManager\build_win\setup\platforms\qwindows.dll"
SetOutPath "$INSTDIR\imageformats"
File "C:\JetiAppManager\build_win\setup\imageformats\qgif.dll"
File "C:\JetiAppManager\build_win\setup\imageformats\qicns.dll"
File "C:\JetiAppManager\build_win\setup\imageformats\qico.dll"
File "C:\JetiAppManager\build_win\setup\imageformats\qjpeg.dll"
File "C:\JetiAppManager\build_win\setup\imageformats\qsvg.dll"
File "C:\JetiAppManager\build_win\setup\imageformats\qtga.dll"
File "C:\JetiAppManager\build_win\setup\imageformats\qtiff.dll"
File "C:\JetiAppManager\build_win\setup\imageformats\qwbmp.dll"
File "C:\JetiAppManager\build_win\setup\imageformats\qwebp.dll"
SetOutPath "$INSTDIR\iconengines"
File "C:\JetiAppManager\build_win\setup\iconengines\qsvgicon.dll"
SetOutPath "$INSTDIR\bearer"
File "C:\JetiAppManager\build_win\setup\bearer\qgenericbearer.dll"
File "C:\JetiAppManager\build_win\setup\bearer\qnativewifibearer.dll"
SectionEnd

######################################################################

Section -Icons_Reg
SetOutPath "$INSTDIR"
WriteUninstaller "$INSTDIR\uninstall.exe"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
CreateDirectory "$SMPROGRAMS\$SM_Folder"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!insertmacro MUI_STARTMENU_WRITE_END
!endif

!ifndef REG_START_MENU
CreateDirectory "$SMPROGRAMS\Jeti App Manager"
CreateShortCut "$SMPROGRAMS\Jeti App Manager\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\Jeti App Manager\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!endif

WriteRegStr ${REG_ROOT} "${REG_APP_PATH}" "" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayName" "${APP_NAME}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "UninstallString" "$INSTDIR\uninstall.exe"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayIcon" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayVersion" "${VERSION}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "Publisher" "${COMP_NAME}"

!ifdef WEB_SITE
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "URLInfoAbout" "${WEB_SITE}"
!endif
SectionEnd

######################################################################

Section Uninstall
${INSTALL_TYPE}
Delete "$INSTDIR\d3dcompiler_47.dll"
Delete "$INSTDIR\JetiAppManager.exe"
Delete "$INSTDIR\libeay32.dll"
Delete "$INSTDIR\libEGL.dll"
Delete "$INSTDIR\libGLESV2.dll"
Delete "$INSTDIR\opengl32sw.dll"
Delete "$INSTDIR\OpenSSL License.txt"
Delete "$INSTDIR\Qt5Core.dll"
Delete "$INSTDIR\Qt5Gui.dll"
Delete "$INSTDIR\Qt5Network.dll"
Delete "$INSTDIR\Qt5Positioning.dll"
Delete "$INSTDIR\Qt5PrintSupport.dll"
Delete "$INSTDIR\Qt5Qml.dll"
Delete "$INSTDIR\Qt5Quick.dll"
Delete "$INSTDIR\Qt5QuickWidgets.dll"
Delete "$INSTDIR\Qt5SerialPort.dll"
Delete "$INSTDIR\Qt5Svg.dll"
Delete "$INSTDIR\Qt5WebChannel.dll"
Delete "$INSTDIR\Qt5WebEngineCore.dll"
Delete "$INSTDIR\Qt5WebEngineWidgets.dll"
Delete "$INSTDIR\Qt5Widgets.dll"
Delete "$INSTDIR\QtWebEngineProcess.exe"
Delete "$INSTDIR\ssleay32.dll"
Delete "$INSTDIR\translations\qt_bg.qm"
Delete "$INSTDIR\translations\qt_ca.qm"
Delete "$INSTDIR\translations\qt_cs.qm"
Delete "$INSTDIR\translations\qt_da.qm"
Delete "$INSTDIR\translations\qt_de.qm"
Delete "$INSTDIR\translations\qt_en.qm"
Delete "$INSTDIR\translations\qt_es.qm"
Delete "$INSTDIR\translations\qt_fi.qm"
Delete "$INSTDIR\translations\qt_fr.qm"
Delete "$INSTDIR\translations\qt_gd.qm"
Delete "$INSTDIR\translations\qt_he.qm"
Delete "$INSTDIR\translations\qt_hu.qm"
Delete "$INSTDIR\translations\qt_it.qm"
Delete "$INSTDIR\translations\qt_ja.qm"
Delete "$INSTDIR\translations\qt_ko.qm"
Delete "$INSTDIR\translations\qt_lv.qm"
Delete "$INSTDIR\translations\qt_pl.qm"
Delete "$INSTDIR\translations\qt_ru.qm"
Delete "$INSTDIR\translations\qt_sk.qm"
Delete "$INSTDIR\translations\qt_uk.qm"
Delete "$INSTDIR\translations\qtwebengine_locales\am.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\ar.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\bg.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\bn.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\ca.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\cs.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\da.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\de.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\el.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\en-GB.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\en-US.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\es-419.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\es.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\et.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\fa.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\fi.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\fil.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\fr.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\gu.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\he.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\hi.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\hr.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\hu.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\id.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\it.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\ja.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\kn.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\ko.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\lt.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\lv.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\ml.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\mr.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\ms.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\nb.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\nl.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\pl.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\pt-BR.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\pt-PT.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\ro.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\ru.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\sk.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\sl.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\sr.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\sv.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\sw.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\ta.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\te.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\th.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\tr.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\uk.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\vi.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\zh-CN.pak"
Delete "$INSTDIR\translations\qtwebengine_locales\zh-TW.pak"
Delete "$INSTDIR\styles\qwindowsvistastyle.dll"
Delete "$INSTDIR\resources\icudtl.dat"
Delete "$INSTDIR\resources\qtwebengine_devtools_resources.pak"
Delete "$INSTDIR\resources\qtwebengine_resources.pak"
Delete "$INSTDIR\resources\qtwebengine_resources_100p.pak"
Delete "$INSTDIR\resources\qtwebengine_resources_200p.pak"
Delete "$INSTDIR\printsupport\windowsprintersupport.dll"
Delete "$INSTDIR\position\qtposition_geoclue.dll"
Delete "$INSTDIR\position\qtposition_positionpoll.dll"
Delete "$INSTDIR\position\qtposition_serialnmea.dll"
Delete "$INSTDIR\position\qtposition_winrt.dll"
Delete "$INSTDIR\platforms\qwindows.dll"
Delete "$INSTDIR\imageformats\qgif.dll"
Delete "$INSTDIR\imageformats\qicns.dll"
Delete "$INSTDIR\imageformats\qico.dll"
Delete "$INSTDIR\imageformats\qjpeg.dll"
Delete "$INSTDIR\imageformats\qsvg.dll"
Delete "$INSTDIR\imageformats\qtga.dll"
Delete "$INSTDIR\imageformats\qtiff.dll"
Delete "$INSTDIR\imageformats\qwbmp.dll"
Delete "$INSTDIR\imageformats\qwebp.dll"
Delete "$INSTDIR\iconengines\qsvgicon.dll"
Delete "$INSTDIR\bearer\qgenericbearer.dll"
Delete "$INSTDIR\bearer\qnativewifibearer.dll"
 
RmDir "$INSTDIR\bearer"
RmDir "$INSTDIR\iconengines"
RmDir "$INSTDIR\imageformats"
RmDir "$INSTDIR\platforms"
RmDir "$INSTDIR\position"
RmDir "$INSTDIR\printsupport"
RmDir "$INSTDIR\resources"
RmDir "$INSTDIR\styles"
RmDir "$INSTDIR\translations\qtwebengine_locales"
RmDir "$INSTDIR\translations"
 
Delete "$INSTDIR\uninstall.exe"
!ifdef WEB_SITE
Delete "$INSTDIR\${APP_NAME} website.url"
!endif

RmDir "$INSTDIR"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_GETFOLDER "Application" $SM_Folder
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk"
!endif
Delete "$DESKTOP\${APP_NAME}.lnk"

RmDir "$SMPROGRAMS\$SM_Folder"
!endif

!ifndef REG_START_MENU
Delete "$SMPROGRAMS\Jeti App Manager\${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\Jeti App Manager\${APP_NAME} Website.lnk"
!endif
Delete "$DESKTOP\${APP_NAME}.lnk"

RmDir "$SMPROGRAMS\Jeti App Manager"
!endif

DeleteRegKey ${REG_ROOT} "${REG_APP_PATH}"
DeleteRegKey ${REG_ROOT} "${UNINSTALL_PATH}"
SectionEnd

######################################################################

