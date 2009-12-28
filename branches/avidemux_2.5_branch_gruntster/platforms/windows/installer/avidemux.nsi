##########################
# Included files
##########################
!include Sections.nsh
!include MUI2.nsh
!include nsDialogs.nsh
!include Memento.nsh
!include FileFunc.nsh
!include WordFunc.nsh
!include ${NSIDIR}\revision.nsh

SetCompressor /SOLID lzma
SetCompressorDictSize 96

##########################
# Defines
##########################
!define INTERNALNAME "Avidemux 2.5"
!define REGKEY "SOFTWARE\${INTERNALNAME}"
!define UNINST_REGKEY "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${INTERNALNAME}"
!define VERSION 2.5.2.${REVISION}
!define COMPANY "Free Software Foundation"
!define URL "http://www.avidemux.org"

!ifndef INST_GTK
!ifndef INST_QT
!define INST_GTK
!define INST_QT
!endif
!endif

!ifdef INST_GTK
!ifdef INST_QT
!define INST_BOTH
!endif
!endif

!ifdef INST_BOTH
OutFile ${EXEDIR}\avidemux_2.5_r${REVISION}_full_win32.exe
Name "Avidemux 2.5.2 Full beta r${REVISION}"
!else ifdef INST_QT
OutFile ${EXEDIR}\avidemux_2.5_r${REVISION}_win32.exe
Name "Avidemux 2.5.2 beta r${REVISION}"
!else ifdef INST_GTK
OutFile ${EXEDIR}\avidemux_2.5_r${REVISION}_gtk_win32.exe
Name "Avidemux 2.5.2 GTK+ beta r${REVISION}"
!endif

##########################
# Memento defines
##########################
!define MEMENTO_REGISTRY_ROOT HKLM
!define MEMENTO_REGISTRY_KEY "${REGKEY}"

##########################
# MUI defines
##########################
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install-blue-full.ico"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_RIGHT
!ifdef INST_GTK
!define MUI_HEADERIMAGE_BITMAP "${NSIDIR}\PageHeaderGtk.bmp"
!else
!define MUI_HEADERIMAGE_BITMAP "${NSIDIR}\PageHeader.bmp"
!endif
!define MUI_STARTMENUPAGE_REGISTRY_ROOT HKLM
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${REGKEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME StartMenuGroup
!define MUI_STARTMENUPAGE_DEFAULTFOLDER Avidemux
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_WELCOMEFINISHPAGE_BITMAP "${NSIDIR}\WelcomeFinishStrip.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "${NSIDIR}\WelcomeFinishStrip.bmp"
!ifdef INST_GTK
!define MUI_UNICON "${NSIDIR}\..\..\..\avidemux\ADM_icons\xpm\adm.ico"
!else
!define MUI_UNICON "${NSIDIR}\..\..\..\avidemux\ADM_icons\xpm\avidemux.ico"
!endif
!define MUI_COMPONENTSPAGE_NODESC

##########################
# Variables
##########################
Var CreateDesktopIcon
Var CreateStartMenuGroup
Var CreateQuickLaunchIcon
Var StartMenuGroup
Var PreviousVersion
Var PreviousVersionState
Var ReinstallUninstall

##########################
# Installer pages
##########################
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "${NSIDIR}\License.rtf"
 Page custom ReinstallPage ReinstallPageLeave
!ifdef INST_BOTH
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE CheckSelectedUIs
!endif
!insertmacro MUI_PAGE_COMPONENTS
Page custom InstallOptionsPage
!define MUI_PAGE_CUSTOMFUNCTION_PRE IsStartMenuRequired
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuGroup
!insertmacro MUI_PAGE_DIRECTORY
!define MUI_PAGE_CUSTOMFUNCTION_PRE ActivateInternalSections
!define MUI_PAGE_CUSTOMFUNCTION_SHOW InstFilesPageShow
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE InstFilesPageLeave
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_FUNCTION RunAvidemux
!define MUI_FINISHPAGE_RUN_TEXT "Run ${INTERNALNAME} now"
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\Change Log.html"
!define MUI_FINISHPAGE_SHOWREADME_TEXT "View Change Log now"
!define MUI_FINISHPAGE_LINK "Visit the Avidemux Builds for Windows website"
!define MUI_FINISHPAGE_LINK_LOCATION "http://avidemux.razorbyte.com.au/"
!define MUI_PAGE_CUSTOMFUNCTION_PRE ConfigureFinishPage
!insertmacro MUI_PAGE_FINISH

!define MUI_PAGE_CUSTOMFUNCTION_PRE un.ConfirmPagePre
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!define MUI_PAGE_CUSTOMFUNCTION_PRE un.FinishPagePre
!insertmacro MUI_UNPAGE_FINISH

##########################
# Installer languages
##########################
!insertmacro MUI_LANGUAGE English

##########################
# Installer attributes
##########################
InstallDir "$PROGRAMFILES\${INTERNALNAME}"
CRCCheck on
XPStyle on
ShowInstDetails nevershow
ShowUninstDetails nevershow
VIProductVersion 2.5.2.${REVISION}
VIAddVersionKey ProductName Avidemux
VIAddVersionKey ProductVersion "${VERSION} (beta)"
VIAddVersionKey FileVersion ""
VIAddVersionKey FileDescription ""
VIAddVersionKey LegalCopyright ""
InstallDirRegKey HKLM "${REGKEY}" Path
BrandingText "Packaged by Gruntster"
InstType Standard
InstType Full

##########################
# Uninstaller macros
##########################
!insertmacro un.GetOptions
!insertmacro un.GetParameters

!define UninstallLogPath "$INSTDIR\uninstall.log"
Var UninstallLogHandle

; Uninstall log file missing.
LangString UninstallLogMissing ${LANG_ENGLISH} "uninstall.log not found!$\r$\nUninstallation cannot proceed!"

!macro InstallFile FILEREGEX
	File "${FILEREGEX}"
	!define Index 'Line${__LINE__}'
	${GetFileName} "${FILEREGEX}" $R0
	FindFirst $0 $1 "$OUTDIR\$R0"
	StrCmp $0 "" "${Index}-End"
"${Index}-Loop:"
	StrCmp $1 "" "${Index}-End"
	FileWrite $UninstallLogHandle "$OUTDIR\$1$\r$\n"
	FindNext $0 $1
	Goto "${Index}-Loop"
"${Index}-End:"
	!undef Index
!macroend
!define File "!insertmacro InstallFile"
 
!macro InstallFolder FILEREGEX
	File /r "${FILEREGEX}\*"
	Push "$OUTDIR"
	Call InstallFolderInternal
!macroend
!define Folder "!insertmacro InstallFolder"
 
Function InstallFolderInternal
	Pop $9
	!define Index 'Line${__LINE__}'
	FindFirst $0 $1 "$9\*"
	StrCmp $0 "" "${Index}-End"
"${Index}-Loop:"
	StrCmp $1 "" "${Index}-End"
	StrCmp $1 "." "${Index}-Next"
	StrCmp $1 ".." "${Index}-Next"
	IfFileExists "$9\$1\*" 0 "${Index}-Write"
		Push $0
		Push $9
		Push "$9\$1"
		Call InstallFolderInternal
		Pop $9
		Pop $0
		Goto "${Index}-Next"
"${Index}-Write:"
	FileWrite $UninstallLogHandle "$9\$1$\r$\n"
"${Index}-Next:"
	FindNext $0 $1
	Goto "${Index}-Loop"
"${Index}-End:"
	!undef Index
FunctionEnd

; WriteUninstaller macro
!macro WriteUninstaller Path
	WriteUninstaller "${Path}"
	FileWrite $UninstallLogHandle "${Path}$\r$\n"
!macroend
!define WriteUninstaller "!insertmacro WriteUninstaller"

##########################
# Macros
##########################
!macro InstallGtkLanguage LANG_NAME LANG_CODE
!ifdef INST_GTK
	SetOverwrite on

	!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} installGtk${LANG_CODE} endGtk${LANG_CODE}

