#######################################
include (admCheckAudioEncoderLibs)
include (admConfigSummary)

ADD_SUBDIRECTORY(twolame)
ADD_SUBDIRECTORY(pcm)
ADD_SUBDIRECTORY(lavcodec)

if(USE_LAME)
	ADD_SUBDIRECTORY(lame)
endif(USE_LAME)

if(USE_AFTEN)
	ADD_SUBDIRECTORY(aften)
endif(USE_AFTEN)

if(USE_VORBIS)
	ADD_SUBDIRECTORY(vorbis)
endif(USE_VORBIS)

if(USE_FAAC)
	ADD_SUBDIRECTORY(faac)
endif(USE_FAAC)
