##############################
# RPM
##############################
SET(CPACK_SET_DESTDIR "ON")
SET(CPACK_RPM_PACKAGE_NAME "avidemux3-cli")
SET (CPACK_GENERATOR "RPM")
# ARCH
IF (X86_64_SUPPORTED)
SET(CPACK_RPM_PACKAGE_ARCHITECTURE "amd64")
ELSE (X86_64_SUPPORTED)
SET(CPACK_RPM_PACKAGE_ARCHITECTURE "i386")
ENDIF (X86_64_SUPPORTED)
# Mandatory
SET(CPACK_RPM_PACKAGE_VENDOR "mean")
SET(CPACK_RPM_PACKAGE_DESCRIPTION "Simple video editor,main program cli version ")

# Some more infos
SET(CPACK_PACKAGE_VERSION_MAJOR "2")
SET(CPACK_PACKAGE_VERSION_MINOR "6")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0_${ADM_SUBVERSION}")
#
SET(CPACK_PACKAGE_NAME "avidemux3-cli")
#

include(CPack)
