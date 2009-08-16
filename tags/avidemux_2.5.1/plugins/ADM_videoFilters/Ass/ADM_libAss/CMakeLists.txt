# Does not seem to work without fontconfig....

INCLUDE(admCheckFontConfig)
INCLUDE(admCheckFreeType)
checkFontConfig()
checkFreeType()
IF(USE_FREETYPE)

        SET(ADM_LIB ADM_libass)

        SET(${ADM_LIB}_SRCS 
        ass_bitmap.c  ass.c  ass_cache.c  ass_drawing.c  ass_font.c    ass_library.c  ass_render.c  ass_utils.c
        )
    IF (FONTCONFIG_FOUND)
        ADD_DEFINITIONS(${FONTCONFIG_CFLAGS} "-DHAVE_FONTCONFIG=1" "-DCONFIG_FONTCONFIG=1")
        SET(${ADM_LIB}_SRCS  ${${ADM_LIB}_SRCS} ass_fontconfig.c)
    ENDIF (FONTCONFIG_FOUND)
        ADD_LIBRARY(${ADM_LIB} STATIC ${${ADM_LIB}_SRCS})
        ADD_DEFINITIONS(${FREETYPE2_CFLAGS} "-I${LIBICONV_INCLUDE_DIR}")

     


        ADD_DEFINITIONS("-DCONFIG_LIBPNG=1")
        ADD_DEFINITIONS("-DCONFIG_FREETYPE=1")

        IF (UNIX)
	        ADD_TARGET_CFLAGS(${ADM_LIB} -fPIC)
        ENDIF (UNIX)

ENDIF(USE_FREETYPE)

