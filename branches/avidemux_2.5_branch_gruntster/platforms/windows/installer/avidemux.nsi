##########################
# Included files
##########################
!include Sections.nsh
!include MUI2.nsh
!include nsDialogs.nsh
!include Memento.nsh
!include FileFunc.nsh
!include ${NSIDIR}\revision.nsh

Name "Avidemux 2.5.0 beta r${REVISION}"

SetCompressor /SOLID lzma
SetCompressorDictSize 96

##########################
# Defines
##########################
!define INTERNALNAME "Avidemux 2.5"
!define REGKEY "SOFTWARE\${INTERNALNAME}"
!define UNINST_REGKEY "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${INTERNALNAME}"
!define VERSION 2.5.0.${REVISION}
!define COMPANY "Free Software Foundation"
!define URL "http://www.avidemux.org"

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
!define MUI_HEADERIMAGE_BITMAP "${NSIDIR}\PageHeader.bmp"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT HKLM
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${REGKEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME StartMenuGroup
!define MUI_STARTMENUPAGE_DEFAULTFOLDER Avidemux
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_WELCOMEFINISHPAGE_BITMAP "${NSIDIR}\WelcomeFinishStrip.bmp"
!define MUI_UNICON "${NSIDIR}\..\..\..\avidemux\xpm\adm.ico"
!define MUI_UNFINISHPAGE_NOAUTOCLOSE
!define MUI_COMPONENTSPAGE_NODESC

##########################
# Variables
##########################
Var CreateDesktopIcon
Var CreateStartMenuGroup
Var CreateQuickLaunchIcon
Var StartMenuGroup

##########################
# Installer pages
##########################
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "${NSIDIR}\License.rtf"
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE CheckSelectedUIs
!insertmacro MUI_PAGE_COMPONENTS
Page custom InstallOptionsPage InstallOptionsPageLeave
!define MUI_PAGE_CUSTOMFUNCTION_PRE IsStartMenuRequired
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuGroup
!insertmacro MUI_PAGE_DIRECTORY
!define MUI_PAGE_CUSTOMFUNCTION_PRE ActivateInternalSections
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_FUNCTION RunAvidemux
!define MUI_FINISHPAGE_RUN_TEXT "Run ${INTERNALNAME} now"
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\Change Log.html"
!define MUI_FINISHPAGE_SHOWREADME_TEXT "View Change Log now"
!define MUI_FINISHPAGE_LINK "Visit the Avidemux Win32 Builds website"
!define MUI_FINISHPAGE_LINK_LOCATION "http://www.razorbyte.com.au/avidemux/"
!define MUI_PAGE_CUSTOMFUNCTION_PRE ConfigureFinishPage
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

##########################
# Installer languages
##########################
!insertmacro MUI_LANGUAGE English

##########################
# Installer attributes
##########################
OutFile ${EXEDIR}\avidemux_2.5_r${REVISION}_win32.exe
InstallDir "$PROGRAMFILES\Avidemux 2.5"
CRCCheck on
XPStyle on
ShowInstDetails nevershow
VIProductVersion 2.5.0.${REVISION}
VIAddVersionKey ProductName Avidemux
VIAddVersionKey ProductVersion "${VERSION} (beta)"
VIAddVersionKey FileVersion ""
VIAddVersionKey FileDescription ""
VIAddVersionKey LegalCopyright ""
InstallDirRegKey HKLM "${REGKEY}" Path
ShowUninstDetails nevershow
BrandingText "Packaged by Gruntster"
InstType Standard
InstType Full

##########################
# Uninstaller macros
##########################
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
	SetOverwrite on

	!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} installGtk${LANG_CODE} endGtk${LANG_CODE}

installGtk${LANG_CODE}:
    SetOutPath $INSTDIR\share\locale\${LANG_CODE}\LC_MESSAGES
    ${File} share\locale\${LANG_CODE}\LC_MESSAGES\avidemux.mo
    ${File} share\locale\${LANG_CODE}\LC_MESSAGES\gtk20.mo

endGtk${LANG_CODE}:
!macroend

