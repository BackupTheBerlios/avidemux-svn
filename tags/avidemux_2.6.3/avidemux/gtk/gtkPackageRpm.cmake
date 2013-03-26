##############################                                                                                                                                                     
# RPM                                                                                                                                                                              
##############################                                                                                                                                                     
SET(CPACK_SET_DESTDIR "ON")
SET(CPACK_RPM_PACKAGE_NAME "avidemux3-gtk")
SET (CPACK_GENERATOR "RPM")
# ARCH                                                                                                                                                                             
IF (X86_64_SUPPORTED)
SET(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
ELSE (X86_64_SUPPORTED)
SET(CPACK_RPM_PACKAGE_ARCHITECTURE "i386")
ENDIF (X86_64_SUPPORTED)
# Mandatory                                                                                                                                                                        
SET(CPACK_RPM_PACKAGE_VENDOR "mean")
SET(CPACK_RPM_PACKAGE_DESCRIPTION "Simple video editor,main program gtk version ")
# Some more infos                                                                                                                                                                  
#                                                                                                                                                                                  
SET(CPACK_PACKAGE_NAME "avidemux3-gtk")
#                                                                                                                                                                                  
SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}.${CPACK_RPM_PACKAGE_ARCHITECTURE}")
include(CPack)

