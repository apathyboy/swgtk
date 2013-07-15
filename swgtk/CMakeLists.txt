
include(GroupSourcesByFilesystem)

file(GLOB_RECURSE SOURCES *.h *.cpp *.cc)
file(GLOB_RECURSE TEST_SOURCES *_unittest.h *_unittest.cpp *_unittest.cc)

GroupSourcesByFilesystem("${SOURCES}")

list(REMOVE_ITEM SOURCES ${TEST_SOURCES})

include_directories(
    ${Boost_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIRS}
)

add_library(swgtk
    ${SOURCES}
)

target_link_libraries(swgtk
    ${ZLIB_LIBRARY}
)

add_executable(swgtk_tests
	${TEST_SOURCES}
)

target_link_libraries(swgtk_tests
	swgtk
)

if(WIN32)
    # Create a custom built user configuration so that the "run in debug mode"
    # works without any issues.
    configure_file(${PROJECT_SOURCE_DIR}/tools/windows/user_project.vcxproj.in
    	${CMAKE_CURRENT_BINARY_DIR}/swgtk_tests.vcxproj.user @ONLY)
endif()
