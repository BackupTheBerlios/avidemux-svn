##########################
# Included files
##########################
!include Sections.nsh
!include MUI2.nsh
!include nsDialogs.nsh
!include Memento.nsh
!include revision.nsh
!include builddir.nsh

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
!define MUI_HEADERIMAGE_BITMAP "PageHeader.bmp"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT HKLM
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${REGKEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME StartMenuGroup
!define MUI_STARTMENUPAGE_DEFAULTFOLDER Avidemux
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_WELCOMEFINISHPAGE_BITMAP "WelcomeFinishStrip.bmp"
!define MUI_UNICON "..\..\..\avidemux\xpm\adm.ico"
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
!insertmacro MUI_PAGE_LICENSE License.rtf
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
OutFile avidemux_2.5_r${REVISION}_win32.exe
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
# Macros
##########################
!macro InstallGtkLanguage LANG_NAME LANG_CODE
	SetOverwrite on

	!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} installGtk${LANG_CODE} endGtk${LANG_CODE}

installGtk${LANG_CODE}:
    SetOutPath $INSTDIR\share\locale\${LANG_CODE}\LC_MESSAGES
    File ${BUILDDIR}\share\locale\${LANG_CODE}\LC_MESSAGES\avidemux.mo
    File ${BUILDDIR}\share\locale\${LANG_CODE}\LC_MESSAGES\gtk20.mo

endGtk${LANG_CODE}:
!macroend

!macro InstallQt4Language LANG_NAME LANG_CODE
	SetOverwrite on

	!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} installQt4${LANG_CODE} endQt4${LANG_CODE}

installQt4${LANG_CODE}:	
	SetOutPath $INSTDIR\i18n
    File ${BUILDDIR}\i18n\avidemux_${LANG_CODE}.qm
    File ${BUILDDIR}\i18n\qt_${LANG_CODE}.qm

endQt4${LANG_CODE}:
!macroend

!macro UninstallLanguage LANG_NAME LANG_CODE
    RmDir /r /REBOOTOK $INSTDIR\share\locale\${LANG_CODE}
    Delete /REBOOTOK $INSTDIR\i18n\avidemux_${LANG_CODE}.qm
    Delete /REBOOTOK $INSTDIR\i18n\qt_${LANG_CODE}.qm
!macroend

##########################
# Installer sections
##########################
Section "Core files (required)" SecCore
    SectionIn 1 2 RO
    SetOutPath $INSTDIR
    SetOverwrite on
    File "${BUILDDIR}\Build Info.txt"
    File "${BUILDDIR}\Change Log.html"
    File ${BUILDDIR}\zlib1.dll
    File ${BUILDDIR}\freetype6.dll
    File ${BUILDDIR}\iconv.dll
    File ${BUILDDIR}\intl.dll
    File ${BUILDDIR}\libADM_core.dll
    File ${BUILDDIR}\libADM_coreAudio.dll
    File ${BUILDDIR}\libADM_coreImage.dll
    File ${BUILDDIR}\libADM_coreUI.dll
    File ${BUILDDIR}\libADM_smjs.dll
    File ${BUILDDIR}\libaften.dll
    File ${BUILDDIR}\libexpat-*.dll
    File ${BUILDDIR}\libfontconfig-1.dll
    File ${BUILDDIR}\libxml2.dll
    File ${BUILDDIR}\ogg.dll
    File ${BUILDDIR}\pthreadGC2.dll
    File ${BUILDDIR}\SDL.dll
    File ${BUILDDIR}\xmltok.dll
    File ${BUILDDIR}\AUTHORS.
    File ${BUILDDIR}\COPYING.
    File ${BUILDDIR}\README.
    File ${BUILDDIR}\avcodec-*.dll
    File ${BUILDDIR}\avformat-*.dll
    File ${BUILDDIR}\avutil-*.dll
    File ${BUILDDIR}\postproc-*.dll
    File ${BUILDDIR}\swscale-*.dll
    SetOutPath $INSTDIR\etc\fonts
    File /r ${BUILDDIR}\etc\fonts\*
