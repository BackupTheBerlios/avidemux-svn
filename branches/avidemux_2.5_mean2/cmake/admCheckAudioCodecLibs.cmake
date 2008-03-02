########################################
# Aften
########################################
OPTION(AFTEN "" ON)

MESSAGE(STATUS "Checking for Aften")
MESSAGE(STATUS "******************")

IF (AFTEN)
	FIND_HEADER_AND_LIB(AFTEN aften/aften.h aften aften_encode_init)
	PRINT_LIBRARY_INFO("Aften" AFTEN_FOUND "${AFTEN_INCLUDE_DIR}" "${AFTEN_LIBRARY_DIR}")

	IF (AFTEN_FOUND)
		SET(USE_AFTEN 1)
		
		IF (NOT DEFINED AFTEN_TEST_RUN_RESULT)
			TRY_RUN(AFTEN_TEST_RUN_RESULT
				AFTEN_TEST_COMPILE_RESULT
				${CMAKE_BINARY_DIR}
				"${CMAKE_SOURCE_DIR}/cmake_compile_check/aften_check.cpp"
				CMAKE_FLAGS "-DINCLUDE_DIRECTORIES=${AFTEN_INCLUDE_DIR}" "-DLINK_LIBRARIES=${AFTEN_LIBRARY_DIR}")
		ENDIF (NOT DEFINED AFTEN_TEST_RUN_RESULT)

		IF (AFTEN_TEST_RUN_RESULT EQUAL 8)
			MESSAGE(STATUS "  version: 0.0.8")
			SET(USE_AFTEN_08 1)
		ELSEIF (AFTEN_TEST_RUN_RESULT EQUAL 7)
			MESSAGE(STATUS "  version: 0.07")
			SET(USE_AFTEN_07 1)
		ELSE (AFTEN_TEST_RUN_RESULT EQUAL 8)
			MESSAGE(STATUS "Warning: Unable to determine Aften version - support for Aften will be turned off")
			SET(USE_AFTEN 0)
		ENDIF (AFTEN_TEST_RUN_RESULT EQUAL 8)
	ENDIF (AFTEN_FOUND)
ELSE (AFTEN)
	MESSAGE("${MSG_DISABLE_OPTION}")
ENDIF (AFTEN)

MESSAGE("")

########################################
# amrnb
########################################
OPTION(AMRNB "" ON)

MESSAGE(STATUS "Checking for amrnb")
MESSAGE(STATUS "******************")

IF (AMRNB)
	IF (LATE_BINDING)
		FIND_HEADER_AND_LIB(AMRNB amrnb/interf_dec.h)
	ELSE (LATE_BINDING)
		FIND_HEADER_AND_LIB(AMRNB amrnb/interf_dec.h amrnb GP3Decoder_Interface_Decode)
	ENDIF (LATE_BINDING)

	PRINT_LIBRARY_INFO("amrnb" AMRNB_FOUND "${AMRNB_INCLUDE_DIR}" "${AMRNB_LIBRARY_DIR}")

	IF (AMRNB_FOUND)
		SET(USE_AMR_NB 1)
		SET(CONFIG_AMR_NB 1)
	ENDIF (AMRNB_FOUND)
ELSE (AMRNB)
	MESSAGE("${MSG_DISABLE_OPTION}")
ENDIF (AMRNB)

MESSAGE("")

########################################
# LAME
########################################
OPTION(LAME "" ON)

MESSAGE(STATUS "Checking for LAME")
MESSAGE(STATUS "*****************")

IF (LAME)
	IF (ADM_OS_LINUX)
		SET(LAME_REQUIRED_FLAGS "-lm")
	ENDIF (ADM_OS_LINUX)

	FIND_HEADER_AND_LIB(LAME lame/lame.h mp3lame lame_init ${LAME_REQUIRED_FLAGS})
	PRINT_LIBRARY_INFO("LAME" LAME_FOUND "${LAME_INCLUDE_DIR}" "${LAME_LIBRARY_DIR}")

	IF (LAME_FOUND)
		SET(HAVE_LIBMP3LAME 1)
	ENDIF (LAME_FOUND)
ELSE (LAME)
	MESSAGE("${MSG_DISABLE_OPTION}")
ENDIF (LAME)

MESSAGE("")

########################################
# libdca
########################################
OPTION(LIBDCA "" ON)

MESSAGE(STATUS "Checking for libdca")
MESSAGE(STATUS "*******************")

