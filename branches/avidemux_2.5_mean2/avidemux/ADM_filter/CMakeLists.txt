SET(ADM_filter_SRCS ADM_vidDummyFilter.cpp  filter.cpp  filter_declaration.cpp  filter_dylib.cpp  filter_saveload.cpp  vidVCD.cpp)

ADD_ADM_LIB_ALL_TARGETS(ADM_filter ${ADM_filter_SRCS})
ADD_CFLAGS_ALL_TARGETS(ADM_filter -I${LIBXML2_INCLUDE_DIR} ${LIBXML2_DEFINITIONS})