SectionEnd

SectionGroup /e "User interfaces" SecGrpUI
    ${MementoSection} "Command line" SecUiCli
        SectionIn 1 2
        SetOutPath $INSTDIR
        SetOverwrite on
        File ${BUILDDIR}\avidemux2_cli.exe
        File ${BUILDDIR}\libADM_render_cli.dll
        File ${BUILDDIR}\libADM_UICli.dll
    ${MementoSectionEnd}

    ${MementoUnselectedSection} GTK+ SecUiGtk
        SectionIn 2
        SetOverwrite on
        SetOutPath $INSTDIR\etc\gtk-2.0
        File /r ${BUILDDIR}\etc\gtk-2.0\*
        SetOutPath $INSTDIR\etc\pango
        File /r ${BUILDDIR}\etc\pango\*
        SetOutPath $INSTDIR\lib\gtk-2.0
        File /r ${BUILDDIR}\lib\gtk-2.0\*
        SetOutPath $INSTDIR\share\themes
        File /r ${BUILDDIR}\share\themes\*
        SetOutPath $INSTDIR
        File ${BUILDDIR}\avidemux2_gtk.exe
        File ${BUILDDIR}\gtk2_prefs.exe
        File ${BUILDDIR}\jpeg62.dll
        File ${BUILDDIR}\libADM_render_gtk.dll
        File ${BUILDDIR}\libADM_UIGtk.dll
        File ${BUILDDIR}\libatk-1.0-0.dll
        File ${BUILDDIR}\libcairo-2.dll
        File ${BUILDDIR}\libgdk_pixbuf-2.0-0.dll
        File ${BUILDDIR}\libgdk-win32-2.0-0.dll
        File ${BUILDDIR}\libgio-2.0-0.dll
        File ${BUILDDIR}\libglib-2.0-0.dll
        File ${BUILDDIR}\libgmodule-2.0-0.dll
        File ${BUILDDIR}\libgobject-2.0-0.dll
        File ${BUILDDIR}\libgthread-2.0-0.dll
        File ${BUILDDIR}\libgtk-win32-2.0-0.dll
        File ${BUILDDIR}\libpango-1.0-0.dll
        File ${BUILDDIR}\libpangocairo-1.0-0.dll
        File ${BUILDDIR}\libpangowin32-1.0-0.dll
        File ${BUILDDIR}\libpng12-0.dll
        File ${BUILDDIR}\libtiff3.dll
    ${MementoSectionEnd}

    ${MementoSection} Qt4 SecUiQt4
        SectionIn 1 2
        SetOutPath $INSTDIR
        SetOverwrite on
        File ${BUILDDIR}\QtGui4.dll
        File ${BUILDDIR}\avidemux2_qt4.exe
        File ${BUILDDIR}\libADM_render_qt4.dll
        File ${BUILDDIR}\libADM_UIQT4.dll
        File ${BUILDDIR}\mingwm10.dll
        File ${BUILDDIR}\QtCore4.dll
    ${MementoSectionEnd}
SectionGroupEnd