!macro InstallQt4Language LANG_NAME LANG_CODE
	SetOverwrite on

	!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} installQt4${LANG_CODE} endQt4${LANG_CODE}

installQt4${LANG_CODE}:	
	SetOutPath $INSTDIR\i18n
    ${File} i18n\avidemux_${LANG_CODE}.qm
    ${File} i18n\qt_${LANG_CODE}.qm

endQt4${LANG_CODE}:
!macroend

##########################
# Installer sections
##########################
Section -OpenLogFile
	CreateDirectory "$INSTDIR"
	FileOpen $UninstallLogHandle ${UninstallLogPath} a
	FileSeek $UninstallLogHandle 0 END
SectionEnd

Section "Core files (required)" SecCore
    SectionIn 1 2 RO
    SetOutPath $INSTDIR
    SetOverwrite on
    ${File} "Build Info.txt"
    ${File} "Change Log.html"
    ${File} zlib1.dll
    ${File} freetype6.dll
    ${File} iconv.dll
    ${File} intl.dll
    ${File} libADM_core.dll
    ${File} libADM_coreAudio.dll
    ${File} libADM_coreImage.dll
    ${File} libADM_coreUI.dll
    ${File} libADM_smjs.dll
    ${File} libaften.dll
    ${File} libexpat-*.dll
    ${File} libfontconfig-1.dll
    ${File} libxml2.dll
    ${File} ogg.dll
    ${File} pthreadGC2.dll
    ${File} SDL.dll
    ${File} xmltok.dll
    ${File} AUTHORS.
    ${File} COPYING.
    ${File} README.
    ${File} avcodec-*.dll
    ${File} avformat-*.dll
    ${File} avutil-*.dll
    ${File} postproc-*.dll
    ${File} swscale-*.dll
    SetOutPath $INSTDIR\etc\fonts
    ${Folder} etc\fonts
SectionEnd

SectionGroup /e "User interfaces" SecGrpUI
    ${MementoSection} "Command line" SecUiCli
        SectionIn 1 2
        SetOutPath $INSTDIR
        SetOverwrite on
        ${File} avidemux2_cli.exe
        ${File} libADM_render_cli.dll
        ${File} libADM_UICli.dll
    ${MementoSectionEnd}

    ${MementoUnselectedSection} GTK+ SecUiGtk
        SectionIn 2
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
        ${File} jpeg62.dll
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
        ${File} libpangowin32-1.0-0.dll
        ${File} libpng12-0.dll
        ${File} libtiff3.dll
    ${MementoSectionEnd}

    ${MementoSection} Qt4 SecUiQt4
        SectionIn 1 2
        SetOutPath $INSTDIR
        SetOverwrite on
        ${File} QtGui4.dll
        ${File} avidemux2_qt4.exe
        ${File} libADM_render_qt4.dll
        ${File} libADM_UIQT4.dll
        ${File} mingwm10.dll
        ${File} QtCore4.dll
    ${MementoSectionEnd}
SectionGroupEnd

