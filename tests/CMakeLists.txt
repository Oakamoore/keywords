FetchContent_Declare(Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG v3.6.0
	GIT_PROGRESS ON
)

FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

add_executable(keywords-tests
	game_tests.cpp
	input_component_tests.cpp
	leaderboard_tests.cpp
	main_menu_tests.cpp
	session_tests.cpp
	word_bank_tests.cpp
	word_tests.cpp
)

target_link_libraries(keywords-tests
	PRIVATE Catch2::Catch2WithMain
	PRIVATE keywords-lib
)

set(test_resource_path "${CMAKE_SOURCE_DIR}/tests/resources")

if(UNIX AND NOT APPLE)
	set(test_destination_path "${CMAKE_CURRENT_BINARY_DIR}/tests/resources")
else()
	set(test_destination_path "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/tests/resources")
endif()

add_custom_command(TARGET keywords-tests PRE_BUILD

	COMMAND ${CMAKE_COMMAND} -E make_directory ${test_destination_path}
	
	# Copy over test resource files
	COMMAND ${CMAKE_COMMAND} -E copy ${test_resource_path}/easy_words_file.txt				${test_destination_path} 
	COMMAND ${CMAKE_COMMAND} -E copy ${test_resource_path}/medium_words_file.txt			${test_destination_path} 
	COMMAND ${CMAKE_COMMAND} -E copy ${test_resource_path}/hard_words_file.txt				${test_destination_path} 
	COMMAND ${CMAKE_COMMAND} -E copy ${test_resource_path}/leaderboard_entries_file.txt		${test_destination_path} 
	COMMAND ${CMAKE_COMMAND} -E copy ${test_resource_path}/session_save_file.txt			${test_destination_path} 
	COMMAND ${CMAKE_COMMAND} -E copy ${test_resource_path}/empty_file.txt					${test_destination_path}
)

# Changes the default working directory so that the above resource
# files are visible when the program is run in Visual Studio
set_property(TARGET keywords-tests PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>")

