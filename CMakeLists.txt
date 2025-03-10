cmake_minimum_required(VERSION 3.29)

option(ENABLE_TESTING "Enable tests" OFF)

# Setup dependencies
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
	GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
	GIT_TAG v5.0.0
	GIT_PROGRESS ON
)
FetchContent_MakeAvailable(ftxui)

if(EMSCRIPTEN)
	set(ENABLE_TESTING CACHE INTERNAL "")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_PTHREADS -O2")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ASYNCIFY=1 -s PROXY_TO_PTHREAD")
endif()

project(keywords
	VERSION 1.0
	DESCRIPTION "A terminal based typing game"
	HOMEPAGE_URL "https://github.com/Oakamoore/keywords"
	LANGUAGES CXX
)

find_package(Threads)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(keywords-lib STATIC
	src/audio.cpp
	src/game.cpp
	src/input_component.cpp
	src/leaderboard.cpp
	src/main_menu.cpp
	src/session.cpp
	src/transition.cpp
	src/util.cpp
	src/word.cpp
	src/word_bank.cpp
)

target_include_directories(keywords-lib
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

target_link_libraries(keywords-lib
	PRIVATE Threads::Threads
	PUBLIC ftxui::component
	PUBLIC ftxui::dom
	PUBLIC ftxui::screen
)

if(ENABLE_TESTING)
	message("Building tests.")
	enable_testing()
	add_subdirectory(tests)
endif()

add_executable(keywords
	src/main.cpp
)

target_link_libraries(keywords
	PRIVATE keywords-lib
)

if(EMSCRIPTEN)
	target_link_options(keywords PUBLIC "SHELL: -s TOTAL_MEMORY=32MB")
	target_compile_options(keywords PUBLIC "-gsource-map")
	target_compile_options(keywords PUBLIC "--source-map-base=\"https://localhost:8888/\"")

	# Copy over resource files
	foreach(file
		"keywords.html"
		"resources/word_bank/easy_words.txt"
		"resources/word_bank/medium_words.txt"
		"resources/word_bank/hard_words.txt"
		"resources/audio/leaderboard_track.wav"
		"resources/audio/main_menu_track.wav"
		"resources/audio/session_fast_track.wav"
		"resources/audio/session_medium_track.wav"
		"resources/audio/session_slow_track.wav"
	)
	configure_file(${file} ${file} COPYONLY)
	endforeach(file)
	
	# Create empty save files
	make_directory("${CMAKE_CURRENT_BINARY_DIR}/resources/save_files")
	file(TOUCH "${CMAKE_CURRENT_BINARY_DIR}/resources/save_files/easy_session_save_file.txt")
	file(TOUCH "${CMAKE_CURRENT_BINARY_DIR}/resources/save_files/medium_session_save_file.txt")
	file(TOUCH "${CMAKE_CURRENT_BINARY_DIR}/resources/save_files/hard_session_save_file.txt")

	# Make the resources files available to the browser.
	set_property(TARGET keywords APPEND_STRING PROPERTY LINK_FLAGS
    "--preload-file ${CMAKE_CURRENT_BINARY_DIR}/resources/@/resources/")

else()
	set(resource_path "${CMAKE_SOURCE_DIR}/resources")

	# Account for different generators
	if(UNIX AND NOT APPLE)
		set(destination_path "${CMAKE_CURRENT_BINARY_DIR}/resources")
	else()
		set(destination_path "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/resources")
	endif()

	add_custom_command(TARGET keywords PRE_BUILD
	
		COMMAND ${CMAKE_COMMAND} -E make_directory ${destination_path}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${destination_path}/word_bank
		COMMAND ${CMAKE_COMMAND} -E make_directory ${destination_path}/save_files
		COMMAND ${CMAKE_COMMAND} -E make_directory ${destination_path}/audio

		# Copy over the word banks 
		COMMAND ${CMAKE_COMMAND} -E copy ${resource_path}/word_bank/easy_words.txt		${destination_path}/word_bank
		COMMAND ${CMAKE_COMMAND} -E copy ${resource_path}/word_bank/medium_words.txt	${destination_path}/word_bank
		COMMAND ${CMAKE_COMMAND} -E copy ${resource_path}/word_bank/hard_words.txt		${destination_path}/word_bank

		# Create new save files
		COMMAND ${CMAKE_COMMAND} -E touch ${destination_path}/save_files/easy_session_save_file.txt
		COMMAND ${CMAKE_COMMAND} -E touch ${destination_path}/save_files/medium_session_save_file.txt
		COMMAND ${CMAKE_COMMAND} -E touch ${destination_path}/save_files/hard_session_save_file.txt

		# Copy over audio files
		COMMAND ${CMAKE_COMMAND} -E copy ${resource_path}/audio/leaderboard_track.wav		${destination_path}/audio
		COMMAND ${CMAKE_COMMAND} -E copy ${resource_path}/audio/main_menu_track.wav			${destination_path}/audio
		COMMAND ${CMAKE_COMMAND} -E copy ${resource_path}/audio/session_fast_track.wav		${destination_path}/audio
		COMMAND ${CMAKE_COMMAND} -E copy ${resource_path}/audio/session_medium_track.wav	${destination_path}/audio
		COMMAND ${CMAKE_COMMAND} -E copy ${resource_path}/audio/session_slow_track.wav		${destination_path}/audio
	)

	# Changes the default working directory so that the above resource
	# files are visible when the program is run in Visual Studio
	set_property(TARGET keywords PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>")
endif()