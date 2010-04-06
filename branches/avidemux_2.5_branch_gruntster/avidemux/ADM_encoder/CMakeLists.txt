SET(ADM_encoder_SRCS 
	adm_encConfig.cpp  adm_encoder.cpp
	adm_encCopy.cpp    adm_encRequant.cpp  adm_encyv12.cpp
	ADM_pluginLoad.cpp ADM_externalEncoder.cpp)

ADD_ADM_LIB_ALL_TARGETS(ADM_encoder ${ADM_encoder_SRCS})