SectionGroup Plugins SecGrpPlugin
	SectionGroup "Audio Decoders" SecGrpAudioDecoder
		${MementoSection} "FAAD2 (AAC)" SecAudDecFaad
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioDecoder
			File ${BUILDDIR}\lib\ADM_plugins\audioDecoder\libADM_ad_faad.dll
			SetOutPath $INSTDIR
			File ${BUILDDIR}\libfaad2.dll
		${MementoSectionEnd}
		${MementoSection} "liba52 (AC-3)" SecAudDecA52
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioDecoder
			File ${BUILDDIR}\lib\ADM_plugins\audioDecoder\libADM_ad_a52.dll
		${MementoSectionEnd}
		${MementoSection} "MAD (MPEG)" SecAudDecMad
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioDecoder
			File ${BUILDDIR}\lib\ADM_plugins\audioDecoder\libADM_ad_Mad.dll
		${MementoSectionEnd}
	SectionGroupEnd
	SectionGroup "Audio Devices" SecGrpAudioDevice
		${MementoUnselectedSection} SDL SecAudDevSdl
			SectionIn 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioDevices
			File ${BUILDDIR}\lib\ADM_plugins\audioDevices\libADM_av_sdl.dll
		${MementoSectionEnd}
		${MementoSection} "MS Windows (Waveform)" SecAudDevWaveform
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioDevices
			File ${BUILDDIR}\lib\ADM_plugins\audioDevices\libADM_av_win32.dll
		${MementoSectionEnd}
	SectionGroupEnd
	SectionGroup "Audio Encoders" SecGrpAudioEncoder
		${MementoSection} "FAAC (AAC)" SecAudEncFaac
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioEncoders
			File ${BUILDDIR}\lib\ADM_plugins\audioEncoders\libADM_ae_faac.dll
			SetOutPath $INSTDIR
			File ${BUILDDIR}\libfaac.dll
		${MementoSectionEnd}
		${MementoSection} "LAME (MP3)" SecAudEncLame
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioEncoders
			File ${BUILDDIR}\lib\ADM_plugins\audioEncoders\libADM_ae_lame.dll
			SetOutPath $INSTDIR
			File ${BUILDDIR}\libmp3lame-0.dll
		${MementoSectionEnd}
		${MementoSection} "libavcodec (AC-3)" SecAudEncLavAc3
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioEncoders
			File ${BUILDDIR}\lib\ADM_plugins\audioEncoders\libADM_ae_lav_ac3.dll
		${MementoSectionEnd}
		${MementoSection} "libavcodec (MP2)" SecAudEncLavMp2
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioEncoders
			File ${BUILDDIR}\lib\ADM_plugins\audioEncoders\libADM_ae_lav_mp2.dll
		${MementoSectionEnd}
		${MementoSection} "libvorbis (Vorbis)" SecAudEncVorbis
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioEncoders
			File ${BUILDDIR}\lib\ADM_plugins\audioEncoders\libADM_ae_vorbis.dll
			SetOutPath $INSTDIR
			File ${BUILDDIR}\vorbis.dll
			File ${BUILDDIR}\vorbisenc.dll
		${MementoSectionEnd}
		${MementoSection} "PCM" SecAudEncPcm
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioEncoders
			File ${BUILDDIR}\lib\ADM_plugins\audioEncoders\libADM_ae_pcm.dll
		${MementoSectionEnd}
		${MementoSection} "TwoLAME (MP2)" SecAudEncTwoLame
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\audioEncoders
			File ${BUILDDIR}\lib\ADM_plugins\audioEncoders\libADM_ae_twolame.dll
		${MementoSectionEnd}
	SectionGroupEnd
	SectionGroup "Video Encoders" SecGrpVideoEncoder
		${MementoSection} "x264 (MPEG-4 AVC)" SecVidEncX264
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\videoEncoder
			File ${BUILDDIR}\lib\ADM_plugins\videoEncoder\libADM_vidEnc_x264.dll
			SetOutPath $INSTDIR\lib\ADM_plugins\videoEncoder\x264
			!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
			File ${BUILDDIR}\lib\ADM_plugins\videoEncoder\x264\libADM_vidEnc_x264_Gtk.dll
CheckQt4:
			!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
			File ${BUILDDIR}\lib\ADM_plugins\videoEncoder\x264\libADM_vidEnc_x264_Qt.dll
