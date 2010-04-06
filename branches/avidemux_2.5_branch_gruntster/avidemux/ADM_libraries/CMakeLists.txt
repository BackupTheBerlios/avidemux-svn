add_subdirectory(ADM_lvemux)
add_subdirectory(ADM_mplex)

if (NOT USE_SYSTEM_SPIDERMONKEY)
	add_subdirectory(ADM_smjs)
endif (NOT USE_SYSTEM_SPIDERMONKEY)

add_subdirectory(ADM_utilities)

if (ADM_DEBUG AND FIND_LEAKS)
	add_subdirectory(ADM_nvwa)
endif (ADM_DEBUG AND FIND_LEAKS)

if (APPLE AND USE_SDL)
	add_subdirectory(ADM_libsdl)
endif (APPLE AND USE_SDL)
