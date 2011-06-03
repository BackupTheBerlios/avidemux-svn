add_library(ADM_xvidRateCtl rateControl.cpp  xvidRateCtl.cpp  xvidRateCtlVbv.cpp)

if (UNIX)
	add_target_cflags(ADM_xvidRateCtl -fPIC)
endif (UNIX)