installGtk${LANG_CODE}:
    SetOutPath $INSTDIR\share\locale\${LANG_CODE}\LC_MESSAGES
    ${File} share\locale\${LANG_CODE}\LC_MESSAGES\avidemux.mo
    ${File} share\locale\${LANG_CODE}\LC_MESSAGES\gtk20.mo

endGtk${LANG_CODE}:
!endif
!macroend

!macro InstallQtLanguage LANG_NAME LANG_CODE
!ifdef INST_QT
	SetOverwrite on

	!insertmacro SectionFlagIsSet ${SecUiQt} ${SF_SELECTED} installQt${LANG_CODE} endQt${LANG_CODE}

installQt${LANG_CODE}:
	SetOutPath $INSTDIR\i18n
    ${File} i18n\avidemux_${LANG_CODE}.qm
    ${File} i18n\qt_${LANG_CODE}.qm

endQt${LANG_CODE}:
!endif
!macroend

##########################
# Installer sections
##########################
Section -OpenLogFile
	CreateDirectory "$INSTDIR"
	FileOpen $UninstallLogHandle ${UninstallLogPath} a
	FileSeek $UninstallLogHandle 0 END
SectionEnd

Section "Avidemux Core" SecCore
    SectionIn 1 2 RO
    SetOutPath $INSTDIR
    SetOverwrite on
    ${File} "Build Info.txt"
    ${File} "Change Log.html"
    ${File} zlib1.dll
    ${File} freetype6.dll
    ${File} libjs.dll
    ${File} libnspr4.dll
    ${File} libADM_core.dll
    ${File} libADM_coreAudio.dll
    ${File} libADM_coreImage.dll
    ${File} libADM_coreUI.dll
    ${File} libaften.dll
    ${File} libxml2-*.dll
    ${File} ogg.dll
    ${File} pthreadGC2.dll
    ${File} SDL.dll
    ${File} AUTHORS.
    ${File} COPYING.
    ${File} README.
    ${File} avcodec-*.dll
    ${File} avformat-*.dll
    ${File} avutil-*.dll
    ${File} postproc-*.dll
    ${File} swscale-*.dll
    ${File} libgcc_sjlj_*.dll
    ${File} libstdc++_sjlj_*.dll
    SetOutPath $INSTDIR\scripts
    ${Folder} scripts
SectionEnd

SectionGroup /e "User interfaces" SecGrpUI
    ${MementoUnselectedSection} "Command line" SecUiCli
        SectionIn 1 2
        SetOutPath $INSTDIR
        SetOverwrite on
        ${File} avidemux2_cli.exe
        ${File} libADM_render_cli.dll
        ${File} libADM_UICli.dll
    ${MementoSectionEnd}

!ifdef INST_BOTH
	${MementoUnselectedSection} GTK+ SecUiGtk
	SectionIn 2
!else ifdef INST_GTK
	${MementoSection} GTK+ SecUiGtk
	SectionIn 1 2 RO
!endif
!ifdef INST_BOTH | INST_GTK
        SetOverwrite on
        SetOutPath $INSTDIR\etc\gtk-2.0
        ${Folder} etc\gtk-2.0
        SetOutPath $INSTDIR\etc\pango
        ${Folder} etc\pango
        SetOutPath $INSTDIR\lib\gtk-2.0
        ${Folder} lib\gtk-2.0
        SetOutPath $INSTDIR\share\themes
        ${Folder} share\themes
        SetOutPath $INSTDIR
        ${File} avidemux2_gtk.exe
        ${File} gtk2_prefs.exe
        ${File} libADM_render_gtk.dll
        ${File} libADM_UIGtk.dll
        ${File} libatk-1.0-0.dll
        ${File} libcairo-2.dll
        ${File} libgdk_pixbuf-2.0-0.dll
        ${File} libgdk-win32-2.0-0.dll
        ${File} libgio-2.0-0.dll
        ${File} libglib-2.0-0.dll
        ${File} libgmodule-2.0-0.dll
        ${File} libgobject-2.0-0.dll
        ${File} libgthread-2.0-0.dll
        ${File} libgtk-win32-2.0-0.dll
        ${File} libpango-1.0-0.dll
        ${File} libpangocairo-1.0-0.dll
        ${File} libpangoft2-1.0-0.dll
        ${File} libpangowin32-1.0-0.dll
        ${File} libpng12-0.dll
    ${MementoSectionEnd}
!endif

!ifdef INST_QT
    ${MementoSection} Qt SecUiQt
!ifdef INST_BOTH
        SectionIn 1 2
!else
		SectionIn 1 2 RO
!endif
        SetOutPath $INSTDIR
        SetOverwrite on
        ${File} QtGui4.dll
        ${File} avidemux2_qt4.exe
        ${File} libADM_render_qt4.dll
        ${File} libADM_UIQT4.dll
        ${File} mingwm10.dll
        ${File} QtCore4.dll
    ${MementoSectionEnd}
!endif
SectionGroupEnd

SectionGroup Plugins SecGrpPlugin
	SectionGroup "Audio Decoders" SecGrpAudioDecoder
		${MementoSection} "FAAD2 (AAC)" SecAudDecFaad
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\audioDecoder
			${File} plugins\audioDecoder\libADM_ad_faad.dll
			SetOutPath $INSTDIR
			${File} libfaad2.dll
		${MementoSectionEnd}
		${MementoSection} "liba52 (AC-3)" SecAudDecA52
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\audioDecoder
			${File} plugins\audioDecoder\libADM_ad_a52.dll
		${MementoSectionEnd}
		${MementoSection} "libvorbis (Vorbis)" SecAudDecVorbis
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\audioDecoder
			${File} plugins\audioDecoder\libADM_ad_vorbis.dll
		${MementoSectionEnd}
		${MementoSection} "MAD (MPEG)" SecAudDecMad
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\audioDecoder
			${File} plugins\audioDecoder\libADM_ad_Mad.dll
		${MementoSectionEnd}
		${MementoSection} "opencore-amrnb (AMR-NB)" SecAudDecOpencoreAmrNb
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\audioDecoder
			${File} plugins\audioDecoder\libADM_ad_opencore_amrnb.dll
			SetOutPath $INSTDIR
			${File} libopencore-amrnb-*.dll
		${MementoSectionEnd}
		${MementoSection} "opencore-amrwb (AMR-WB)" SecAudDecOpencoreAmrWb
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\audioDecoder
			${File} plugins\audioDecoder\libADM_ad_opencore_amrwb.dll
			SetOutPath $INSTDIR
			${File} libopencore-amrwb-*.dll
		${MementoSectionEnd}
	SectionGroupEnd
	SectionGroup "Audio Devices" SecGrpAudioDevice
		${MementoUnselectedSection} SDL SecAudDevSdl
			SectionIn 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\audioDevices
			${File} plugins\audioDevices\libADM_av_sdl.dll
		${MementoSectionEnd}
		${MementoSection} "MS Windows (Waveform)" SecAudDevWaveform
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\audioDevices
			${File} plugins\audioDevices\libADM_av_win32.dll
		${MementoSectionEnd}
	SectionGroupEnd
	SectionGroup "Audio Encoders" SecGrpAudioEncoder
		${MementoSection} "Aften (AC-3)" SecAudDecAften
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\audioEncoders
			${File} plugins\audioEncoders\libADM_ae_aften.dll
			SetOutPath $INSTDIR
			${File} libaften.dll
		${MementoSectionEnd}
		${MementoSection} "FAAC (AAC)" SecAudEncFaac
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\audioEncoders
			${File} plugins\audioEncoders\libADM_ae_faac.dll
			SetOutPath $INSTDIR
			${File} libfaac.dll
		${MementoSectionEnd}
		${MementoSection} "LAME (MP3)" SecAudEncLame
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\audioEncoders
			${File} plugins\audioEncoders\libADM_ae_lame.dll
			SetOutPath $INSTDIR
			${File} libmp3lame-0.dll
		${MementoSectionEnd}
		${MementoSection} "libavcodec (AC-3)" SecAudEncLavAc3
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\audioEncoders
			${File} plugins\audioEncoders\libADM_ae_lav_ac3.dll
		${MementoSectionEnd}
		${MementoSection} "libavcodec (MP2)" SecAudEncLavMp2
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\audioEncoders
			${File} plugins\audioEncoders\libADM_ae_lav_mp2.dll
		${MementoSectionEnd}
		${MementoSection} "libvorbis (Vorbis)" SecAudEncVorbis
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\audioEncoders
			${File} plugins\audioEncoders\libADM_ae_vorbis.dll
			SetOutPath $INSTDIR
			${File} vorbis.dll
			${File} vorbisenc.dll
		${MementoSectionEnd}
		${MementoSection} "PCM" SecAudEncPcm
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\audioEncoders
			${File} plugins\audioEncoders\libADM_ae_pcm.dll
		${MementoSectionEnd}
		${MementoSection} "TwoLAME (MP2)" SecAudEncTwoLame
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\audioEncoders
			${File} plugins\audioEncoders\libADM_ae_twolame.dll
		${MementoSectionEnd}
	SectionGroupEnd
	SectionGroup "Video Encoders" SecGrpVideoEncoder
		${MementoSection} "avcodec (multiple encoders)" SecVidEncAvcodec
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\videoEncoder
			${File} plugins\videoEncoder\libADM_vidEnc_avcodec.dll
			SetOutPath $INSTDIR\plugins\videoEncoder\avcodec
			${File} plugins\videoEncoder\avcodec\Mpeg1Param.xsd
			SetOutPath $INSTDIR\plugins\videoEncoder\avcodec\mpeg-1
			${File} plugins\videoEncoder\avcodec\mpeg-1\*.xml
		${MementoSectionEnd}
		${MementoSection} "x264 (MPEG-4 AVC)" SecVidEncX264
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\videoEncoder
			${File} plugins\videoEncoder\libADM_vidEnc_x264.dll
			SetOutPath $INSTDIR\plugins\videoEncoder\x264