End:
			File ${BUILDDIR}\lib\ADM_plugins\videoEncoder\x264\x264Param.xsd
			File ${BUILDDIR}\lib\ADM_plugins\videoEncoder\x264\*.xml
			SetOutPath $INSTDIR
			File ${BUILDDIR}\libx264-*.dll
		${MementoSectionEnd}
		${MementoSection} "Xvid (MPEG-4 ASP)" SecVidEncXvid
			SectionIn 1 2
			SetOverwrite on
			SetOutPath $INSTDIR\lib\ADM_plugins\videoEncoder
			File ${BUILDDIR}\lib\ADM_plugins\videoEncoder\libADM_vidEnc_xvid.dll
SetOutPath $INSTDIR\lib\ADM_plugins\videoEncoder\xvid
			!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
			File ${BUILDDIR}\lib\ADM_plugins\videoEncoder\xvid\libADM_vidEnc_Xvid_Gtk.dll
CheckQt4:
			!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
			File ${BUILDDIR}\lib\ADM_plugins\videoEncoder\xvid\libADM_vidEnc_Xvid_Qt.dll
End:
			File ${BUILDDIR}\lib\ADM_plugins\videoEncoder\xvid\XvidParam.xsd
			#File ${BUILDDIR}\lib\ADM_plugins\videoEncoder\xvid\*.xml
			SetOutPath $INSTDIR
			File ${BUILDDIR}\xvidcore.dll
		${MementoSectionEnd}
	SectionGroupEnd
	SectionGroup "Video Filters" SecGrpVideoFilter
		SectionGroup "Transform Filters" SecGrpVideoFilterTransform
			${MementoSection} "Add Black Borders" SecVidFltBlackBorders
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_addborders.dll
			${MementoSectionEnd}
			${MementoSection} "Avisynth Resize" SecVidFltAvisynthResize
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_avisynthResize_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_avisynthResize_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Blacken Borders" SecVidFltBlackenBorders
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_blackenBorders.dll
			${MementoSectionEnd}
			${MementoSection} "Crop" SecVidFltCrop
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_Crop_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_crop_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Fade" SecVidFltFade
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_fade.dll
			${MementoSectionEnd}
			${MementoSection} "MPlayer Resize" SecVidFltMplayerResize
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_mplayerResize_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_mplayerResize_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Resample FPS" SecVidFltResampleFps
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_resampleFps.dll
			${MementoSectionEnd}
			${MementoSection} "Reverse" SecVidFltReverse
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_reverse.dll
			${MementoSectionEnd}
			${MementoSection} "Rotate" SecVidFltRotate
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_rotate.dll
			${MementoSectionEnd}
			${MementoSection} "Vertical Flip" SecVidFltVerticalFlip
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_vflip.dll
			${MementoSectionEnd}
		SectionGroupEnd
		SectionGroup "Interlacing Filters" SecGrpVideoFilterInterlacing
			${MementoSection} "Decomb Decimate" SecVidFltDecombDecimate
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_decimate.dll
			${MementoSectionEnd}
			${MementoSection} "Decomb Telecide" SecVidFltDecombTelecide
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_telecide.dll
			${MementoSectionEnd}
			${MementoSection} "Deinterlace" SecVidFltDeinterlace
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_Deinterlace.dll
			${MementoSectionEnd}
			${MementoSection} "DG Bob" SecVidFltDbGob
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_blendDgBob.dll
			${MementoSectionEnd}
			${MementoSection} "Drop" SecVidFltDrop
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_dropOut.dll
			${MementoSectionEnd}
			${MementoSection} "Fade" SecVidFltFadeInterlace
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_separateField.dll
			${MementoSectionEnd}
			${MementoSection} "Gauss Smooth" SecVidFltGaussSmooth
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_fastconvolutiongauss.dll
			${MementoSectionEnd}
			${MementoSection} "Keep Even Fields" SecVidFltKeepEvenFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_keepEvenField.dll
			${MementoSectionEnd}
			${MementoSection} "Keep Odd Fields" SecVidFltKeepOddFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_keepOddField.dll
			${MementoSectionEnd}
			${MementoSection} "KernelDeint" SecVidFltKernelDeint
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_kernelDeint.dll
			${MementoSectionEnd}
			${MementoSection} "libavcodec Deinterlacer" SecVidFltLavDeinterlacer
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_lavDeinterlace.dll
			${MementoSectionEnd}
			${MementoSection} "mcDeint" SecVidFltMcDeint
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_mcdeint.dll
			${MementoSectionEnd}
			${MementoSection} "Mean" SecVidFltMean
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_fastconvolutionmean.dll
			${MementoSectionEnd}
			${MementoSection} "Median" SecVidFltMedian
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_fastconvolutionmedian.dll
			${MementoSectionEnd}
			${MementoSection} "Merge Fields" SecVidFltMergeFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_mergeField.dll
			${MementoSectionEnd}
			${MementoSection} "PAL Field Shift" SecVidFltPalFieldShift
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_palShift.dll
			${MementoSectionEnd}
			${MementoSection} "PAL Smart Field Shift" SecVidFltPalSmartFieldShift
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_smartPalShift.dll
			${MementoSectionEnd}
			${MementoSection} "Pulldown" SecVidFltPulldown
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_Pulldown.dll
			${MementoSectionEnd}
			${MementoSection} "Separate Fields" SecVidFltSeparateFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_hzStackField.dll
			${MementoSectionEnd}
			${MementoSection} "Sharpen" SecVidFltSharpen
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_fastconvolutionsharpen.dll
			${MementoSectionEnd}
			${MementoSection} "Smart Swap Fields" SecVidFltSmartSwapFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_smartSwapField.dll
			${MementoSectionEnd}
			${MementoSection} "Stack Fields" SecVidFltStackFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_stackField.dll
			${MementoSectionEnd}
			${MementoSection} "Swap Fields" SecVidFltSwapFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_swapField.dll
			${MementoSectionEnd}
			${MementoSection} "TDeint" SecVidFltTDeint
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_tdeint.dll
			${MementoSectionEnd}
			${MementoSection} "Unstack Fields" SecVidFltUnstackFields
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_unstackField.dll
			${MementoSectionEnd}
			${MementoSection} "yadif" SecVidFltYadif
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_yadif.dll
			${MementoSectionEnd}
		SectionGroupEnd
		SectionGroup "Colour Filters" SecGrpVideoFilterColour
			${MementoSection} "Avisynth ColorYUV" SecVidFltAvisynthColorYuv
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_colorYUV_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_colorYUV_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Blend Removal" SecVidFltBlendRemoval
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_blendRemoval.dll
			${MementoSectionEnd}
			${MementoSection} "Chroma Shift" SecVidFltChromaShift
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_chromaShift_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_chromaShift_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Chroma U" SecVidFltChromaU
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vidChromaU.dll
			${MementoSectionEnd}
			${MementoSection} "Chroma V" SecVidFltChromaV
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vidChromaV.dll
			${MementoSectionEnd}
			${MementoSection} "Contrast" SecVidFltContrast
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_contrast_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_contrast_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Luma Delta" SecVidFltLumaDelta
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_Delta.dll
			${MementoSectionEnd}
			${MementoSection} "Luma Equaliser" SecVidFltLumaEqualiser
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_equalizer_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_equalizer_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Luma Only" SecVidFltLumaOnly
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_lumaonly.dll
			${MementoSectionEnd}
			${MementoSection} "Mplayer Eq2" SecVidFltMPlayerEq2
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_eq2_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_eq2_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Mplayer Hue" SecVidFltMPlayerHue
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_hue_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_hue_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Swap U and V" SecVidFltSwapUandV
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_swapuv.dll
			${MementoSectionEnd}
		SectionGroupEnd
		SectionGroup "Noise Filters" SecGrpVideoFilterNoise
			${MementoSection} "Cnr2" SecVidFltCnr2
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_cnr2_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_cnr2_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Denoise" SecVidFltDenoise
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_Denoise.dll
			${MementoSectionEnd}
			${MementoSection} "FluxSmooth" SecVidFltFluxSmooth
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_FluxSmooth.dll
			${MementoSectionEnd}
			${MementoSection} "Forced Post-processing" SecVidFltForcedPostProcessing
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_forcedPP.dll
			${MementoSectionEnd}
			${MementoSection} "Light Denoiser" SecVidFltLightDenoiser
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_Stabilize.dll
			${MementoSectionEnd}
			${MementoSection} "Median (5x5)" SecVidFltMediam5x5
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_largemedian.dll
			${MementoSectionEnd}
			${MementoSection} "MPlayer Denoise3d" SecVidFltMPlayerDenoise3d
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_denoise3d.dll
			${MementoSectionEnd}
			${MementoSection} "MPlayer Hqdn3d" SecVidFltMPlayerHqdn3d
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_denoise3dhq.dll
			${MementoSectionEnd}
			${MementoSection} "Temporal Cleaner" SecVidFltTemporalCleaner
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_vlad.dll
			${MementoSectionEnd}
			${MementoSection} "TIsophote" SecVidFltTisophote
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_Tisophote.dll
			${MementoSectionEnd}
		SectionGroupEnd
		SectionGroup "Sharpness Filters" SecGrpVideoFilterSharpness
			${MementoSection} "asharp" SecVidFltAsharp
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_asharp_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_asharp_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "MSharpen" SecVidFltMSharpen
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_mSharpen.dll
			${MementoSectionEnd}
			${MementoSection} "MSmooth" SecVidFltMSmooth
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_mSmooth.dll
			${MementoSectionEnd}
			${MementoSection} "Soften" SecVidFltSoften
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_soften.dll
			${MementoSectionEnd}
		SectionGroupEnd
		SectionGroup "Subtitle Filters" SecGrpVideoFilterSubtitle
			${MementoSection} "ASS/SSA" SecVidFltAssSsa
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_ssa.dll
			${MementoSectionEnd}
			${MementoSection} "Subtitler" SecVidFltSubtitler
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_sub_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_sub_qt4.dll
End:
			${MementoSectionEnd}
		SectionGroupEnd
		SectionGroup "Miscellaneous Filters" SecGrpVideoFilterMiscellaneous
			${MementoSection} "Mosaic" SecVidFltMosaic
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_Mosaic.dll
			${MementoSectionEnd}
			${MementoSection} "MPlayer Delogo" SecVidFltMPlayerDelogo
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter

				!insertmacro SectionFlagIsSet ${SecUiGtk} ${SF_SELECTED} InstallGtk CheckQt4
