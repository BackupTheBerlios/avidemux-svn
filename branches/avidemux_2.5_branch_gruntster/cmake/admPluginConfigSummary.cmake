# ARGV2 - capable flag
MACRO(ADM_DISPLAY _name)
	SET(PROCEED 1)

	IF (NOT ${ARGV2})
		SET(PROCEED 0)
	ENDIF (NOT ${ARGV2})

	IF (${PROCEED})
		IF (${ARGV1})
			SET(s "Yes")
		ELSE (${ARGV1})
			SET(s "No")
		ENDIF (${ARGV1})

		MESSAGE("   ${_name}  ${s}")
	ENDIF (${PROCEED})
ENDMACRO(ADM_DISPLAY)

MESSAGE("*************************")
MESSAGE("***      SUMMARY      ***")
MESSAGE("*************************")
MESSAGE("***    Video Codec    ***")
ADM_DISPLAY("x264          " "$ENV{ADM_HAVE_X264}")
ADM_DISPLAY("Xvid          " "$ENV{ADM_HAVE_XVID}")

MESSAGE("***    Audio Codec    ***")
ADM_DISPLAY("Aften         " "$ENV{ADM_HAVE_AFTEN}")
ADM_DISPLAY("FAAC          " "$ENV{ADM_HAVE_FAAC}")
ADM_DISPLAY("FAAD          " "$ENV{ADM_HAVE_FAAD}")
ADM_DISPLAY("LAME          " "$ENV{ADM_HAVE_LAME}")
ADM_DISPLAY("opencore-amrnb" "$ENV{ADM_HAVE_OPENCORE_AMRNB}")
ADM_DISPLAY("opencore-amrwb" "$ENV{ADM_HAVE_OPENCORE_AMRWB}")
ADM_DISPLAY("Vorbis Decoder" "$ENV{ADM_HAVE_VORBIS_DEC}")
ADM_DISPLAY("Vorbis Encoder" "$ENV{ADM_HAVE_VORBIS_ENC}")

IF (UNIX)
	MESSAGE("***   Audio Devices   ***")
	
IF (NOT APPLE)
	ADM_DISPLAY("ALSA          " "$ENV{ADM_HAVE_ALSA}")
ENDIF (NOT APPLE)

	ADM_DISPLAY("aRts          " "$ENV{ADM_HAVE_ARTS}")
	ADM_DISPLAY("ESD           " "$ENV{ADM_HAVE_ESD}")
	ADM_DISPLAY("JACK          " "$ENV{ADM_HAVE_JACK}")
	
IF (NOT APPLE)
	ADM_DISPLAY("OSS           " "$ENV{ADM_HAVE_OSS}")
	ADM_DISPLAY("PulseAudio    " "$ENV{ADM_HAVE_PULSEAUDIO}")
ENDIF (NOT APPLE)
ENDIF (UNIX)

MESSAGE("***   Miscellaneous   ***")
ADM_DISPLAY("FontConfig    " "$ENV{ADM_HAVE_FONTCONFIG}")
ADM_DISPLAY("FreeType2     " "${FREETYPE2_FOUND}")
ADM_DISPLAY("gettext       " "${HAVE_GETTEXT}")

MESSAGE("*************************")

IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
	MESSAGE("***    Debug Build    ***")
ELSE (CMAKE_BUILD_TYPE STREQUAL "Debug")
	MESSAGE("***   Release Build   ***")
ENDIF(CMAKE_BUILD_TYPE STREQUAL "Debug")

MESSAGE("*************************")
