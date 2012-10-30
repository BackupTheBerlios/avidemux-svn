FILE(GLOB py_files ${CMAKE_CURRENT_SOURCE_DIR}/*.py)
FILE(GLOB py_libFiles ${CMAKE_CURRENT_SOURCE_DIR}/lib/*.py)

ADM_INSTALL_PLUGIN_FILES(autoScripts ${py_files})
ADM_INSTALL_PLUGIN_FILES(autoScripts/lib ${py_libFiles})