SectionGroup Plugins SecGrpPlugin
	SectionGroup "Audio Decoders" SecGrpAudioDecoder
		${MementoSection} "FAAD2 (AAC)" SecAudDecFaad
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioDecoder
			${File} lib\ADM_plugins\audioDecoder\libADM_ad_faad.dll
			SetOutPath $INSTDIR
			${File} libfaad2.dll
		${MementoSectionEnd}
		${MementoSection} "liba52 (AC-3)" SecAudDecA52
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioDecoder
			${File} lib\ADM_plugins\audioDecoder\libADM_ad_a52.dll
		${MementoSectionEnd}
		${MementoSection} "MAD (MPEG)" SecAudDecMad
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioDecoder
			${File} lib\ADM_plugins\audioDecoder\libADM_ad_Mad.dll
		${MementoSectionEnd}
	SectionGroupEnd
	SectionGroup "Audio Devices" SecGrpAudioDevice
		${MementoUnselectedSection} SDL SecAudDevSdl
			SectionIn 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioDevices
			${File} lib\ADM_plugins\audioDevices\libADM_av_sdl.dll
		${MementoSectionEnd}
		${MementoSection} "MS Windows (Waveform)" SecAudDevWaveform
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioDevices
			${File} lib\ADM_plugins\audioDevices\libADM_av_win32.dll
		${MementoSectionEnd}
	SectionGroupEnd
	SectionGroup "Audio Encoders" SecGrpAudioEncoder
		${MementoSection} "FAAC (AAC)" SecAudEncFaac
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioEncoders
			${File} lib\ADM_plugins\audioEncoders\libADM_ae_faac.dll
			SetOutPath $INSTDIR
			${File} libfaac.dll
		${MementoSectionEnd}
		${MementoSection} "LAME (MP3)" SecAudEncLame
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioEncoders
			${File} lib\ADM_plugins\audioEncoders\libADM_ae_lame.dll
			SetOutPath $INSTDIR
			${File} libmp3lame-0.dll
		${MementoSectionEnd}
		${MementoSection} "libavcodec (AC-3)" SecAudEncLavAc3
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioEncoders
			${File} lib\ADM_plugins\audioEncoders\libADM_ae_lav_ac3.dll
		${MementoSectionEnd}
		${MementoSection} "libavcodec (MP2)" SecAudEncLavMp2
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioEncoders
			${File} lib\ADM_plugins\audioEncoders\libADM_ae_lav_mp2.dll
		${MementoSectionEnd}
		${MementoSection} "libvorbis (Vorbis)" SecAudEncVorbis
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioEncoders
			${File} lib\ADM_plugins\audioEncoders\libADM_ae_vorbis.dll
			SetOutPath $INSTDIR
			${File} vorbis.dll
			${File} vorbisenc.dll
		${MementoSectionEnd}
		${MementoSection} "PCM" SecAudEncPcm
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioEncoders
			${File} lib\ADM_plugins\audioEncoders\libADM_ae_pcm.dll
		${MementoSectionEnd}
		${MementoSection} "TwoLAME (MP2)" SecAudEncTwoLame
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioEncoders
			${File} lib\ADM_plugins\audioEncoders\libADM_ae_twolame.dll
		${MementoSectionEnd}
	SectionGroupEnd
	SectionGroup "Video Encoders" SecGrpVideoEncoder
		${MementoSection} "x264 (MPEG-4 AVC)" SecVidEncX264
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\videoEncoder
			${File} lib\ADM_plugins\videoEncoder\libADM_vidEnc_x264.dll
			SetOutPath $INSTDIR\lib\ADM_plugins\videoEncoder\x264
			!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
			${File} lib\ADM_plugins\videoEncoder\x264\libADM_vidEnc_x264_Gtk.dll
CheckQt4:
			!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
			${File} lib\ADM_plugins\videoEncoder\x264\libADM_vidEnc_x264_Qt.dll
End:
			${File} lib\ADM_plugins\videoEncoder\x264\x264Param.xsd
			${File} lib\ADM_plugins\videoEncoder\x264\*.xml
			SetOutPath $INSTDIR
			${File} libx264-*.dll
		${MementoSectionEnd}
		${MementoSection} "Xvid (MPEG-4 ASP)" SecVidEncXvid
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\videoEncoder
			${File} lib\ADM_plugins\videoEncoder\libADM_vidEnc_xvid.dll
SetOutPath $INSTDIR\lib\ADM_plugins\videoEncoder\xvid
			!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
			${File} lib\ADM_plugins\videoEncoder\xvid\libADM_vidEnc_Xvid_Gtk.dll
CheckQt4:
			!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
			${File} lib\ADM_plugins\videoEncoder\xvid\libADM_vidEnc_Xvid_Qt.dll
