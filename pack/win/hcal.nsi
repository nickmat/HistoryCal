;* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
;* Name:        pack/win/hcal.nsi
;* Project:     HistoryCal: Programmable Historical Calendar Calculator.
;* Purpose:     NSIS Windows installer script.
;* Author:      Nick Matthews
;* Website:     http://historycal.org
;* Created:     24th November 2014
;* Copyright:   Copyright (c) 2014-2015, Nick Matthews.
;* Licence:     GNU GPLv3
;*
;*  HistoryCal is free software: you can redistribute it and/or modify
;*  it under the terms of the GNU General Public License as published by
;*  the Free Software Foundation, either version 3 of the License, or
;*  (at your option) any later version.
;*
;*  HistoryCal is distributed in the hope that it will be useful,
;*  but WITHOUT ANY WARRANTY; without even the implied warranty of
;*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;*  GNU General Public License for more details.
;*
;*  You should have received a copy of the GNU General Public License
;*  along with HistoryCal.  If not, see <http://www.gnu.org/licenses/>.
;*
;* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


!include "MUI2.nsh"
!include "FileAssociation.nsh"

;--------------------------------

!define HCALNAME      "HistoryCal"
!define APP_EXE       "hcal.exe"
!define VERSION_DOT   "0.0.5.0"
!define VERSION_TXT   "V${VERSION_DOT} alpha"
!define LICENSE       "license.txt"
!define DESCRIPTION   "HistoryCal Application Installer"
!define COPYRIGHT     "2014 ~ 2015 @ Nick Matthews"

!define DOC_PATH      "$DOCUMENTS\${HCALNAME}"
!define EXAMPLE_PATH  "$DOCUMENTS\${HCALNAME}"

!define HIVE          "HKCU"
!define REG_APP       "Software\${HCALNAME}"
!define REG_UNINSTALL "Software\Microsoft\Windows\CurrentVersion\Uninstall\${HCALNAME}"

Name "${HCALNAME}"
OutFile "hcal-setup.exe"

VIProductVersion "${VERSION_DOT}"
VIAddVersionKey "ProductName" "${HCALNAME}"
VIAddVersionKey "CompanyName" "Nick Matthews"
VIAddVersionKey "LegalCopyright" "${COPYRIGHT}"
VIAddVersionKey "FileDescription" "${DESCRIPTION}"
VIAddVersionKey "FileVersion" "${VERSION_DOT}"
VIAddVersionKey "ProductVersion" "${VERSION_TXT}"

;Default installation folder
InstallDir "$PROGRAMFILES\${HCALNAME}"

;Get installation folder from registry if available
InstallDirRegKey ${HIVE} "${REG_APP}" ""

;Request application privileges
RequestExecutionLevel admin

;--------------------------------
;Variables

Var StartMenuFolder

;--------------------------------
;Pages

!insertmacro MUI_PAGE_WELCOME

!insertmacro MUI_PAGE_LICENSE "${LICENSE}"

!insertmacro MUI_PAGE_COMPONENTS

!insertmacro MUI_PAGE_DIRECTORY

!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${HIVE}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${REG_APP}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder

!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_RUN "$INSTDIR\${APP_EXE}"
!insertmacro MUI_PAGE_FINISH


!insertmacro MUI_UNPAGE_CONFIRM

!insertmacro MUI_UNPAGE_INSTFILES

!define MUI_FINISHPAGE_TEXT "The HistoryCal application has been uninstalled from your computer but example and user created script files have NOT be deleted. Use File Explorer to delete these files manually.$\r$\n$\r$\nThe HistoryCal files are normally found in the$\r$\n${DOC_PATH}$\r$\nfolder.$\r$\n$\r$\nClick Finish to close this wizard."
!insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Install Program" MainInstall

  SectionIn RO

  SetOutPath $INSTDIR

  ;Files to install
  File ${APP_EXE}
  File cal.dll
  File utf8api.dll
  File ${LICENSE}
  File cal-lib-changes.txt
  File hcal-changes.txt

  ;Store installation folder
  WriteRegStr ${HIVE} "${REG_APP}" "" $INSTDIR
  ;Uninstall data
  WriteRegStr ${HIVE} "${REG_UNINSTALL}" "DisplayName" "${HCALNAME}"
  WriteRegStr ${HIVE} "${REG_UNINSTALL}" "UninstallString" "$INSTDIR\uninstall.exe"
  WriteRegStr ${HIVE} "${REG_UNINSTALL}" "DisplayIcon" "$INSTDIR\${APP_EXE}"
  WriteRegStr ${HIVE} "${REG_UNINSTALL}" "DisplayVersion" "${VERSION_DOT}"
  WriteRegStr ${HIVE} "${REG_UNINSTALL}" "Publisher" "Nick Matthews"
  WriteRegDWORD ${HIVE} "${REG_UNINSTALL}" "NoModify" 1
  WriteRegDWORD ${HIVE} "${REG_UNINSTALL}" "NoRepair" 1

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  CreateDirectory "${DOC_PATH}"
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application

    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"

    SetOutPath "${DOC_PATH}"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\${HCALNAME}.lnk" "$INSTDIR\hcal.exe"
    SetOutPath $INSTDIR

    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_END

;  ${RegisterExtension} "$INSTDIR\${APP_EXE}" ".hcs" "${HCALNAME} Script"

SectionEnd

Section "Examples" InstallExamples

  CreateDirectory "${EXAMPLE_PATH}"
  SetOutPath      "${EXAMPLE_PATH}"

  File "day-counts.hcs"
  File "easter.hcs"
  File "html-format.hcs"
  File "Islamic-Ia.hcs"
  File "Islamic-Ic.hcs"
  File "Islamic-IIIc.hcs"
  File "Islamic-IVa.hcs"
  File "Islamic-IVc.hcs"
  File "misc-calc.hcs"
  File "today-is.hcs"

SectionEnd

;--------------------------------
;Section Descriptions

;Language strings
LangString DESC_MainInstall ${LANG_ENGLISH} "Install HistoryCal program."
LangString DESC_InstallExamples ${LANG_ENGLISH} "Install example files."

;Assign language strings to sections
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${MainInstall} $(DESC_MainInstall)
  !insertmacro MUI_DESCRIPTION_TEXT ${InstallExamples} $(DESC_InstallExamples)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...
  Delete "$INSTDIR\hcal.exe"
  Delete "cal.dll"
  Delete "utf8api.dll"
  Delete "$INSTDIR\license.txt"
  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder

  Delete "$SMPROGRAMS\$StartMenuFolder\${HCALNAME}.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"

  DeleteRegKey ${HIVE} "${REG_APP}"
  DeleteRegKey ${HIVE} "${REG_UNINSTALL}"

;  ${UnRegisterExtension} ".hcs" "${HCALNAME} Script"

SectionEnd