InstallGtk:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_mpdelogo_gtk.dll
CheckQt4:
				!insertmacro SectionFlagIsSet ${SecUiQt4} ${SF_SELECTED} InstallQt4 End
InstallQt4:
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_mpdelogo_qt4.dll
End:
			${MementoSectionEnd}
			${MementoSection} "Whirl" SecVidFltWhirl
				SectionIn 1 2
				SetOverwrite on
				SetOutPath $INSTDIR\lib\ADM_plugins\videoFilter
				File ${BUILDDIR}\lib\ADM_plugins\videoFilter\libADM_vf_Whirl.dll
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
    File /r ${BUILDDIR}\avsproxy.exe
    File /r ${BUILDDIR}\avsproxy_gui.exe
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

##########################
# Uninstaller sections
##########################
Section /o "un.AvsProxy" UnSecAvsProxy
    Delete /REBOOTOK $INSTDIR\avsproxy.exe
    Delete /REBOOTOK $INSTDIR\avsproxy_gui.exe
SectionEnd

Section /o un.Catalan UnSecLangCatalan
	!insertmacro UninstallLanguage Catalan ca
SectionEnd

Section /o un.Czech UnSecLangCzech
	!insertmacro UninstallLanguage Czech cs
SectionEnd

Section /o un.French UnSecLangFrench
	!insertmacro UninstallLanguage French fr