!ifdef INST_GTK
			!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt
InstallGtk:
			${File} plugins\videoEncoder\x264\libADM_vidEnc_x264_Gtk.dll
CheckQt:
!endif
!ifdef INST_QT
			!insertmacro SectionFlagIsSet ${SecUiQt} ${SF_SELECTED} InstallQt End
InstallQt:
			${File} plugins\videoEncoder\x264\libADM_vidEnc_x264_Qt.dll
End:
!endif
			${File} plugins\videoEncoder\x264\x264Param.xsd
			${File} plugins\videoEncoder\x264\*.xml
			SetOutPath $INSTDIR
			${File} libx264-*.dll
		${MementoSectionEnd}
		${MementoSection} "Xvid (MPEG-4 ASP)" SecVidEncXvid
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\plugins\videoEncoder
			${File} plugins\videoEncoder\libADM_vidEnc_xvid.dll
			SetOutPath $INSTDIR\plugins\videoEncoder\xvid
!ifdef INST_GTK
			!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt
InstallGtk:
			${File} plugins\videoEncoder\xvid\libADM_vidEnc_Xvid_Gtk.dll
CheckQt:
!endif
!ifdef INST_QT
			!insertmacro SectionFlagIsSet ${SecUiQt} ${SF_SELECTED} InstallQt End
InstallQt:
			${File} plugins\videoEncoder\xvid\libADM_vidEnc_Xvid_Qt.dll
End:
!endif
			${File} plugins\videoEncoder\xvid\XvidParam.xsd
			#${File} plugins\videoEncoder\xvid\*.xml
			SetOutPath $INSTDIR
			${File} xvidcore.dll
		${MementoSectionEnd}
	SectionGroupEnd
	SectionGroup "Video Filters" SecGrpVideoFilter
		SectionGroup "Transform Filters" SecGrpVideoFilterTransform
			${MementoSection} "Add Black Borders" SecVidFltBlackBorders
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_addborders.dll
			${MementoSectionEnd}
			${MementoSection} "Avisynth Resize" SecVidFltAvisynthResize
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				!insertmacro SectionFlagIsSet ${SecUiCli} ${SF_SELECTED} InstallCli CheckGtk
InstallCli:
				${File} plugins\videoFilter\libADM_vf_avisynthResize_cli.dll
CheckGtk:
!ifdef INST_GTK
				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt
InstallGtk:
				${File} plugins\videoFilter\libADM_vf_avisynthResize_gtk.dll
CheckQt:
!endif
!ifdef INST_QT
				!insertmacro SectionFlagIsSet ${SecUiQt} ${SF_SELECTED} InstallQt End
InstallQt:
				${File} plugins\videoFilter\libADM_vf_avisynthResize_qt4.dll
End:
!endif
			${MementoSectionEnd}
			${MementoSection} "Blacken Borders" SecVidFltBlackenBorders
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_blackenBorders.dll
			${MementoSectionEnd}
			${MementoSection} "Crop" SecVidFltCrop
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				!insertmacro SectionFlagIsSet ${SecUiCli} ${SF_SELECTED} InstallCli CheckGtk
InstallCli:
				${File} plugins\videoFilter\libADM_vf_crop_cli.dll
CheckGtk:
!ifdef INST_GTK
				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt
InstallGtk:
				${File} plugins\videoFilter\libADM_vf_Crop_gtk.dll
CheckQt:
!endif
!ifdef INST_QT
				!insertmacro SectionFlagIsSet ${SecUiQt} ${SF_SELECTED} InstallQt End
InstallQt:
				${File} plugins\videoFilter\libADM_vf_crop_qt4.dll
End:
!endif
			${MementoSectionEnd}
			${MementoSection} "Fade" SecVidFltFade
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_fade.dll
			${MementoSectionEnd}
			${MementoSection} "MPlayer Resize" SecVidFltMplayerResize
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				!insertmacro SectionFlagIsSet ${SecUiCli} ${SF_SELECTED} InstallCli CheckGtk
InstallCli:
				${File} plugins\videoFilter\libADM_vf_mplayerResize_cli.dll
CheckGtk:
!ifdef INST_GTK
				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt
InstallGtk:
				${File} plugins\videoFilter\libADM_vf_mplayerResize_gtk.dll
CheckQt:
!endif
!ifdef INST_QT
				!insertmacro SectionFlagIsSet ${SecUiQt} ${SF_SELECTED} InstallQt End
InstallQt:
				${File} plugins\videoFilter\libADM_vf_mplayerResize_qt4.dll
