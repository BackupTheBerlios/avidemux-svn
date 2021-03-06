#include "config.h"

#ifdef USE_LIBDCA
#include <stdio.h>
#include "default.h"
#include "libwrapper_dca.h"

ADM_LibWrapperDca::ADM_LibWrapperDca() : ADM_LibWrapper()
{
#ifdef USE_LATE_BINDING
#ifdef __WIN32
#ifdef USE_LIBDCA_002
	const char* libname = "libdts.dll";
#else
	const char* libname = "libdca-0.dll";
#endif	// USE_LIBDCA_002
#else
#ifdef USE_LIBDCA_002
	const char* libname = "libdts.so";	// ???
#else
	const char* libname = "libdca-0.so";	// ???
#endif	// USE_LIBDCA_002
#endif	// __WIN32

#ifdef USE_LIBDCA_002
	initialised = (loadLibrary(libname) && getSymbols(7, 
		&func_dts_block, "dts_block", 
		&func_dts_blocks_num, "dts_blocks_num", 
		&func_dts_frame, "dts_frame", 
		&func_dts_free, "dts_free", 
		&func_dts_init, "dts_init",
		&func_dts_samples, "dts_samples", 
		&func_dts_syncinfo, "dts_syncinfo"));
#else
	initialised = (loadLibrary(libname) && getSymbols(7, 
		&func_dts_block, "dca_block", 
		&func_dts_blocks_num, "dca_blocks_num", 
		&func_dts_frame, "dca_frame", 
		&func_dts_free, "dca_free", 
		&func_dts_init, "dca_init",
		&func_dts_samples, "dca_samples", 
		&func_dts_syncinfo, "dca_syncinfo"));
#endif	// USE_LIBDCA_002
#else
	initialised = true;
#endif	// USE_LATE_BINDING
}

ADM_LibWrapperDca::~ADM_LibWrapperDca() {}

int ADM_LibWrapperDca::dts_block_ (dts_state_t* state)
{
#ifdef USE_LATE_BINDING
	return func_dts_block(state);
#else
	return dts_block(state);
#endif
}

int ADM_LibWrapperDca::dts_blocks_num_ (dts_state_t* state)
{
#ifdef USE_LATE_BINDING
	return func_dts_blocks_num(state);
#else
	return dts_blocks_num(state);
#endif
}

int ADM_LibWrapperDca::dts_frame_ (dts_state_t* state, uint8_t* buf, int* flags, level_t* level, sample_t bias)
{
#ifdef USE_LATE_BINDING
	return func_dts_frame(state, buf, flags, level, bias);
#else
	return dts_frame(state, buf, flags, level, bias);
#endif
}

void ADM_LibWrapperDca::dts_free_ (dts_state_t* state)
{
#ifdef USE_LATE_BINDING
	return func_dts_free(state);
#else
	return dts_free(state);
#endif
}

dts_state_t* ADM_LibWrapperDca::dts_init_ (uint32_t mm_accel)
{
#ifdef USE_LATE_BINDING
	return func_dts_init(mm_accel);
#else
	return dts_init(mm_accel);
#endif
}

sample_t* ADM_LibWrapperDca::dts_samples_ (dts_state_t* state)
{
#ifdef USE_LATE_BINDING
	return func_dts_samples(state);
#else
	return dts_samples(state);
#endif
}

int ADM_LibWrapperDca::dts_syncinfo_ (dts_state_t* state, uint8_t* buf, int* flags, int* sample_rate, int* bit_rate, int* frame_length)
{
#ifdef USE_LATE_BINDING
	return func_dts_syncinfo(state, buf, flags, sample_rate, bit_rate, frame_length);
#else
	return dts_syncinfo(state, buf, flags, sample_rate, bit_rate, frame_length);
#endif
}

#endif
