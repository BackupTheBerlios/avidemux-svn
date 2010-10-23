include(admCheckLibxml2)

set(ADM_vidEnc_pluginOptions_SRCS  PluginOptions.cpp  PluginXmlOptions.cpp)

include_directories("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_core/include"
					"${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_coreUI/include"
					"${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_plugin"
					"${LIBXML2_INCLUDE_DIR}")

add_definitions(${LIBXML2_DEFINITIONS})
add_library(ADM_vidEnc_pluginOptions ${ADM_vidEnc_pluginOptions_SRCS})
target_link_libraries(ADM_vidEnc_pluginOptions ${LIBXML2_LIBRARIES} ADM_core)

if (UNIX)
	add_target_cflags(ADM_vidEnc_pluginOptions -fPIC)
endif (UNIX)