
file(GLOB_RECURSE SOURCES *.h *.cpp *.cc)

GroupSourcesByFilesystem("${SOURCES}")

add_executable(file_info
    ${SOURCES}
)

target_link_libraries(file_info
    swgtk
    ${ZLIB_LIBRARIES}
)

if(WIN32)
    configure_file(${CMAKE_SOURCE_DIR}/build-aux/windows/user_project.vcxproj.in
        ${CMAKE_CURRENT_BINARY_DIR}/file_info.vcxproj.user @ONLY
    )
endif()