SectionEnd

Section /o un.German UnSecLangGerman
	!insertmacro UninstallLanguage German de
SectionEnd

Section /o un.Greek UnSecLangGreek
	!insertmacro UninstallLanguage Greek el
SectionEnd

Section /o un.Italian UnSecLangItalian
	!insertmacro UninstallLanguage Italian it
SectionEnd

Section /o un.Japanese UnSecLangJapanese
	!insertmacro UninstallLanguage Japanese ja
SectionEnd

Section /o un.Russian UnSecLangRussian
	!insertmacro UninstallLanguage Russian ru
SectionEnd

Section /o un.Serbian UnSecLangSerbianCyrillic
	!insertmacro UninstallLanguage SerbianCyrillic sr
SectionEnd

Section /o un.SerbianLatin UnSecLangSerbianLatin
	!insertmacro UninstallLanguage SerbianLatin sr@latin
SectionEnd

Section /o un.Spanish UnSecLangSpanish
	!insertmacro UninstallLanguage Spanish es
SectionEnd

Section /o un.Turkish UnSecLangTurkish
	!insertmacro UninstallLanguage Turkish tr
SectionEnd

Section /o un.GTK+ UnSecUiGtk
	Delete /REBOOTOK $INSTDIR\libtiff3.dll
	Delete /REBOOTOK $INSTDIR\libpng12-0.dll
    Delete /REBOOTOK $INSTDIR\libpangowin32-1.0-0.dll
    Delete /REBOOTOK $INSTDIR\libpangocairo-1.0-0.dll
    Delete /REBOOTOK $INSTDIR\libpango-1.0-0.dll
    Delete /REBOOTOK $INSTDIR\libgio-2.0-0.dll
    Delete /REBOOTOK $INSTDIR\libglib-2.0-0.dll
    Delete /REBOOTOK $INSTDIR\libgtk-win32-2.0-0.dll
    Delete /REBOOTOK $INSTDIR\libgthread-2.0-0.dll
    Delete /REBOOTOK $INSTDIR\libgobject-2.0-0.dll
    Delete /REBOOTOK $INSTDIR\libgmodule-2.0-0.dll
    Delete /REBOOTOK $INSTDIR\libgdk-win32-2.0-0.dll
    Delete /REBOOTOK $INSTDIR\libgdk_pixbuf-2.0-0.dll
    Delete /REBOOTOK $INSTDIR\libcairo-2.dll
    Delete /REBOOTOK $INSTDIR\libatk-1.0-0.dll
    Delete /REBOOTOK $INSTDIR\jpeg62.dll
    Delete /REBOOTOK $INSTDIR\gtk2_prefs.exe
    Delete /REBOOTOK $INSTDIR\avidemux2_gtk.exe
    RmDir /r /REBOOTOK $INSTDIR\share\themes
    RmDir /r /REBOOTOK $INSTDIR\lib\gtk-2.0
    RmDir /r /REBOOTOK $INSTDIR\etc\gtk-2.0
    RmDir /r /REBOOTOK $INSTDIR\etc\pango
