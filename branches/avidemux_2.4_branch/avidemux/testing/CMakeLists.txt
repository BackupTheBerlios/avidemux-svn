macro(admTest category url_dir url_filename video_codec audio_codec output_format md5sum)
	if (${output_format} STREQUAL "AVI_UNP")
		set(params "--force-unpack")
	else (${output_format} STREQUAL "AVI_UNP")
		set(params)
	endif (${output_format} STREQUAL "AVI_UNP")

	add_test(
		${category}
		${CMAKE_CURRENT_BINARY_DIR}/tester
		samples/${category}
		${url_filename}
		"${url_dir}/${url_filename}"
		"--nogui --autoindex ${params} --load samples/${category}/${url_filename} --video-codec ${video_codec} --audio-codec ${audio_codec} --output-format ${output_format} --save ${OUTPUT_PATH}/${OUTPUT_FILE} --quit"
		${md5sum}
	)
endmacro(admTest)

add_executable(tester tester.cpp)

if (WIN32)
	target_link_libraries(tester "-Wl,-subsystem,console")
endif (WIN32)

find_program(WGET_PATH wget)
find_program(MD5SUM_PATH md5sum)
find_program(AVIDEMUX_PATH avidemux2_cli PATHS "${CMAKE_BINARY_DIR}/avidemux")
set(OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/_output")
set(OUTPUT_FILE "output")

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/tester.cfg" "${WGET_PATH}\n")
file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/tester.cfg" "${MD5SUM_PATH}\n")
file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/tester.cfg" "${AVIDEMUX_PATH}\n")
file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/tester.cfg" "${OUTPUT_PATH}\n")
file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/tester.cfg" "${OUTPUT_FILE}\n")

add_test(basic_run ${AVIDEMUX_PATH})
admTest(asf http://samples.mplayerhq.hu/asf-wmv elephant.asf XVID4 MP3 AVI 6d65a64f0ad4f4fedc139ac37a9127a5)
admTest(ts http://samples.mplayerhq.hu/MPEG2 foxksaz.ts Copy AC3 AVI 0)
admTest(avi http://samples.mplayerhq.hu/avi verona60avi56k.avi XVID4 MP3 AVI 0)
admTest(avi_unpack http://samples.mplayerhq.hu/avi GoneNutty.avi Copy Copy AVI_UNP d472495e2e8a19535ba9fb09b444c859)