
SET(ADM_audiodevice_SRCS 
ADM_audiodevice.cpp  ADM_deviceArts.cpp       ADM_deviceEsd.cpp  ADM_deviceSDL.cpp    oss_out.cpp
ADM_deviceALSA.cpp   ADM_deviceAudioCore.cpp  ADM_deviceoss.cpp  ADM_deviceWin32.cpp  ADM_deviceJack.cpp
ADM_devicePulseSimple.cpp
)
ADD_LIBRARY(ADM_audiodevice STATIC ${ADM_audiodevice_SRCS})
SDLify(ADM_deviceSDL.cpp)
if(USE_ARTS)
  add_definitions(${ARTS_CFLAGS})
endif(USE_ARTS)
if(USE_PULSE_SIMPLE)
  add_definitions(${PULSEAUDIOSIMPLE_DEFINITIONS})
  include_directories(${PULSEAUDIOSIMPLE_INCLUDE_DIR})
endif(USE_ARTS)