End:
			${File} lib\ADM_plugins\videoEncoder\xvid\XvidParam.xsd
			#${File} lib\ADM_plugins\videoEncoder\xvid\*.xml
			SetOutPath $INSTDIR
			${File} xvidcore.dll
		${MementoSectionEnd}
	SectionGroupEnd
	SectionGroup "Video Filters" SecGrpVideoFilter
		SectionGroup "Transform Filters" SecGrpVideoFilterTransform
			${MementoSection} "Add Black Borders" SecVidFltBlackBorders
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_addborders.dll
			${MementoSectionEnd}
			${MementoSection} "Avisynth Resize" SecVidFltAvisynthResize
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_avisynthResize_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_avisynthResize_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Blacken Borders" SecVidFltBlackenBorders
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_blackenBorders.dll
			${MementoSectionEnd}
			${MementoSection} "Crop" SecVidFltCrop
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_Crop_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_crop_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Fade" SecVidFltFade
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_fade.dll
			${MementoSectionEnd}
			${MementoSection} "MPlayer Resize" SecVidFltMplayerResize
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_mplayerResize_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_mplayerResize_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Resample FPS" SecVidFltResampleFps
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_resampleFps.dll
			${MementoSectionEnd}
			${MementoSection} "Reverse" SecVidFltReverse
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_reverse.dll
			${MementoSectionEnd}
			${MementoSection} "Rotate" SecVidFltRotate
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_rotate.dll
			${MementoSectionEnd}
			${MementoSection} "Vertical Flip" SecVidFltVerticalFlip
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_vflip.dll
			${MementoSectionEnd}
		SectionGroupEnd
		SectionGroup "Interlacing Filters" SecGrpVideoFilterInterlacing
			${MementoSection} "Decomb Decimate" SecVidFltDecombDecimate
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_decimate.dll
			${MementoSectionEnd}
			${MementoSection} "Decomb Telecide" SecVidFltDecombTelecide
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_telecide.dll
			${MementoSectionEnd}
			${MementoSection} "Deinterlace" SecVidFltDeinterlace
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_Deinterlace.dll
			${MementoSectionEnd}
			${MementoSection} "DG Bob" SecVidFltDbGob
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_blendDgBob.dll
			${MementoSectionEnd}
			${MementoSection} "Drop" SecVidFltDrop
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_dropOut.dll
			${MementoSectionEnd}
			${MementoSection} "Fade" SecVidFltFadeInterlace
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_separateField.dll
			${MementoSectionEnd}
			${MementoSection} "Gauss Smooth" SecVidFltGaussSmooth
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_fastconvolutiongauss.dll
			${MementoSectionEnd}
			${MementoSection} "Keep Even Fields" SecVidFltKeepEvenFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_keepEvenField.dll
			${MementoSectionEnd}
			${MementoSection} "Keep Odd Fields" SecVidFltKeepOddFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_keepOddField.dll
			${MementoSectionEnd}
			${MementoSection} "KernelDeint" SecVidFltKernelDeint
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_kernelDeint.dll
			${MementoSectionEnd}
			${MementoSection} "libavcodec Deinterlacer" SecVidFltLavDeinterlacer
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_lavDeinterlace.dll
			${MementoSectionEnd}
			${MementoSection} "mcDeint" SecVidFltMcDeint
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_mcdeint.dll
			${MementoSectionEnd}
			${MementoSection} "Mean" SecVidFltMean
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_fastconvolutionmean.dll
			${MementoSectionEnd}
			${MementoSection} "Median" SecVidFltMedian
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_fastconvolutionmedian.dll
			${MementoSectionEnd}
			${MementoSection} "Merge Fields" SecVidFltMergeFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_mergeField.dll
			${MementoSectionEnd}
			${MementoSection} "PAL Field Shift" SecVidFltPalFieldShift
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_palShift.dll
			${MementoSectionEnd}
			${MementoSection} "PAL Smart Field Shift" SecVidFltPalSmartFieldShift
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_smartPalShift.dll
			${MementoSectionEnd}
			${MementoSection} "Pulldown" SecVidFltPulldown
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_Pulldown.dll
			${MementoSectionEnd}
			${MementoSection} "Separate Fields" SecVidFltSeparateFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_hzStackField.dll
			${MementoSectionEnd}
			${MementoSection} "Sharpen" SecVidFltSharpen
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_fastconvolutionsharpen.dll
			${MementoSectionEnd}
			${MementoSection} "Smart Swap Fields" SecVidFltSmartSwapFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_smartSwapField.dll
			${MementoSectionEnd}
			${MementoSection} "Stack Fields" SecVidFltStackFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_stackField.dll
			${MementoSectionEnd}
			${MementoSection} "Swap Fields" SecVidFltSwapFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_swapField.dll
			${MementoSectionEnd}
			${MementoSection} "TDeint" SecVidFltTDeint
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_tdeint.dll
			${MementoSectionEnd}
			${MementoSection} "Unstack Fields" SecVidFltUnstackFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_unstackField.dll
			${MementoSectionEnd}
			${MementoSection} "yadif" SecVidFltYadif
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_yadif.dll
			${MementoSectionEnd}
		SectionGroupEnd
		SectionGroup "Colour Filters" SecGrpVideoFilterColour
			${MementoSection} "Avisynth ColorYUV" SecVidFltAvisynthColorYuv
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_colorYUV_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_colorYUV_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Blend Removal" SecVidFltBlendRemoval
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_blendRemoval.dll
			${MementoSectionEnd}
			${MementoSection} "Chroma Shift" SecVidFltChromaShift
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_chromaShift_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_chromaShift_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Chroma U" SecVidFltChromaU
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vidChromaU.dll
			${MementoSectionEnd}
			${MementoSection} "Chroma V" SecVidFltChromaV
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vidChromaV.dll
			${MementoSectionEnd}
			${MementoSection} "Contrast" SecVidFltContrast
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_contrast_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_contrast_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Luma Delta" SecVidFltLumaDelta
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_Delta.dll
			${MementoSectionEnd}
			${MementoSection} "Luma Equaliser" SecVidFltLumaEqualiser
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_equalizer_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_equalizer_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Luma Only" SecVidFltLumaOnly
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_lumaonly.dll
			${MementoSectionEnd}
			${MementoSection} "Mplayer Eq2" SecVidFltMPlayerEq2
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_eq2_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_eq2_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Mplayer Hue" SecVidFltMPlayerHue
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_hue_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_hue_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Swap U and V" SecVidFltSwapUandV
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_swapuv.dll
			${MementoSectionEnd}
		SectionGroupEnd
		SectionGroup "Noise Filters" SecGrpVideoFilterNoise
			${MementoSection} "Cnr2" SecVidFltCnr2
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_cnr2_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_cnr2_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Denoise" SecVidFltDenoise
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_Denoise.dll
			${MementoSectionEnd}
			${MementoSection} "FluxSmooth" SecVidFltFluxSmooth
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_FluxSmooth.dll
			${MementoSectionEnd}
			${MementoSection} "Forced Post-processing" SecVidFltForcedPostProcessing
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_forcedPP.dll
			${MementoSectionEnd}
			${MementoSection} "Light Denoiser" SecVidFltLightDenoiser
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_Stabilize.dll
			${MementoSectionEnd}
			${MementoSection} "Median (5x5)" SecVidFltMediam5x5
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_largemedian.dll
			${MementoSectionEnd}
			${MementoSection} "MPlayer Denoise3d" SecVidFltMPlayerDenoise3d
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_denoise3d.dll
			${MementoSectionEnd}
			${MementoSection} "MPlayer Hqdn3d" SecVidFltMPlayerHqdn3d
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_denoise3dhq.dll
			${MementoSectionEnd}
			${MementoSection} "Temporal Cleaner" SecVidFltTemporalCleaner
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_vlad.dll
			${MementoSectionEnd}
			${MementoSection} "TIsophote" SecVidFltTisophote
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_Tisophote.dll
			${MementoSectionEnd}
		SectionGroupEnd
		SectionGroup "Sharpness Filters" SecGrpVideoFilterSharpness
			${MementoSection} "asharp" SecVidFltAsharp
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_asharp_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_asharp_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "MSharpen" SecVidFltMSharpen
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_mSharpen.dll
			${MementoSectionEnd}
			${MementoSection} "MSmooth" SecVidFltMSmooth
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_mSmooth.dll
			${MementoSectionEnd}
			${MementoSection} "Soften" SecVidFltSoften
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_soften.dll
			${MementoSectionEnd}
		SectionGroupEnd
		SectionGroup "Subtitle Filters" SecGrpVideoFilterSubtitle
			${MementoSection} "ASS/SSA" SecVidFltAssSsa
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_ssa.dll
			${MementoSectionEnd}
			${MementoSection} "Subtitler" SecVidFltSubtitler
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_sub_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_sub_qt4.dll
End:
			${MementoSectionEnd}
		SectionGroupEnd
		SectionGroup "Miscellaneous Filters" SecGrpVideoFilterMiscellaneous
			${MementoSection} "Mosaic" SecVidFltMosaic
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_Mosaic.dll
			${MementoSectionEnd}
			${MementoSection} "MPlayer Delogo" SecVidFltMPlayerDelogo
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_mpdelogo_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				${File} lib\ADM_plugins\videoFilter\libADM_vf_mpdelogo_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Whirl" SecVidFltWhirl
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				${File} lib\ADM_plugins\videoFilter\libADM_vf_Whirl.dll
			${MementoSectionEnd}
		SectionGroupEnd
	SectionGroupEnd
