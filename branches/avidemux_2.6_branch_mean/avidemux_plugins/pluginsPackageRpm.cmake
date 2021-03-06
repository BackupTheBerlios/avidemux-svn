##############################
# RPM
##############################
SET(CPACK_SET_DESTDIR "ON")
SET(CPACK_RPM_PACKAGE_NAME "avidemux3-plugins-${PLUGIN_UI}")
SET (CPACK_GENERATOR "RPM")
# ARCH
IF (X86_64_SUPPORTED)
SET(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
ELSE (X86_64_SUPPORTED)
SET(CPACK_RPM_PACKAGE_ARCHITECTURE "i386")
ENDIF (X86_64_SUPPORTED)
# Mandatory
SET(CPACK_RPM_PACKAGE_VERSION "${AVIDEMUX_VERSION}")
SET(CPACK_RPM_PACKAGE_RELEASE "1.r${ADM_SUBVERSION}.bootstrap")
SET(CPACK_RPM_PACKAGE_LICENSE "GPLv2+")
SET(CPACK_RPM_PACKAGE_GROUP "System Environment/Libraries")
SET(CPACK_RPM_PACKAGE_VENDOR "mean")
SET(CPACK_RPM_PACKAGE_DESCRIPTION "This package contains the runtime libraries for avidemux.")

#
SET(CPACK_RPM_PACKAGE_SECTION "extra")
SET(CPACK_RPM_PACKAGE_PRIORITY "optional")
# Some more infos
#
SET(CPACK_RPM_PACKAGE_SUMMARY "Libraries for avidemux")
SET(CPACK_RPM_PACKAGE_REQUIRES "avidemux3-core%{?_isa} >= ${CPACK_RPM_PACKAGE_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}")
#
SET(CPACK_PACKAGE_FILE_NAME "${CPACK_RPM_PACKAGE_NAME}-${CPACK_RPM_PACKAGE_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}.${CPACK_RPM_PACKAGE_ARCHITECTURE}")

include(CPack)