IF (LIBDCA)
	IF (ADM_OS_LINUX)
		SET(LIBDCA_REQUIRED_FLAGS "-lm")
	ENDIF (ADM_OS_LINUX)

	SET(LIBDCA_VERSION 005)

	IF (USE_LATE_BINDING)
		FIND_HEADER_AND_LIB(LIBDCA_005 dca.h)

		IF (NOT LIBDCA_005_FOUND)
			SET(LIBDCA_VERSION 002)
			FIND_HEADER_AND_LIB(LIBDCA_002 dts.h)
		ENDIF (NOT LIBDCA_005_FOUND)
	ELSE (USE_LATE_BINDING)
		FIND_HEADER_AND_LIB(LIBDCA_005 dca.h dca dca_init ${LIBDCA_REQUIRED_FLAGS})

		IF (NOT LIBDCA_005_FOUND)
			SET(LIBDCA_VERSION 002)
			FIND_HEADER_AND_LIB(LIBDCA_002 dts.h dts dts_init ${LIBDCA_REQUIRED_FLAGS})
		ENDIF (NOT LIBDCA_005_FOUND)
	ENDIF (USE_LATE_BINDING)

	IF (LIBDCA_${LIBDCA_VERSION}_FOUND)
		SET(LIBDCA_FOUND 1)
		SET(USE_LIBDCA 1)
		SET(USE_LIBDCA_${LIBDCA_VERSION} 1)
		SET(LIBDCA_INCLUDE_DIR "${LIBDCA_${LIBDCA_VERSION}_INCLUDE_DIR}")
		SET(LIBDCA_LIBRARY_DIR "${LIBDCA_${LIBDCA_VERSION}_LIBRARY_DIR}")
	ENDIF (LIBDCA_${LIBDCA_VERSION}_FOUND)

	PRINT_LIBRARY_INFO("libdca" LIBDCA_FOUND "${LIBDCA_INCLUDE_DIR}" "${LIBDCA_LIBRARY_DIR}")
ELSE (LIBDCA)
	MESSAGE("${MSG_DISABLE_OPTION}")
ENDIF (LIBDCA)

MESSAGE("")

########################################
# FAAC
########################################
OPTION(FAAC "" ON)

MESSAGE(STATUS "Checking for FAAC")
MESSAGE(STATUS "*****************")

IF (FAAC)
	FIND_HEADER_AND_LIB(FAAC faac.h faac faacEncClose)
	PRINT_LIBRARY_INFO("FAAC" FAAC_FOUND "${FAAC_INCLUDE_DIR}" "${FAAC_LIBRARY_DIR}")

	IF (FAAC_FOUND)
		SET(USE_FAAC 1)
	ENDIF (FAAC_FOUND)
ELSE (FAAC)
	MESSAGE("${MSG_DISABLE_OPTION}")
ENDIF (FAAC)

MESSAGE("")

########################################
# FAAD
########################################
OPTION (FAAD "" ON)

MESSAGE(STATUS "Checking for FAAD")
MESSAGE(STATUS "*****************")

IF (FAAD)
	FIND_HEADER_AND_LIB(FAAD faad.h faad)

	IF (FAAD_FOUND)
		ADM_CHECK_FUNCTION_EXISTS(faacDecInit "${FAAD_LIBRARY_DIR}" FAACDEC_FUNCTION_FOUND)

		IF (NOT FAACDEC_FUNCTION_FOUND)
			ADM_CHECK_FUNCTION_EXISTS(NeAACDecInit "${FAAD_LIBRARY_DIR}" NEAACDEC_FUNCTION_FOUND)

			IF (NOT NEAACDEC_FUNCTION_FOUND)
				SET(FAAD_FOUND 0)
			ENDIF (NOT NEAACDEC_FUNCTION_FOUND)
		ENDIF (NOT FAACDEC_FUNCTION_FOUND)
	ENDIF (FAAD_FOUND)

	PRINT_LIBRARY_INFO("FAAD" FAAD_FOUND "${FAAD_INCLUDE_DIR}" "${FAAD_LIBRARY_DIR}")

	IF (FAAD_FOUND)
		SET(USE_FAAD 1)

		CHECK_CFLAGS_REQUIRED(faad_check.cpp "-DOLD_FAAD_PROTO" "${FAAD_INCLUDE_DIR}" "${FAAD_LIBRARY_DIR}" FAAD_CFLAGS_REQUIRED)

		IF (FAAD_CFLAGS_REQUIRED)
			SET(OLD_FAAD_PROTO 1)
		ENDIF (FAAD_CFLAGS_REQUIRED)
	ENDIF (FAAD_FOUND)
ELSE (FAAD)
	MESSAGE("${MSG_DISABLE_OPTION}")
ENDIF (FAAD)

MESSAGE("")

########################################
# Vorbis
########################################
OPTION(VORBIS "" ON)

MESSAGE(STATUS "Checking for Vorbis")
MESSAGE(STATUS "*******************")

IF (VORBIS)
	FIND_HEADER_AND_LIB(VORBIS vorbis/vorbisenc.h vorbis vorbis_info_init)
	FIND_HEADER_AND_LIB(VORBISENC "" vorbisenc vorbis_encode_init)

	IF (VORBIS_FOUND AND VORBISENC_FOUND)
		SET(USE_VORBIS 1)
	ELSE (VORBIS_FOUND AND VORBISENC_FOUND)
		SET(VORBIS_FOUND 0 CACHE INTERNAL "")
	ENDIF (VORBIS_FOUND AND VORBISENC_FOUND)

	PRINT_LIBRARY_INFO("Vorbis" VORBIS_FOUND "${VORBIS_INCLUDE_DIR}" "${VORBIS_LIBRARY_DIR} ${VORBISENC_LIBRARY_DIR}")
ELSE (VORBIS)
	MESSAGE("${MSG_DISABLE_OPTION}")
ENDIF (VORBIS)

MESSAGE ("")