End:
!endif
			${MementoSectionEnd}
			${MementoSection} "Resample FPS" SecVidFltResampleFps
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_resampleFps.dll
			${MementoSectionEnd}
			${MementoSection} "Reverse" SecVidFltReverse
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_reverse.dll
			${MementoSectionEnd}
			${MementoSection} "Rotate" SecVidFltRotate
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_rotate.dll
			${MementoSectionEnd}
			${MementoSection} "Vertical Flip" SecVidFltVerticalFlip
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_vflip.dll
			${MementoSectionEnd}
		SectionGroupEnd
		SectionGroup "Interlacing Filters" SecGrpVideoFilterInterlacing
			${MementoSection} "Decomb Decimate" SecVidFltDecombDecimate
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_decimate.dll
			${MementoSectionEnd}
			${MementoSection} "Decomb Telecide" SecVidFltDecombTelecide
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_telecide.dll
			${MementoSectionEnd}
			${MementoSection} "Deinterlace" SecVidFltDeinterlace
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_Deinterlace.dll
			${MementoSectionEnd}
			${MementoSection} "DG Bob" SecVidFltDbGob
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_blendDgBob.dll
			${MementoSectionEnd}
			${MementoSection} "Drop" SecVidFltDrop
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_dropOut.dll
			${MementoSectionEnd}
			${MementoSection} "Fade" SecVidFltFadeInterlace
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_separateField.dll
			${MementoSectionEnd}
			${MementoSection} "Gauss Smooth" SecVidFltGaussSmooth
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_fastconvolutiongauss.dll
			${MementoSectionEnd}
			${MementoSection} "Keep Even Fields" SecVidFltKeepEvenFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_keepEvenField.dll
			${MementoSectionEnd}
			${MementoSection} "Keep Odd Fields" SecVidFltKeepOddFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_keepOddField.dll
			${MementoSectionEnd}
			${MementoSection} "KernelDeint" SecVidFltKernelDeint
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_kernelDeint.dll
			${MementoSectionEnd}
			${MementoSection} "libavcodec Deinterlacer" SecVidFltLavDeinterlacer
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_lavDeinterlace.dll
			${MementoSectionEnd}
			${MementoSection} "mcDeint" SecVidFltMcDeint
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_mcdeint.dll
			${MementoSectionEnd}
			${MementoSection} "Mean" SecVidFltMean
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_fastconvolutionmean.dll
			${MementoSectionEnd}
			${MementoSection} "Median" SecVidFltMedian
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_fastconvolutionmedian.dll
			${MementoSectionEnd}
			${MementoSection} "Merge Fields" SecVidFltMergeFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_mergeField.dll
			${MementoSectionEnd}
			${MementoSection} "PAL Field Shift" SecVidFltPalFieldShift
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_palShift.dll
			${MementoSectionEnd}
			${MementoSection} "PAL Smart Field Shift" SecVidFltPalSmartFieldShift
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_smartPalShift.dll
			${MementoSectionEnd}
			${MementoSection} "Pulldown" SecVidFltPulldown
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_Pulldown.dll
			${MementoSectionEnd}
			${MementoSection} "Separate Fields" SecVidFltSeparateFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_hzStackField.dll
			${MementoSectionEnd}
			${MementoSection} "Sharpen" SecVidFltSharpen
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_fastconvolutionsharpen.dll
			${MementoSectionEnd}
			${MementoSection} "Smart Swap Fields" SecVidFltSmartSwapFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_smartSwapField.dll
			${MementoSectionEnd}
			${MementoSection} "Stack Fields" SecVidFltStackFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_stackField.dll
			${MementoSectionEnd}
			${MementoSection} "Swap Fields" SecVidFltSwapFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_swapField.dll
			${MementoSectionEnd}
			${MementoSection} "TDeint" SecVidFltTDeint
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_tdeint.dll
			${MementoSectionEnd}
			${MementoSection} "Unstack Fields" SecVidFltUnstackFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_unstackField.dll
			${MementoSectionEnd}
			${MementoSection} "yadif" SecVidFltYadif
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_yadif.dll
			${MementoSectionEnd}
		SectionGroupEnd
		SectionGroup "Colour Filters" SecGrpVideoFilterColour
			${MementoSection} "Avisynth ColorYUV" SecVidFltAvisynthColorYuv
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				!insertmacro SectionFlagIsSet ${SecUiCli} ${SF_SELECTED} InstallCli CheckGtk
InstallCli:
				${File} plugins\videoFilter\libADM_vf_colorYUV_cli.dll
CheckGtk:
!ifdef INST_GTK
				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt
InstallGtk:
				${File} plugins\videoFilter\libADM_vf_colorYUV_gtk.dll
CheckQt:
!endif
!ifdef INST_QT
				!insertmacro SectionFlagIsSet ${SecUiQt} ${SF_SELECTED} InstallQt End
InstallQt:
				${File} plugins\videoFilter\libADM_vf_colorYUV_qt4.dll
End:
!endif
			${MementoSectionEnd}
			${MementoSection} "Blend Removal" SecVidFltBlendRemoval
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_blendRemoval.dll
			${MementoSectionEnd}
			${MementoSection} "Chroma Shift" SecVidFltChromaShift
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				!insertmacro SectionFlagIsSet ${SecUiCli} ${SF_SELECTED} InstallCli CheckGtk
InstallCli:
				${File} plugins\videoFilter\libADM_vf_chromashift_cli.dll
CheckGtk:
!ifdef INST_GTK
				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt
InstallGtk:
				${File} plugins\videoFilter\libADM_vf_chromaShift_gtk.dll
CheckQt:
!endif
!ifdef INST_QT
				!insertmacro SectionFlagIsSet ${SecUiQt} ${SF_SELECTED} InstallQt End
InstallQt:
				${File} plugins\videoFilter\libADM_vf_chromaShift_qt4.dll
End:
!endif
			${MementoSectionEnd}
			${MementoSection} "Chroma U" SecVidFltChromaU
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vidChromaU.dll
			${MementoSectionEnd}
			${MementoSection} "Chroma V" SecVidFltChromaV
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vidChromaV.dll
			${MementoSectionEnd}
!ifdef INST_QT
			${MementoSection} "Colour Curve Editor" SecVidFltColourCurveEditor
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_curveEditor_qt4.dll
			${MementoSectionEnd}
!endif
			${MementoSection} "Contrast" SecVidFltContrast
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				!insertmacro SectionFlagIsSet ${SecUiCli} ${SF_SELECTED} InstallCli CheckGtk
InstallCli:
				${File} plugins\videoFilter\libADM_vf_contrast_cli.dll
CheckGtk:
!ifdef INST_GTK
				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt
InstallGtk:
				${File} plugins\videoFilter\libADM_vf_contrast_gtk.dll
CheckQt:
!endif
!ifdef INST_QT
				!insertmacro SectionFlagIsSet ${SecUiQt} ${SF_SELECTED} InstallQt End
InstallQt:
				${File} plugins\videoFilter\libADM_vf_contrast_qt4.dll
End:
!endif
			${MementoSectionEnd}
			${MementoSection} "Luma Delta" SecVidFltLumaDelta
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_Delta.dll
			${MementoSectionEnd}
			${MementoSection} "Luma Equaliser" SecVidFltLumaEqualiser
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				!insertmacro SectionFlagIsSet ${SecUiCli} ${SF_SELECTED} InstallCli CheckGtk
InstallCli:
				${File} plugins\videoFilter\libADM_vf_equalizer_cli.dll
CheckGtk:
!ifdef INST_GTK
				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt
InstallGtk:
				${File} plugins\videoFilter\libADM_vf_equalizer_gtk.dll
CheckQt:
!endif
!ifdef INST_QT
				!insertmacro SectionFlagIsSet ${SecUiQt} ${SF_SELECTED} InstallQt End
InstallQt:
				${File} plugins\videoFilter\libADM_vf_equalizer_qt4.dll
End:
!endif
			${MementoSectionEnd}
			${MementoSection} "Luma Only" SecVidFltLumaOnly
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_lumaonly.dll
			${MementoSectionEnd}
			${MementoSection} "Mplayer Eq2" SecVidFltMPlayerEq2
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				!insertmacro SectionFlagIsSet ${SecUiCli} ${SF_SELECTED} InstallCli CheckGtk
InstallCli:
				${File} plugins\videoFilter\libADM_vf_eq2_cli.dll
CheckGtk:
!ifdef INST_GTK
				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt
InstallGtk:
				${File} plugins\videoFilter\libADM_vf_eq2_gtk.dll
CheckQt:
!endif
!ifdef INST_QT
				!insertmacro SectionFlagIsSet ${SecUiQt} ${SF_SELECTED} InstallQt End
InstallQt:
				${File} plugins\videoFilter\libADM_vf_eq2_qt4.dll
End:
!endif
			${MementoSectionEnd}
			${MementoSection} "Mplayer Hue" SecVidFltMPlayerHue
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				!insertmacro SectionFlagIsSet ${SecUiCli} ${SF_SELECTED} InstallCli CheckGtk
InstallCli:
				${File} plugins\videoFilter\libADM_vf_Hue_cli.dll
CheckGtk:
!ifdef INST_GTK
				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt
InstallGtk:
				${File} plugins\videoFilter\libADM_vf_hue_gtk.dll
CheckQt:
!endif
!ifdef INST_QT
				!insertmacro SectionFlagIsSet ${SecUiQt} ${SF_SELECTED} InstallQt End
InstallQt:
				${File} plugins\videoFilter\libADM_vf_hue_qt4.dll
End:
!endif
			${MementoSectionEnd}
			${MementoSection} "Swap U and V" SecVidFltSwapUandV
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_swapuv.dll
			${MementoSectionEnd}
		SectionGroupEnd
		SectionGroup "Noise Filters" SecGrpVideoFilterNoise
			${MementoSection} "Cnr2" SecVidFltCnr2
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				!insertmacro SectionFlagIsSet ${SecUiCli} ${SF_SELECTED} InstallCli CheckGtk
InstallCli:
				${File} plugins\videoFilter\libADM_vf_cnr2_cli.dll