SectionGroupEnd

SectionGroup "Additional languages" SecGrpLang
    ${MementoUnselectedSection} "Catalan (GTK+ only)" SecLangCatalan
        SectionIn 2
        !insertmacro InstallGtkLanguage Catalan ca
    ${MementoSectionEnd}

    ${MementoUnselectedSection} "Czech" SecLangCzech
        SectionIn 2
        !insertmacro InstallGtkLanguage Czech cs
        !insertmacro InstallQt4Language Czech cs
    ${MementoSectionEnd}

    ${MementoUnselectedSection} "French" SecLangFrench
        SectionIn 2
        !insertmacro InstallGtkLanguage French fr
        !insertmacro InstallQt4Language French fr
    ${MementoSectionEnd}

    ${MementoUnselectedSection} "German (GTK+ only)" SecLangGerman
        SectionIn 2
        !insertmacro InstallGtkLanguage German de
    ${MementoSectionEnd}

    ${MementoUnselectedSection} "Greek (GTK+ only)" SecLangGreek
        SectionIn 2
        !insertmacro InstallGtkLanguage Greek el
    ${MementoSectionEnd}

    ${MementoUnselectedSection} "Italian" SecLangItalian
        SectionIn 2
        !insertmacro InstallGtkLanguage Italian it
        !insertmacro InstallQt4Language Italian it
    ${MementoSectionEnd}

    ${MementoUnselectedSection} "Japanese (GTK+ only)" SecLangJapanese
        SectionIn 2
        !insertmacro InstallGtkLanguage Japanese ja
    ${MementoSectionEnd}

    ${MementoUnselectedSection} "Russian (GTK+ only)" SecLangRussian
        SectionIn 2
        !insertmacro InstallGtkLanguage Russian ru
    ${MementoSectionEnd}

    ${MementoUnselectedSection} "Serbian Cyrillic (GTK+ only)" SecLangSerbianCyrillic
        SectionIn 2
        !insertmacro InstallGtkLanguage SerbianCyrillic sr
    ${MementoSectionEnd}

    ${MementoUnselectedSection} "Serbian Latin (GTK+ only)" SecLangSerbianLatin
        SectionIn 2
        !insertmacro InstallGtkLanguage SerbianLatin sr@latin
    ${MementoSectionEnd}

    ${MementoUnselectedSection} "Spanish (GTK+ only)" SecLangSpanish
        SectionIn 2
        !insertmacro InstallGtkLanguage Spanish es
    ${MementoSectionEnd}

    ${MementoUnselectedSection} "Turkish (GTK+ only)" SecLangTurkish
        SectionIn 2
        !insertmacro InstallGtkLanguage Turkish tr
    ${MementoSectionEnd}
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
    CreateDirectory $SMPROGRAMS\$StartMenuGroup
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    SetOutPath $INSTDIR
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\${INTERNALNAME} GTK+.lnk" $INSTDIR\avidemux2_gtk.exe
    !insertmacro MUI_STARTMENU_WRITE_END
