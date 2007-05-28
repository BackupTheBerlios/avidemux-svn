###############################
# Part Of avidemux cmake conf #
###############################
##
INCLUDE(CheckIncludeFiles)
INCLUDE(CheckSymbolExists)
INCLUDE(CheckFunctionExists)
INCLUDE(CheckLibraryExists)
include(lavcodec)
include(CMakeDetermineSystem)
include(adm_checkHeaderLib)
#INCLUDE(CheckStructMember)
check_function_exists(gettimeofday    HAVE_GETTIMEOFDAY)
# Header
CHECK_INCLUDE_FILES(inttypes.h      HAVE_INTTYPES_H)                    # simapi.h
CHECK_INCLUDE_FILES(stddef.h        HAVE_STDDEF_H)                      # simapi.h
CHECK_INCLUDE_FILES(stdint.h        HAVE_STDINT_H)                      # simapi.h
CHECK_INCLUDE_FILES(stdlib.h        HAVE_STDLIB_H)                      # simapi.h
CHECK_INCLUDE_FILES(string.h        HAVE_STRING_H)                      # _core/libintl.cpp
CHECK_INCLUDE_FILES(sys/stat.h      HAVE_SYS_STAT_H)                    # gpg/gpg.cpp
CHECK_INCLUDE_FILES(sys/types.h     HAVE_SYS_TYPES_H)                   # simapi.h
CHECK_INCLUDE_FILES(unistd.h        HAVE_UNISTD_H)                      # simapi.h
CHECK_INCLUDE_FILES(malloc.h        HAVE_MALLOC_H)                      # simapi.h
# Set lavcodec/util/format configuration
SET_LAVCODEC_FLAGS()

# Symbols
CHECK_SYMBOL_EXISTS(strcasecmp  "strings.h"         HAVE_STRCASECMP)    # simapi.h, various

# Functions
IF(NOT WIN32) # there is a chmod function on win32, but not usable the way we want...
  CHECK_FUNCTION_EXISTS(chmod     HAVE_CHMOD)         # __homedir/homedir.cpp, gpg/gpg.cpp
ENDIF(NOT WIN32)
# VERSION(s)
SET(VERSION 2.4)
SET(PACKAGE_VERSION 2.4)

# We use FFMPEG
SET(USE_MP3    1)
SET(USE_AC3    1)
SET(USE_FFMPEG    1)
SET(USE_MJPEG    1)
SET(USE_LIBXML2    1)
SET(HAVE_LRINTF    1)
SET(EMULATE_FAST_INT    1)
SET(RUNTIME_CPUDETECT    1)
########################################
# WIN32
########################################
if(WIN32)
SET(WIN32_DEF 1)
else(WIN32)
SET(WIN32_DEF 0)
endif(WIN32)

########################################
# ALSA
########################################
if(NOT WIN32)
  MESSAGE(STATUS "<Checking for ALSA>")
  include(FindAlsa)
  if(ALSA_FOUND)
    ALSA_VERSION_STRING(alsaVersion)
    MESSAGE("Found alsa version :${alsaVersion}") 
    MESSAGE("Found alsa lib     :${ASOUND_LIBRARY}")
    SET(ALSA_SUPPORT 1)
    SET(ALSA_1_0_SUPPORT 1)
  endif(ALSA_FOUND)
endif(NOT WIN32)
########################################
# SDL
########################################
MESSAGE(STATUS "<Checking for SDL>")
include(FindSDL)
if(SDL_FOUND)
 SET(USE_SDL 1)
 MESSAGE(STATUS "Found")
else(SDL_FOUND)
  MESSAGE(STATUS "Not Found")
endif(SDL_FOUND)
########################################
# FONTCONFIG
########################################
ADM_CHECK_HL(FontConfig fontconfig/fontconfig.h fontconfig FcStrSetCreate USE_FONTCONFIG)
if(USE_FONTCONFIG)
SET(HAVE_FONTCONFIG 1)
endif(USE_FONTCONFIG)
########################################
# Xvideo
########################################
if(NOT WIN32)
ADM_CHECK_HL(Xvideo X11/extensions/Xvlib.h Xv XvShmPutImage USE_XV)
endif(NOT WIN32)
########################################
# OSS
########################################