CheckGtk:
!ifdef INST_GTK
				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt
InstallGtk:
				${File} plugins\videoFilter\libADM_vf_cnr2_gtk.dll
CheckQt:
!endif
!ifdef INST_QT
				!insertmacro SectionFlagIsSet ${SecUiQt} ${SF_SELECTED} InstallQt End
InstallQt:
				${File} plugins\videoFilter\libADM_vf_cnr2_qt4.dll
End:
!endif
			${MementoSectionEnd}
			${MementoSection} "Denoise" SecVidFltDenoise
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_Denoise.dll
			${MementoSectionEnd}
			${MementoSection} "FluxSmooth" SecVidFltFluxSmooth
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_FluxSmooth.dll
			${MementoSectionEnd}
			${MementoSection} "Forced Post-processing" SecVidFltForcedPostProcessing
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_forcedPP.dll
			${MementoSectionEnd}
			${MementoSection} "Light Denoiser" SecVidFltLightDenoiser
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_Stabilize.dll
			${MementoSectionEnd}
			${MementoSection} "Median (5x5)" SecVidFltMediam5x5
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_largemedian.dll
			${MementoSectionEnd}
			${MementoSection} "MPlayer Denoise3d" SecVidFltMPlayerDenoise3d
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_denoise3d.dll
			${MementoSectionEnd}
			${MementoSection} "MPlayer Hqdn3d" SecVidFltMPlayerHqdn3d
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_denoise3dhq.dll
			${MementoSectionEnd}
			${MementoSection} "Temporal Cleaner" SecVidFltTemporalCleaner
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_vlad.dll
			${MementoSectionEnd}
			${MementoSection} "TIsophote" SecVidFltTisophote
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_Tisophote.dll
			${MementoSectionEnd}
		SectionGroupEnd
		SectionGroup "Sharpness Filters" SecGrpVideoFilterSharpness
			${MementoSection} "asharp" SecVidFltAsharp
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				!insertmacro SectionFlagIsSet ${SecUiCli} ${SF_SELECTED} InstallCli CheckGtk
InstallCli:
				${File} plugins\videoFilter\libADM_vf_asharp_cli.dll
CheckGtk:
!ifdef INST_GTK
				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt
InstallGtk:
				${File} plugins\videoFilter\libADM_vf_asharp_gtk.dll
CheckQt:
!endif
!ifdef INST_QT
				!insertmacro SectionFlagIsSet ${SecUiQt} ${SF_SELECTED} InstallQt End
InstallQt:
				${File} plugins\videoFilter\libADM_vf_asharp_qt4.dll
End:
!endif
			${MementoSectionEnd}
			${MementoSection} "MSharpen" SecVidFltMSharpen
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_mSharpen.dll
			${MementoSectionEnd}
			${MementoSection} "MSmooth" SecVidFltMSmooth
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_mSmooth.dll
			${MementoSectionEnd}
			${MementoSection} "Soften" SecVidFltSoften
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_soften.dll
			${MementoSectionEnd}
		SectionGroupEnd
		SectionGroup "Subtitle Filters" SecGrpVideoFilterSubtitle
			${MementoSection} "ASS/SSA" SecVidFltAssSsa
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_ssa.dll
				SetOutPath $INSTDIR
				${File} libfontconfig-1.dll
				SetOutPath $INSTDIR\etc\fonts
				${Folder} etc\fonts
			${MementoSectionEnd}
			${MementoSection} "Subtitler" SecVidFltSubtitler
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				!insertmacro SectionFlagIsSet ${SecUiCli} ${SF_SELECTED} InstallCli CheckGtk
InstallCli:
				${File} plugins\videoFilter\libADM_vf_sub_cli.dll
CheckGtk:
!ifdef INST_GTK
				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt
InstallGtk:
				${File} plugins\videoFilter\libADM_vf_sub_gtk.dll
CheckQt:
!endif
!ifdef INST_QT
				!insertmacro SectionFlagIsSet ${SecUiQt} ${SF_SELECTED} InstallQt End
InstallQt:
				${File} plugins\videoFilter\libADM_vf_sub_qt4.dll
End:
!endif
			${MementoSectionEnd}
		SectionGroupEnd
		SectionGroup "Miscellaneous Filters" SecGrpVideoFilterMiscellaneous
			${MementoSection} "Mosaic" SecVidFltMosaic
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_Mosaic.dll
			${MementoSectionEnd}
			${MementoSection} "MPlayer Delogo" SecVidFltMPlayerDelogo
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				!insertmacro SectionFlagIsSet ${SecUiCli} ${SF_SELECTED} InstallCli CheckGtk
InstallCli:
				${File} plugins\videoFilter\libADM_vf_mpdelogo_cli.dll
CheckGtk:
!ifdef INST_GTK
				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt
InstallGtk:
				${File} plugins\videoFilter\libADM_vf_mpdelogo_gtk.dll
CheckQt:
!endif
!ifdef INST_QT
				!insertmacro SectionFlagIsSet ${SecUiQt} ${SF_SELECTED} InstallQt End
InstallQt:
				${File} plugins\videoFilter\libADM_vf_mpdelogo_qt4.dll
End:
!endif
			${MementoSectionEnd}
			${MementoSection} "Whirl" SecVidFltWhirl
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\plugins\videoFilter
				${File} plugins\videoFilter\libADM_vf_Whirl.dll
			${MementoSectionEnd}
		SectionGroupEnd
	SectionGroupEnd
SectionGroupEnd

SectionGroup "Additional languages" SecGrpLang
!ifdef INST_BOTH | INST_GTK
!ifdef INST_BOTH
    ${MementoUnselectedSection} "Catalan (GTK+ only)" SecLangCatalan
!else ifdef INST_GTK
	${MementoUnselectedSection} "Catalan" SecLangCatalan
!endif
        SectionIn 2
        !insertmacro InstallGtkLanguage Catalan ca
    ${MementoSectionEnd}
!endif

    ${MementoUnselectedSection} "Czech" SecLangCzech
        SectionIn 2
        !insertmacro InstallGtkLanguage Czech cs
        !insertmacro InstallQtLanguage Czech cs
    ${MementoSectionEnd}

    ${MementoUnselectedSection} "French" SecLangFrench
        SectionIn 2
        !insertmacro InstallGtkLanguage French fr
        !insertmacro InstallQtLanguage French fr
    ${MementoSectionEnd}

!ifdef INST_BOTH | INST_GTK
!ifdef INST_BOTH
    ${MementoUnselectedSection} "German (GTK+ only)" SecLangGerman
!else ifdef INST_GTK
	${MementoUnselectedSection} "German" SecLangGerman
!endif
        SectionIn 2
        !insertmacro InstallGtkLanguage German de
    ${MementoSectionEnd}
!endif

!ifdef INST_BOTH | INST_GTK
!ifdef INST_BOTH
    ${MementoUnselectedSection} "Greek (GTK+ only)" SecLangGreek
!else ifdef INST_GTK
	${MementoUnselectedSection} "Greek" SecLangGreek
!endif
        SectionIn 2
        !insertmacro InstallGtkLanguage Greek el
    ${MementoSectionEnd}
!endif

    ${MementoUnselectedSection} "Italian" SecLangItalian
        SectionIn 2
        !insertmacro InstallGtkLanguage Italian it
        !insertmacro InstallQtLanguage Italian it
    ${MementoSectionEnd}

!ifdef INST_BOTH | INST_GTK
!ifdef INST_BOTH
    ${MementoUnselectedSection} "Japanese (GTK+ only)" SecLangJapanese
!else ifdef INST_GTK
	${MementoUnselectedSection} "Japanese" SecLangJapanese
!endif
        SectionIn 2
        !insertmacro InstallGtkLanguage Japanese ja
    ${MementoSectionEnd}
!endif

!ifdef INST_BOTH | INST_GTK
!ifdef INST_BOTH
    ${MementoUnselectedSection} "Russian (GTK+ only)" SecLangRussian
!else ifdef INST_GTK
	${MementoUnselectedSection} "Russian" SecLangRussian
!endif
        SectionIn 2
        !insertmacro InstallGtkLanguage Russian ru
    ${MementoSectionEnd}
!endif

