if (WIN32)
	SET(scriptInstallDir ${CMAKE_INSTALL_PREFIX}/scripts)
else (WIN32)
	SET(scriptInstallDir ${CMAKE_INSTALL_PREFIX}/share/ADM_scripts)
endif (WIN32)

install(DIRECTORY . DESTINATION "${scriptInstallDir}" FILES_MATCHING PATTERN "*.js" PATTERN ".svn" EXCLUDE)