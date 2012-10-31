MACRO(checkFridibi minVersion)
	OPTION(FRIDIBI "" ON)

	MESSAGE(STATUS "Checking for FriBidi (want ${minVersion})")
	MESSAGE(STATUS "**************************************")

	IF (FRIDIBI)
		include(admCheckPkgConfig)

		if (PKG_CONFIG_FOUND)
			PKG_CHECK_MODULES(FRIDIBI fribidi)
		endif (PKG_CONFIG_FOUND)

		if (NOT FRIDIBI_FOUND)
			message("Checking for header and lib")
			unset(FRIDIBI_FOUND CACHE)
			FIND_HEADER_AND_LIB(FRIDIBI "" fribidi fribidi_get_bidi_type)

			if (FRIDIBI_FOUND)
				set(FRIDIBI_VERSION ${minVersion})
				set(FRIDIBI_LDFLAGS ${FRIDIBI_LIBRARY_DIR})
				set(FRIDIBI_CFLAGS "-I${FRIDIBI_INCLUDE_DIR}")
			endif (FRIDIBI_FOUND)
		endif (NOT FRIDIBI_FOUND)

		PRINT_LIBRARY_INFO("Fribidi" FRIDIBI_FOUND "${FRIDIBI_CFLAGS}" "${FRIDIBI_LDFLAGS}")

		IF (FRIDIBI_FOUND)
			# Warning does not work if we ask 0.19 and get 0.20
			MESSAGE(STATUS "Fridibi version ${FRIDIBI_VERSION}")

			IF("${FRIDIBI_VERSION}" MATCHES "${minVersion}.*")
				SET(USE_FRIDIBI 1)
			ELSE("${FRIDIBI_VERSION}" MATCHES "${minVersion}.*")
				MESSAGE(STATUS "Your version of FriBidi is too old, we need ${minVersion}.*")
			ENDIF("${FRIDIBI_VERSION}" MATCHES "${minVersion}.*")
		ENDIF (FRIDIBI_FOUND)
	ELSE (FRIDIBI)
		MESSAGE("${MSG_DISABLE_OPTION}")
	ENDIF (FRIDIBI)

	APPEND_SUMMARY_LIST("Miscellaneous" "FRIBIDI" "${FRIDIBI_FOUND}")

	MESSAGE("")
ENDMACRO(checkFridibi)