${MementoSectionEnd}

${MementoSection} "-Start menu Qt4" SecStartMenuQt4
    CreateDirectory $SMPROGRAMS\$StartMenuGroup
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    SetOutPath $INSTDIR
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\${INTERNALNAME} Qt4.lnk" $INSTDIR\avidemux2_qt4.exe
    !insertmacro MUI_STARTMENU_WRITE_END
${MementoSectionEnd}

${MementoSection} "-Start menu AVS Proxy GUI" SecStartMenuAvsProxyGui
    CreateDirectory $SMPROGRAMS\$StartMenuGroup
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    SetOutPath $INSTDIR
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\AVS Proxy GUI.lnk" "$INSTDIR\avsproxy_gui.exe"
    !insertmacro MUI_STARTMENU_WRITE_END
${MementoSectionEnd}

${MementoSection} "-Quick Launch GTK+" SecQuickLaunchGtk
    SetOutPath $INSTDIR
    CreateShortcut "$QUICKLAUNCH\${INTERNALNAME} GTK+.lnk" $INSTDIR\avidemux2_gtk.exe
${MementoSectionEnd}

${MementoSection} "-Quick Launch Qt4" SecQuickLaunchQt4
    SetOutPath $INSTDIR
    CreateShortcut "$QUICKLAUNCH\${INTERNALNAME} Qt4.lnk" $INSTDIR\avidemux2_qt4.exe
