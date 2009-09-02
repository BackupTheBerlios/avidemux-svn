SET(ADM_script_SRCS
	ADM_AvidemuxAudio.cpp  ADM_AvidemuxVideo.cpp    ADM_JSAvidemux.cpp       ADM_JSDirectorySearch.cpp  ADM_JSGlobal.cpp
	ADM_Avidemux.cpp       ADM_JSAvidemuxAudio.cpp  ADM_JSAvidemuxVideo.cpp  ADM_JSFunctions.cpp        DirectorySearch.cpp
	ADM_JSDialogFactory.cpp  ADM_JSDFMenu.cpp  ADM_JSAppliedVideoFilter.cpp  ADM_JSAudioTrackInfo.cpp)

include_directories("${PTHREAD_INCLUDE_DIR}")
ADD_ADM_LIB_ALL_TARGETS(ADM_script ${ADM_script_SRCS})

IF (WIN32)
	ADD_CFLAGS_ALL_TARGETS(ADM_script "-DXP_WIN")
ELSE (WIN32)
	ADD_CFLAGS_ALL_TARGETS(ADM_script "-DXP_UNIX")
ENDIF (WIN32)

if (USE_SYSTEM_SPIDERMONKEY)
	include_directories(${SPIDERMONKEY_INCLUDE_DIR})
else (USE_SYSTEM_SPIDERMONKEY)
	include_directories(${CMAKE_SOURCE_DIR}/avidemux/ADM_libraries/ADM_smjs)
endif (USE_SYSTEM_SPIDERMONKEY)