SectionEnd

Section /o un.Qt4 UnSecUiQt4
    Delete /REBOOTOK $INSTDIR\QtCore4.dll
    Delete /REBOOTOK $INSTDIR\mingwm10.dll
    Delete /REBOOTOK $INSTDIR\avidemux2_qt4.exe
    Delete /REBOOTOK $INSTDIR\QtGui4.dll
SectionEnd

Section /o "un.Command line" UnSecUiCli
    Delete /REBOOTOK $INSTDIR\avidemux2_cli.exe
SectionEnd

Section /o "un.Core files (required)" UnSecCore
    Delete /REBOOTOK $INSTDIR\xvidcore.dll
    Delete /REBOOTOK $INSTDIR\xmltok.dll
    Delete /REBOOTOK $INSTDIR\vorbisenc.dll
    Delete /REBOOTOK $INSTDIR\vorbis.dll
    Delete /REBOOTOK $INSTDIR\SDL.dll
    Delete /REBOOTOK $INSTDIR\pthreadGC2.dll
    Delete /REBOOTOK $INSTDIR\ogg.dll
    Delete /REBOOTOK $INSTDIR\libxml2.dll
    Delete /REBOOTOK $INSTDIR\libx264-*.dll
    Delete /REBOOTOK $INSTDIR\libmp3lame-0.dll
    Delete /REBOOTOK $INSTDIR\libfontconfig-1.dll
    Delete /REBOOTOK $INSTDIR\libfaad2.dll
    Delete /REBOOTOK $INSTDIR\libfaac.dll
    Delete /REBOOTOK $INSTDIR\libexpat-*.dll
    Delete /REBOOTOK $INSTDIR\libaften.dll
    Delete /REBOOTOK $INSTDIR\intl.dll
    Delete /REBOOTOK $INSTDIR\iconv.dll
    Delete /REBOOTOK $INSTDIR\freetype6.dll
    Delete /REBOOTOK $INSTDIR\zlib1.dll
    Delete /REBOOTOK "$INSTDIR\Build Info.txt"
    Delete /REBOOTOK "$INSTDIR\Change Log.html"
    Delete /REBOOTOK "$INSTDIR\stdout.txt"
    Delete /REBOOTOK "$INSTDIR\stderr.txt"
    RmDir /r /REBOOTOK $INSTDIR\etc\fonts

    RmDir /REBOOTOK $INSTDIR\share\gettext
    RmDir /REBOOTOK $INSTDIR\share