${MementoSectionEnd}

${MementoSection} "-Desktop GTK+" SecDesktopGtk
    SetOutPath $INSTDIR
    CreateShortcut "$DESKTOP\${INTERNALNAME} GTK+.lnk" $INSTDIR\avidemux2_gtk.exe
${MementoSectionEnd}

${MementoSection} "-Desktop Qt4" SecDesktopQt4
    SetOutPath $INSTDIR
    CreateShortcut "$DESKTOP\${INTERNALNAME} Qt4.lnk" $INSTDIR\avidemux2_qt4.exe
${MementoSectionEnd}

${MementoSectionDone}

Section -post SecUninstaller
    SectionIn 1 2
    WriteRegStr HKLM "${REGKEY}" Path $INSTDIR
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
	
	Delete /REBOOTOK "$QUICKLAUNCH\${INTERNALNAME} GTK+.lnk"
    Delete /REBOOTOK "$QUICKLAUNCH\${INTERNALNAME} Qt4.lnk"
    Delete /REBOOTOK "$DESKTOP\${INTERNALNAME} GTK+.lnk"
    Delete /REBOOTOK "$DESKTOP\${INTERNALNAME} Qt4.lnk"
	Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\Change Log.lnk"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\${INTERNALNAME} GTK+.lnk"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\${INTERNALNAME} Qt4.lnk"
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
    ReadRegStr $R0  HKLM "${UNINST_REGKEY}" "UninstallString"
    StrCmp $R0 "" startInstall

    MessageBox MB_YESNO|MB_ICONEXCLAMATION "${INTERNALNAME} has already been installed. $\n$\nDo you want to remove \
      the previous version before installing $(^Name)?" IDNO startInstall

    # Run the uninstaller
    ClearErrors
    ExecWait '$R0 _?=$INSTDIR' ; Do not copy the uninstaller to a temp file

startInstall:
    InitPluginsDir

    ${MementoSectionRestore}
    SetShellVarContext all
	!insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuGroup

#checkStartMenuGtk:
	!insertmacro SectionFlagIsSet ${SecStartMenuGtk} ${SF_SELECTED} enableStartMenu checkStartMenuQt4
checkStartMenuQt4:
	!insertmacro SectionFlagIsSet ${SecStartMenuQt4} ${SF_SELECTED} enableStartMenu checkDesktopGtk

enableStartMenu:
	StrCpy $CreateStartMenuGroup 1

checkDesktopGtk:
	!insertmacro SectionFlagIsSet ${SecDesktopGtk} ${SF_SELECTED} enableDesktop checkDesktopQt4
checkDesktopQt4:
	!insertmacro SectionFlagIsSet ${SecDesktopQt4} ${SF_SELECTED} enableDesktop checkQuickLaunchGtk

enableDesktop:
	StrCpy $CreateDesktopIcon 1
	
checkQuickLaunchGtk:
	!insertmacro SectionFlagIsSet ${SecQuickLaunchGtk} ${SF_SELECTED} enableQuickLaunch checkQuickLaunchQt4
