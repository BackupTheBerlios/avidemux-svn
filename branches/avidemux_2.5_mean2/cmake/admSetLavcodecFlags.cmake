SET(CONFIG_ENCODERS 1)
SET(CONFIG_DVVIDEO_ENCODER 1)

SET(CONFIG_DECODERS 1)
SET(CONFIG_DVVIDEO_DECODER 1)
SET(CONFIG_H263_DECODER 1)
SET(CONFIG_MPEG4_DECODER 1)
SET(CONFIG_MPEGAUDIO_HP 1)
SET(CONFIG_SNOW_DECODER 1)
SET(CONFIG_VC1_DECODER 1)
SET(CONFIG_WMV2_DECODER 1)
SET(CONFIG_WMV3_DECODER 1)
SET(CONFIG_ZLIB 1)

SET(CONFIG_MUXERS 1)
SET(CONFIG_MOV_MUXER 1)
SET(CONFIG_MP4_MUXER 1)
SET(CONFIG_PSP_MUXER 1)
SET(CONFIG_TG2_MUXER 1)
SET(CONFIG_TGP_MUXER 1)

SET(ENABLE_THREADS 1)
SET(HAVE_LRINTF 1)
SET(HAVE_THREADS 1)
SET(RUNTIME_CPUDETECT 1)

IF (ADM_CPU_X86 OR ADM_CPU_X86_64)
	SET(ENABLE_MMX 1)
	SET(HAVE_MMX 1)
	SET(HAVE_FAST_UNALIGNED 1)

	IF (ADM_CPU_X86_64)
		SET(HAVE_FAST_64BIT 1)
	ENDIF (ADM_CPU_X86_64)
ELSE (ADM_CPU_X86 OR ADM_CPU_X86_64)
	SET(ENABLE_MMX 0)
ENDIF (ADM_CPU_X86 OR ADM_CPU_X86_64)

IF (ADM_OS_DARWIN)
	SET(CONFIG_DARWIN 1)
ENDIF (ADM_OS_DARWIN)

########################################
# Check GCC support for SSSE3
########################################
IF (ADM_CPU_X86 OR ADM_CPU_X86_64)
	MESSAGE(STATUS "Checking GCC support for SSSE3")
	MESSAGE(STATUS "******************************")

	ADM_COMPILE(ssse3_check.cpp "" "" "" SSSE3_SUPPORTED outputSsse3Test)

	IF (SSSE3_SUPPORTED)
		SET(HAVE_SSSE3 1)

		MESSAGE(STATUS "GCC supports SSSE3")
	ELSE (SSSE3_SUPPORTED)
		MESSAGE(STATUS "GCC doesn't support SSSE3")

		IF (VERBOSE)
			MESSAGE("Error Message: ${outputSsse3Test}")
		ENDIF (VERBOSE)	
	ENDIF (SSSE3_SUPPORTED)

	MESSAGE("")
ENDIF (ADM_CPU_X86 OR ADM_CPU_X86_64)