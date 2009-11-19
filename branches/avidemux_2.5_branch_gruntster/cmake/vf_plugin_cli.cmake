MACRO(INIT_VIDEOFILTER_PLUGIN_CLI  lib  _srcsCli )

ADD_LIBRARY(${lib} SHARED ${ARGN} ${_srcsCli})
INCLUDE_DIRECTORIES(${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_UIs/ADM_CLI/include/)
ADD_TARGET_CFLAGS(${lib} "-DADM_UI_TYPE_BUILD=1")
INIT_VIDEOFILTER_PLUGIN(${lib})
INSTALL_VIDEOFILTER(${lib})
ENDMACRO(INIT_VIDEOFILTER_PLUGIN_CLI)