SectionEnd

Section /o "un.Start menu" UnSecStartMenu
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\Change Log.lnk"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\${INTERNALNAME} GTK+.lnk"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\${INTERNALNAME} Qt4.lnk"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\AVS Proxy GUI.lnk"
SectionEnd

Section /o "un.Quick Launch" UnSecQuickLaunch
    Delete /REBOOTOK "$QUICKLAUNCH\${INTERNALNAME} GTK+.lnk"
    Delete /REBOOTOK "$QUICKLAUNCH\${INTERNALNAME} Qt4.lnk"
SectionEnd

Section /o "un.Desktop" UnSecDesktop
    Delete /REBOOTOK "$DESKTOP\${INTERNALNAME} GTK+.lnk"
    Delete /REBOOTOK "$DESKTOP\${INTERNALNAME} Qt4.lnk"
SectionEnd

Section un.post UnSecUninstaller
    RmDir /REBOOTOK $INSTDIR\etc
    RmDir /REBOOTOK $INSTDIR\i18n
    RmDir /REBOOTOK $INSTDIR\lib
    RmDir /REBOOTOK $INSTDIR\share\locale
    RmDir /REBOOTOK $INSTDIR\share
    
    DeleteRegKey HKLM "${UNINST_REGKEY}"
    Delete /REBOOTOK $INSTDIR\uninstall.exe
    DeleteRegValue HKLM "${REGKEY}" StartMenuGroup
    DeleteRegValue HKLM "${REGKEY}" Path
    DeleteRegKey /IfEmpty HKLM "${REGKEY}"
    RmDir /REBOOTOK $SMPROGRAMS\$StartMenuGroup
    RmDir /REBOOTOK $INSTDIR
    Push $R0
    StrCpy $R0 $StartMenuGroup 1
    StrCmp $R0 ">" no_smgroup
no_smgroup:
    Pop $R0
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
    ReadRegStr $INSTDIR HKLM "${REGKEY}" Path
    !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuGroup
FunctionEnd
