
include(GroupSourcesByFilesystem)

file(GLOB_RECURSE SOURCES *.h *.cpp *.cc)

GroupSourcesByFilesystem("${SOURCES}")

include_directories(
    ${Boost_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIRS}
)

add_library(swgtk
    ${SOURCES}
)

if(WIN32)
    # Create a custom built user configuration so that the "run in debug mode"
    # works without any issues.
    configure_file(${PROJECT_SOURCE_DIR}/tools/windows/user_project.vcxproj.in
    	${CMAKE_CURRENT_BINARY_DIR}/swgtk.vcxproj.user @ONLY)
endif()