!ifdef INST_BOTH | INST_GTK
!ifdef INST_BOTH
    ${MementoUnselectedSection} "Serbian Cyrillic (GTK+ only)" SecLangSerbianCyrillic
!else ifdef INST_GTK
    ${MementoUnselectedSection} "Serbian Cyrillic" SecLangSerbianCyrillic
!endif
        SectionIn 2
        !insertmacro InstallGtkLanguage SerbianCyrillic sr
    ${MementoSectionEnd}
!endif

!ifdef INST_BOTH | INST_GTK
!ifdef INST_BOTH
	${MementoUnselectedSection} "Serbian Latin (GTK+ only)" SecLangSerbianLatin
!else ifdef INST_GTK
    ${MementoUnselectedSection} "Serbian Latin" SecLangSerbianLatin
!endif
        SectionIn 2
        !insertmacro InstallGtkLanguage SerbianLatin sr@latin
    ${MementoSectionEnd}
!endif

!ifdef INST_BOTH | INST_GTK
!ifdef INST_BOTH
    ${MementoUnselectedSection} "Spanish (GTK+ only)" SecLangSpanish
!else ifdef INST_GTK
	${MementoUnselectedSection} "Spanish" SecLangSpanish
!endif
        SectionIn 2
        !insertmacro InstallGtkLanguage Spanish es
    ${MementoSectionEnd}
!endif

!ifdef INST_BOTH | INST_GTK
!ifdef INST_BOTH
    ${MementoUnselectedSection} "Turkish (GTK+ only)" SecLangTurkish
!else ifdef INST_GTK
	${MementoUnselectedSection} "Turkish" SecLangTurkish
!endif
        SectionIn 2
        !insertmacro InstallGtkLanguage Turkish tr
    ${MementoSectionEnd}
!endif
SectionGroupEnd

${MementoUnselectedSection} "Avisynth Proxy" SecAvsProxy
    SectionIn 2
    SetOutPath $INSTDIR
    SetOverwrite on
    ${File} avsproxy.exe
    ${File} avsproxy_gui.exe
${MementoSectionEnd}

${MementoSection} "-Start menu Change Log" SecStartMenuChangeLog
    CreateDirectory $SMPROGRAMS\$StartMenuGroup
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    SetOutPath $INSTDIR
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\Change Log.lnk" "$INSTDIR\Change Log.html"
    !insertmacro MUI_STARTMENU_WRITE_END
${MementoSectionEnd}

${MementoSection} "-Start menu GTK+" SecStartMenuGtk
!ifdef INST_GTK
    CreateDirectory $SMPROGRAMS\$StartMenuGroup
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    SetOutPath $INSTDIR
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\${INTERNALNAME} GTK+.lnk" $INSTDIR\avidemux2_gtk.exe
    !insertmacro MUI_STARTMENU_WRITE_END
!endif
${MementoSectionEnd}

${MementoSection} "-Start menu Qt" SecStartMenuQt
!ifdef INST_QT
    CreateDirectory $SMPROGRAMS\$StartMenuGroup
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    SetOutPath $INSTDIR
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\${INTERNALNAME}.lnk" $INSTDIR\avidemux2_qt4.exe
    !insertmacro MUI_STARTMENU_WRITE_END
!endif
${MementoSectionEnd}

${MementoSection} "-Start menu AVS Proxy GUI" SecStartMenuAvsProxyGui
    CreateDirectory $SMPROGRAMS\$StartMenuGroup
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    SetOutPath $INSTDIR
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\AVS Proxy GUI.lnk" "$INSTDIR\avsproxy_gui.exe"
    !insertmacro MUI_STARTMENU_WRITE_END
${MementoSectionEnd}

${MementoSection} "-Quick Launch GTK+" SecQuickLaunchGtk
!ifdef INST_GTK
    SetOutPath $INSTDIR
    CreateShortcut "$QUICKLAUNCH\${INTERNALNAME} GTK+.lnk" $INSTDIR\avidemux2_gtk.exe
!endif
${MementoSectionEnd}

${MementoSection} "-Quick Launch Qt" SecQuickLaunchQt
!ifdef INST_QT
    SetOutPath $INSTDIR
    CreateShortcut "$QUICKLAUNCH\${INTERNALNAME}.lnk" $INSTDIR\avidemux2_qt4.exe
!endif
${MementoSectionEnd}

${MementoSection} "-Desktop GTK+" SecDesktopGtk
!ifdef INST_GTK
    SetOutPath $INSTDIR
    CreateShortcut "$DESKTOP\${INTERNALNAME} GTK+.lnk" $INSTDIR\avidemux2_gtk.exe
!endif
${MementoSectionEnd}

${MementoSection} "-Desktop Qt" SecDesktopQt
!ifdef INST_QT
    SetOutPath $INSTDIR
    CreateShortcut "$DESKTOP\${INTERNALNAME}.lnk" $INSTDIR\avidemux2_qt4.exe
!endif
${MementoSectionEnd}

${MementoSectionDone}

Section -post SecUninstaller
    SectionIn 1 2
    WriteRegStr HKLM "${REGKEY}" Path $INSTDIR
    WriteRegStr HKLM "${REGKEY}" Version ${VERSION}
    SetOutPath $INSTDIR
    WriteUninstaller $INSTDIR\uninstall.exe
    WriteRegStr HKLM "${UNINST_REGKEY}" DisplayName "${INTERNALNAME}"
    WriteRegStr HKLM "${UNINST_REGKEY}" DisplayVersion "${VERSION}"
    WriteRegStr HKLM "${UNINST_REGKEY}" DisplayIcon $INSTDIR\uninstall.exe
    WriteRegStr HKLM "${UNINST_REGKEY}" UninstallString $INSTDIR\uninstall.exe
    WriteRegDWORD HKLM "${UNINST_REGKEY}" NoModify 1
    WriteRegDWORD HKLM "${UNINST_REGKEY}" NoRepair 1
SectionEnd

Section -CloseLogFile
	FileClose $UninstallLogHandle
	SetFileAttributes ${UninstallLogPath} HIDDEN
SectionEnd
 
Section Uninstall
	!insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuGroup	

!ifdef INST_GTK
	Delete /REBOOTOK "$QUICKLAUNCH\${INTERNALNAME} GTK+.lnk"
    Delete /REBOOTOK "$DESKTOP\${INTERNALNAME} GTK+.lnk"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\${INTERNALNAME} GTK+.lnk"
!endif

!ifdef INST_QT
    Delete /REBOOTOK "$QUICKLAUNCH\${INTERNALNAME}.lnk"
    Delete /REBOOTOK "$DESKTOP\${INTERNALNAME}.lnk"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\${INTERNALNAME}.lnk"
!endif

	Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\Change Log.lnk"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\AVS Proxy GUI.lnk"
    RmDir /REBOOTOK $SMPROGRAMS\$StartMenuGroup
    DeleteRegValue HKLM "${REGKEY}" StartMenuGroup
    
    DeleteRegKey HKLM "${UNINST_REGKEY}"
    DeleteRegValue HKLM "${REGKEY}" Path
    DeleteRegKey /IfEmpty HKLM "${REGKEY}"

	FileOpen $UninstallLogHandle "${UninstallLogPath}" r
UninstallLoop:
    ClearErrors
    FileRead $UninstallLogHandle $R0
    IfErrors UninstallEnd
	Push $R0
    Call un.TrimNewLines
    Pop $R0
    Delete "$R0"
    Goto UninstallLoop
UninstallEnd:
	FileClose $UninstallLogHandle
	Delete "${UninstallLogPath}"
	Delete "$INSTDIR\uninstall.exe"
	Push "\"
	Call un.RemoveEmptyDirs
	RMDir "$INSTDIR"
SectionEnd

##########################
# Installer functions
##########################
Function .onInit
	Call LoadPreviousSettings

	ReadRegStr $PreviousVersion HKLM "${REGKEY}" Version

	${If} $PreviousVersion != ""
		${VersionCompare} ${VERSION} $PreviousVersion $PreviousVersionState
	${EndIf}
	
    InitPluginsDir
    SetShellVarContext all
FunctionEnd

Function .onInstSuccess
	${MementoSectionSave}
