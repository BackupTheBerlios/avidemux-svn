#ifndef ADM_CONFIG_H
#define ADM_CONFIG_H

#include "../ADM_coreConfig.h"

#define ADM_BUILD_CLI 1
#define ADM_BUILD_GTK 2
#define ADM_BUILD_QT4 3

/* Jog Shuttle */
#cmakedefine USE_JOG

#define PACKAGE   "avidemux"
#define ADMLOCALE "${ADM_LOCALE}"

#if ${CONFIG_HEADER_TYPE} == ADM_BUILD_GTK || ${CONFIG_HEADER_TYPE} == ADM_BUILD_QT4
/* use ALSA as possible audio device */
#cmakedefine ALSA_SUPPORT
#endif

#if ${CONFIG_HEADER_TYPE} == ADM_BUILD_CLI || ${CONFIG_HEADER_TYPE} == ADM_BUILD_GTK
/* Define if the GNU gettext() function is already present or preinstalled. */
#cmakedefine HAVE_GETTEXT
#endif

// GTK+ uses X11 framework
#cmakedefine HAVE_GTK_X11

/* OSS detected */
#cmakedefine OSS_SUPPORT

/* use Aften AC3 encoder */
#cmakedefine USE_AFTEN
#cmakedefine USE_AFTEN_07	// 0.07
#cmakedefine USE_AFTEN_08	// 0.0.8
#cmakedefine USE_AFTEN_08_SVN	// 0.0.8_svn

#if ${CONFIG_HEADER_TYPE} == ADM_BUILD_GTK || ${CONFIG_HEADER_TYPE} == ADM_BUILD_QT4
/* aRts detected */
#cmakedefine USE_ARTS
#endif

#if ${CONFIG_HEADER_TYPE} == ADM_BUILD_GTK || ${CONFIG_HEADER_TYPE} == ADM_BUILD_QT4
/* ESD detected */
#cmakedefine USE_ESD
#endif

/* Jack detected */
#cmakedefine USE_JACK

/* FFmpeg */
#define USE_FFMPEG

/* Libxml2 is available */
#cmakedefine USE_LIBXML2

#if ${CONFIG_HEADER_TYPE} == ADM_BUILD_GTK || ${CONFIG_HEADER_TYPE} == ADM_BUILD_QT4
/* SDL detected */
#cmakedefine USE_SDL
#endif

#if ${CONFIG_HEADER_TYPE} == ADM_BUILD_GTK || ${CONFIG_HEADER_TYPE} == ADM_BUILD_QT4
/* XVideo detected */
#cmakedefine USE_XV
#endif

#cmakedefine USE_VPX
/* Version number of package */
#define VERSION "${VERSION}"

/* use Nvwa memory leak detector */
#cmakedefine FIND_LEAKS

#if defined(OSS_SUPPORT) || defined (USE_ARTS) || defined(USE_SDL) || defined(__APPLE__) || defined(__WIN32) || defined(ALSA_SUPPORT)
#define HAVE_AUDIO
#endif

#define HAVE_ENCODER

// FIXME - start
#ifdef HAVE_GETTEXT
#  include <libintl.h>
#  undef _
#endif

extern const char* translate(const char *__domainname, const char *__msgid);

#ifdef QT_TR_NOOP
#undef QT_TR_NOOP
#endif

#define QT_TR_NOOP(String) translate (PACKAGE, String)
// FIXME - end

#endif