checkQuickLaunchQt4:
	!insertmacro SectionFlagIsSet ${SecQuickLaunchQt4} ${SF_SELECTED} enableQuickLaunch end

enableQuickLaunch:
	StrCpy $CreateQuickLaunchIcon 1	

end:
FunctionEnd

Function .onInstSuccess
	${MementoSectionSave}
FunctionEnd

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

Function CheckSelectedUIs
	!insertmacro SectionFlagIsSet ${SecGrpUI} ${SF_SELECTED} end checkPartial
checkPartial:
	!insertmacro SectionFlagIsSet ${SecGrpUI} ${SF_PSELECTED} end displayError
displayError:
    MessageBox MB_OK|MB_ICONSTOP "At least one User Interface must be selected."
    Abort
end:
FunctionEnd

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

	${NSD_CreateCheckBox} 0 36u 100% 12u "In my &Start Menu Programs folder"
	Pop $chkStartMenu
	${NSD_SetState} $chkStartMenu $CreateStartMenuGroup

	${NSD_CreateCheckBox} 0 54u 100% 12u "In my &Quick Launch bar"
	Pop $chkQuickLaunch
	${NSD_SetState} $chkQuickLaunch $CreateQuickLaunchIcon

	nsDialogs::Show
FunctionEnd

Function InstallOptionsPageLeave
	${NSD_GetState} $chkDesktop $CreateDesktopIcon
	${NSD_GetState} $chkStartMenu $CreateStartMenuGroup
	${NSD_GetState} $chkQuickLaunch $CreateQuickLaunchIcon
FunctionEnd

Function IsInstallOptionsRequired
	!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} end checkQt4
checkQt4:
	!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} end resetOptions

resetOptions:
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

    #GTK shortcuts:
    SectionGetFlags ${SecUiGtk} $0
    IntOp $0 $0 & ${SF_SELECTED}

    IntOp $1 $0 & $CreateDesktopIcon
    SectionSetFlags ${SecDesktopGtk} $1

    IntOp $1 $0 & $CreateQuickLaunchIcon
    SectionSetFlags ${SecQuickLaunchGtk} $1

    IntOp $1 $0 & $CreateStartMenuGroup
    SectionSetFlags ${SecStartMenuGtk} $1

    #Qt4 shortcuts:
    SectionGetFlags ${SecUiQt4} $0
    IntOp $0 $0 & ${SF_SELECTED}

    IntOp $1 $0 & $CreateDesktopIcon
    SectionSetFlags ${SecDesktopQt4} $1

    IntOp $1 $0 & $CreateQuickLaunchIcon
    SectionSetFlags ${SecQuickLaunchQt4} $1

    IntOp $1 $0 & $CreateStartMenuGroup
    SectionSetFlags ${SecStartMenuQt4} $1
FunctionEnd

Function ConfigureFinishPage
    SectionGetFlags ${SecUiGtk} $0
    IntOp $0 $0 & ${SF_SELECTED}
    StrCmp $0 ${SF_SELECTED} end

    SectionGetFlags ${SecUiQt4} $0
    IntOp $0 $0 & ${SF_SELECTED}
    StrCmp $0 ${SF_SELECTED} end

    DeleteINISec "$PLUGINSDIR\ioSpecial.ini" "Field 4"

end:
FunctionEnd

Function RunAvidemux
    SetOutPath $INSTDIR

#Qt4:
    SectionGetFlags ${SecUiQt4} $0
    IntOp $0 $0 & ${SF_SELECTED}

    StrCmp $0 ${SF_SELECTED} 0 GTK
        Exec "$INSTDIR\avidemux2_qt4.exe"

    Goto end

GTK:
    SectionGetFlags ${SecUiGtk} $0
    IntOp $0 $0 & ${SF_SELECTED}

    StrCmp $0 ${SF_SELECTED} 0 End
        Exec "$INSTDIR\avidemux2_gtk.exe"

end:
FunctionEnd

##########################
# Uninstaller functions
##########################
Function un.onInit
	!insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuGroup
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