FunctionEnd

!ifdef INST_BOTH
Function .onSelChange
	!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} end checkCLI
checkCLI:
	!insertmacro SectionFlagIsSet ${SecUiCli} ${SF_SELECTED} end disable

disable:  # GTK langs only
	SectionSetFlags ${SecLangCatalan} SF_RO
	SectionSetFlags ${SecLangCzech} SF_RO
	SectionSetFlags ${SecLangFrench} SF_RO
	SectionSetFlags ${SecLangGerman} SF_RO
	SectionSetFlags ${SecLangGreek} SF_RO
	SectionSetFlags ${SecLangJapanese} SF_RO
	SectionSetFlags ${SecLangRussian} SF_RO
	SectionSetFlags ${SecLangSerbianCyrillic} SF_RO
	SectionSetFlags ${SecLangSerbianLatin} SF_RO
	SectionSetFlags ${SecLangSpanish} SF_RO
	SectionSetFlags ${SecLangTurkish} SF_RO
end:
FunctionEnd
!endif

Function LoadPreviousSettings
    ${MementoSectionRestore}
	!insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuGroup

#checkStartMenuGtk:
	!insertmacro SectionFlagIsSet ${SecStartMenuGtk} ${SF_SELECTED} enableStartMenu checkStartMenuQt
checkStartMenuQt:
	!insertmacro SectionFlagIsSet ${SecStartMenuQt} ${SF_SELECTED} enableStartMenu checkDesktopGtk

enableStartMenu:
	StrCpy $CreateStartMenuGroup 1

checkDesktopGtk:
	!insertmacro SectionFlagIsSet ${SecDesktopGtk} ${SF_SELECTED} enableDesktop checkDesktopQt
checkDesktopQt:
	!insertmacro SectionFlagIsSet ${SecDesktopQt} ${SF_SELECTED} enableDesktop checkQuickLaunchGtk

enableDesktop:
	StrCpy $CreateDesktopIcon 1
	
checkQuickLaunchGtk:
	!insertmacro SectionFlagIsSet ${SecQuickLaunchGtk} ${SF_SELECTED} enableQuickLaunch checkQuickLaunchQt
checkQuickLaunchQt:
	!insertmacro SectionFlagIsSet ${SecQuickLaunchQt} ${SF_SELECTED} enableQuickLaunch end

enableQuickLaunch:
	StrCpy $CreateQuickLaunchIcon 1	

end:
FunctionEnd

Function RunUninstaller
    ReadRegStr $R1  HKLM "${UNINST_REGKEY}" "UninstallString"

	${If} $R1 == ""
		Return
	${EndIf}

	;Run uninstaller
	HideWindow
	ClearErrors
	
	${If} $PreviousVersionState == 0
	${AndIf} $ReinstallUninstall == 1
		ExecWait '$R1 _?=$INSTDIR'
	${Else}
		ExecWait '$R1 /frominstall _?=$INSTDIR'
	${EndIf}

	IfErrors NoRemoveUninstaller
	IfFileExists "$INSTDIR\uninstall.exe" 0 NoRemoveUninstaller
		Delete "$R1"
		RMDir $INSTDIR

NoRemoveUninstaller:
FunctionEnd

!ifdef INST_BOTH
Function CheckSelectedUIs
	!insertmacro SectionFlagIsSet ${SecGrpUI} ${SF_SELECTED} end checkPartial
checkPartial:
	!insertmacro SectionFlagIsSet ${SecGrpUI} ${SF_PSELECTED} end displayError
displayError:
    MessageBox MB_OK|MB_ICONSTOP "At least one User Interface must be selected."
    Abort
end:
FunctionEnd
!endif

LangString INSTALL_OPTS_PAGE_TITLE ${LANG_ENGLISH} "Choose Install Options"
LangString INSTALL_OPTS_PAGE_SUBTITLE ${LANG_ENGLISH} "Choose where to install Avidemux icons."
Var dlgInstallOptions
Var lblCreateIcons
Var chkDesktop
Var chkStartMenu
Var chkQuickLaunch

Function InstallOptionsPage
	Call IsInstallOptionsRequired
	!insertmacro MUI_HEADER_TEXT "$(INSTALL_OPTS_PAGE_TITLE)" "$(INSTALL_OPTS_PAGE_SUBTITLE)"

	nsDialogs::Create 1018
	Pop $dlgInstallOptions

	${If} $dlgInstallOptions == error
		Abort
	${EndIf}

	${NSD_CreateLabel} 0 0u 100% 12u "Create icons for Avidemux:"
	Pop $lblCreateIcons

	${NSD_CreateCheckBox} 0 18u 100% 12u "On my &Desktop"
	Pop $chkDesktop
	${NSD_SetState} $chkDesktop $CreateDesktopIcon
	${NSD_OnClick} $chkDesktop UpdateInstallOptions

	${NSD_CreateCheckBox} 0 36u 100% 12u "In my &Start Menu Programs folder"
	Pop $chkStartMenu
	${NSD_SetState} $chkStartMenu $CreateStartMenuGroup
	${NSD_OnClick} $chkStartMenu UpdateInstallOptions

	${NSD_CreateCheckBox} 0 54u 100% 12u "In my &Quick Launch bar"
	Pop $chkQuickLaunch
	${NSD_SetState} $chkQuickLaunch $CreateQuickLaunchIcon
	${NSD_OnClick} $chkQuickLaunch UpdateInstallOptions

	nsDialogs::Show
FunctionEnd

Function UpdateInstallOptions
	${NSD_GetState} $chkDesktop $CreateDesktopIcon
	${NSD_GetState} $chkStartMenu $CreateStartMenuGroup
	${NSD_GetState} $chkQuickLaunch $CreateQuickLaunchIcon
FunctionEnd

Function IsInstallOptionsRequired
!ifdef INST_GTK
	!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} end checkQt
checkQt:
!ifndef INST_BOTH
Goto end
!endif
!endif
!ifdef INST_QT
	!insertmacro SectionFlagIsSet ${SecUiQt} ${SF_SELECTED} end resetOptions
resetOptions:
!endif

    StrCpy $CreateDesktopIcon 0
    StrCpy $CreateStartMenuGroup 0
    StrCpy $CreateQuickLaunchIcon 0
    Abort

end:
FunctionEnd

Function IsStartMenuRequired
    StrCmp $CreateStartMenuGroup 1 +2
        Abort
FunctionEnd

Function ActivateInternalSections
    #AVS Proxy GUI shortcut:
    SectionGetFlags ${SecAvsProxy} $0
    IntOp $0 $0 & ${SF_SELECTED}
    IntOp $0 $0 & $CreateStartMenuGroup
    SectionSetFlags ${SecStartMenuAvsProxyGui} $0

    #Change Log shortcut:
    SectionSetFlags ${SecStartMenuChangeLog} $CreateStartMenuGroup

!ifdef INST_GTK
    #GTK shortcuts:
    SectionGetFlags ${SecUiGtk} $0
    IntOp $0 $0 & ${SF_SELECTED}

    IntOp $1 $0 & $CreateDesktopIcon
    SectionSetFlags ${SecDesktopGtk} $1

    IntOp $1 $0 & $CreateQuickLaunchIcon
    SectionSetFlags ${SecQuickLaunchGtk} $1

    IntOp $1 $0 & $CreateStartMenuGroup
    SectionSetFlags ${SecStartMenuGtk} $1
!endif

!ifdef INST_QT
    #Qt shortcuts:
    SectionGetFlags ${SecUiQt} $0
    IntOp $0 $0 & ${SF_SELECTED}

    IntOp $1 $0 & $CreateDesktopIcon
    SectionSetFlags ${SecDesktopQt} $1

    IntOp $1 $0 & $CreateQuickLaunchIcon
    SectionSetFlags ${SecQuickLaunchQt} $1

    IntOp $1 $0 & $CreateStartMenuGroup
    SectionSetFlags ${SecStartMenuQt} $1
!endif
FunctionEnd

Function InstFilesPageShow
	${If} $ReinstallUninstall != ""
		Call RunUninstaller
		BringToFront
	${EndIf}
FunctionEnd