if(NOT WIN32)
MESSAGE(STATUS "<Checking for OSS>")
CHECK_INCLUDE_FILES(sys/soundcard.h      OSS_SUPPORT)
if(OSS_SUPPORT)
MESSAGE(STATUS "Found")
else(OSS_SUPPORT)
MESSAGE(STATUS "Not found")
endif(OSS_SUPPORT)

endif(NOT WIN32)
########################################
# ESD
########################################
if(NOT WIN32)
ADM_CHECK_HL(Esd esd.h  esd esd_close USE_ESD)
endif(NOT WIN32)

########################################
# LAME
########################################
ADM_CHECK_HL(Lame lame/lame.h mp3lame lame_init HAVE_LIBMP3LAME)
########################################
# Xvid
########################################
ADM_CHECK_HL(Xvid xvid.h xvidcore xvid_plugin_single USE_XVID_4)

########################################
# X264
########################################
ADM_CHECK_HL(x264 x264.h x264 x264_encoder_open USE_X264)
########################################
# PNG
########################################
ADM_CHECK_HL(libPNG png.h png png_malloc USE_PNG)
########################################
# FAAC
########################################
ADM_CHECK_HL(FAAC faac.h faac faacEncClose USE_FAAC)
########################################
# Vorbis
########################################
ADM_CHECK_HL(Vorbis vorbisenc.h vorbis vorbis_info_init USE_VORBIS1)
ADM_CHECK_HL(Vorbis vorbisenc.h vorbisenc vorbis_encode_init USE_VORBIS2)
if(USE_VORBIS1 AND USE_VORBIS2)
  SET(USE_VORBIS 1)
endif(USE_VORBIS1 AND USE_VORBIS2)
########################################
# FAAD
########################################
ADM_CHECK_HL(FAAD faad.h faad faacDecInit USE_FAAD)
if(NOT USE_FAAD)
 ADM_CHECK_HL(NeAAC neaacdec.h faad NeAACDecInit USE_FAAD)
endif(NOT USE_FAAD)
# FIXME
# FIXME
# FIXME
SET(OLD_FAAD_PROTO 1)
#SET(ICONV_NEED_CONST 1)
########################################
# FreeType
########################################
if(FT_FOUND)
SET(USE_FREETYPE 1)
endif(FT_FOUND)
########################################
# CPU and Host
########################################
SET(HAVE_AUDIO    1)
MESSAGE("Checking CPU and OS")
MESSAGE("<CPU:${CMAKE_SYSTEM_PROCESSOR}>")
# windows is always X86 for now on...
 if(WIN32)
   MESSAGE(STATUS WIN32)
   SET(ADM_WIN32 1)
   SET(CYG_MANGLING 1)
   SET(ARCH_X86    1)
   SET(HAVE_MMX    1)
   SET(ARCH_X86    1)
   SET(HAVE_MMX    1)
   SET(ARCH_X86_32    1)
   SET(FPM_INTEL    1)
   SET(MEMALIGN_HACK    1)
   add_definitions(-mms-bitfields -mno-cygwin)
 else(WIN32)
      if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i586" OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i686")
          MESSAGE(STATUS "x86 CPU")
          SET(ARCH_X86    1)
          SET(HAVE_MMX    1)
          SET(ARCH_X86_32    1)
          SET(FPM_INTEL    1)
     endif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i586" OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i686")
     if( ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64" OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "amd64")
         MESSAGE(STATUS "AMD64 CPU")
         SET(ARCH_X86    1)
         SET(ARCH_X86_64    1)
         SET(FPM_DEFAULT    1)
         SET(ARCH_64_BITS    1)
         SET(HAVE_MMX    1)
     endif( ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64" OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "amd64")
 endif(WIN32)
MESSAGE("End of CPU and OS Check")
#
SET(ADM_DEBUG 1)
# EOF