Function InstFilesPageLeave
	; Don't advance automatically if details expanded
	FindWindow $R0 "#32770" "" $HWNDPARENT
	GetDlgItem $R0 $R0 1016
	System::Call user32::IsWindowVisible(i$R0)i.s
	Pop $R0

	StrCmp $R0 0 +2
	SetAutoClose false
FunctionEnd

Function ConfigureFinishPage
!ifdef INST_GTK
    SectionGetFlags ${SecUiGtk} $0
    IntOp $0 $0 & ${SF_SELECTED}
    StrCmp $0 ${SF_SELECTED} end
!endif

!ifdef INST_QT
    SectionGetFlags ${SecUiQt} $0
    IntOp $0 $0 & ${SF_SELECTED}
    StrCmp $0 ${SF_SELECTED} end
!endif

    DeleteINISec "$PLUGINSDIR\ioSpecial.ini" "Field 4"

end:
FunctionEnd

Function RunAvidemux
    SetOutPath $INSTDIR

!ifdef INST_QT
#Qt:
    SectionGetFlags ${SecUiQt} $0
    IntOp $0 $0 & ${SF_SELECTED}

    StrCmp $0 ${SF_SELECTED} 0 GTK
        Exec "$INSTDIR\avidemux2_qt4.exe"

    Goto end
GTK:
!endif

!ifdef INST_GTK
    SectionGetFlags ${SecUiGtk} $0
    IntOp $0 $0 & ${SF_SELECTED}

    StrCmp $0 ${SF_SELECTED} 0 End
        Exec "$INSTDIR\avidemux2_gtk.exe"
!endif

end:
FunctionEnd

Var ReinstallUninstallButton

Function ReinstallPage
	${If} $PreviousVersion == ""
		Abort
	${EndIf}

	nsDialogs::Create /NOUNLOAD 1018
	Pop $0

	${If} $PreviousVersionState == 1
		!insertmacro MUI_HEADER_TEXT "Already Installed" "Choose how you want to install ${INTERNALNAME}."
		nsDialogs::CreateItem /NOUNLOAD STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 0 0 100% 40 "An older version of Avidemux is installed on your system.  Select the operation you want to perform and click Next to continue."
		Pop $R0
		nsDialogs::CreateItem /NOUNLOAD BUTTON ${BS_AUTORADIOBUTTON}|${BS_VCENTER}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS}|${WS_GROUP}|${WS_TABSTOP} 0 10 55 100% 30 "Upgrade Avidemux using previous settings (recommended)"
		Pop $ReinstallUninstallButton
		nsDialogs::CreateItem /NOUNLOAD BUTTON ${BS_AUTORADIOBUTTON}|${BS_TOP}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 10 85 100% 50 "Change settings (advanced)"
		Pop $R0

		${If} $ReinstallUninstall == ""
			StrCpy $ReinstallUninstall 1
		${EndIf}
	${ElseIf} $PreviousVersionState == 2
		!insertmacro MUI_HEADER_TEXT "Already Installed" "Choose how you want to install ${INTERNALNAME}."
		nsDialogs::CreateItem /NOUNLOAD STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 0 0 100% 40 "A newer version of Avidemux is already installed! It is not recommended that you downgrade to an older version. Select the operation you want to perform and click Next to continue."
		Pop $R0
		nsDialogs::CreateItem /NOUNLOAD BUTTON ${BS_AUTORADIOBUTTON}|${BS_VCENTER}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS}|${WS_GROUP}|${WS_TABSTOP} 0 10 55 100% 30 "Downgrade Avidemux using previous settings (recommended)"
		Pop $ReinstallUninstallButton
		nsDialogs::CreateItem /NOUNLOAD BUTTON ${BS_AUTORADIOBUTTON}|${BS_TOP}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 10 85 100% 50 "Change settings (advanced)"
		Pop $R0

		${If} $ReinstallUninstall == ""
			StrCpy $ReinstallUninstall 1
		${EndIf}
	${ElseIf} $PreviousVersionState == 0
		!insertmacro MUI_HEADER_TEXT "Already Installed" "Choose the maintenance option to perform."
		nsDialogs::CreateItem /NOUNLOAD STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 0 0 100% 40 "Avidemux ${VERSION} is already installed. Select the operation you want to perform and click Next to continue."
		Pop $R0
		nsDialogs::CreateItem /NOUNLOAD BUTTON ${BS_AUTORADIOBUTTON}|${BS_VCENTER}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS}|${WS_GROUP}|${WS_TABSTOP} 0 10 55 100% 30 "Add/Remove/Reinstall components"
		Pop $R0
		nsDialogs::CreateItem /NOUNLOAD BUTTON ${BS_AUTORADIOBUTTON}|${BS_TOP}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 10 85 100% 50 "Uninstall Avidemux"
		Pop $ReinstallUninstallButton

		${If} $ReinstallUninstall == ""
			StrCpy $ReinstallUninstall 2
		${EndIf}
	${Else}
		MessageBox MB_ICONSTOP "Unknown value of PreviousVersionState, aborting" /SD IDOK
		Abort
	${EndIf}

	${If} $ReinstallUninstall == "1"
		SendMessage $ReinstallUninstallButton ${BM_SETCHECK} 1 0
	${Else}
		SendMessage $R0 ${BM_SETCHECK} 1 0
	${EndIf}

	nsDialogs::Show
FunctionEnd

Function ReinstallPageLeave
	SendMessage $ReinstallUninstallButton ${BM_GETCHECK} 0 0 $R0

	${If} $R0 == 1
		; Option to uninstall old version selected
		StrCpy $ReinstallUninstall 1
	${Else}
		; Custom up/downgrade or add/remove/reinstall
		StrCpy $ReinstallUninstall 2
	${EndIf}

	${If} $ReinstallUninstall == 1
		${If} $PreviousVersionState == 0
			Call RunUninstaller
			Quit
		${Else}
			; Need to reload defaults. User could have
			; chosen custom, change something, went back and selected
			; the express option.
			Call LoadPreviousSettings
		${EndIf}
	${EndIf}
FunctionEnd


##########################
# Uninstaller functions
##########################
Function un.onInit
	SetShellVarContext all
FunctionEnd

; TrimNewlines (copied from NSIS documentation)
; input, top of stack  (e.g. whatever$\r$\n)
; output, top of stack (replaces, with e.g. whatever)
; modifies no other variables.
Function un.TrimNewlines
	Exch $R0
	Push $R1
	Push $R2
	StrCpy $R1 0

loop:
	IntOp $R1 $R1 - 1
	StrCpy $R2 $R0 1 $R1
	StrCmp $R2 "$\r" loop
	StrCmp $R2 "$\n" loop
	IntOp $R1 $R1 + 1
	IntCmp $R1 0 no_trim_needed
	StrCpy $R0 $R0 $R1

no_trim_needed:
	Pop $R2
	Pop $R1
	Exch $R0
FunctionEnd
 
Function un.RemoveEmptyDirs
	Pop $9
	!define Index 'Line${__LINE__}'
	FindFirst $0 $1 "$INSTDIR$9*"
	StrCmp $0 "" "${Index}-End"
"${Index}-Loop:"
	StrCmp $1 "" "${Index}-End"
	StrCmp $1 "." "${Index}-Next"
	StrCmp $1 ".." "${Index}-Next"
	Push $0
	Push $1
	Push $9
	Push "$9$1\"
	Call un.RemoveEmptyDirs
	Pop $9
	Pop $1
	Pop $0
;"${Index}-Remove:"
	RMDir "$INSTDIR$9$1"
"${Index}-Next:"
	FindNext $0 $1
	Goto "${Index}-Loop"
"${Index}-End:"
	FindClose $0
	!undef Index
FunctionEnd

Function un.ConfirmPagePre
	${un.GetParameters} $R0
	${un.GetOptions} $R0 "/frominstall" $R1
	${Unless} ${Errors}
		Abort
	${EndUnless}
FunctionEnd

Function un.FinishPagePre
	${un.GetParameters} $R0
	${un.GetOptions} $R0 "/frominstall" $R1
	${Unless} ${Errors}
		SetRebootFlag false
		Abort
	${EndUnless}
FunctionEnd
