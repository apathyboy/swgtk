
file(GLOB_RECURSE SOURCES *.h *.cpp *.cc)

GroupSourcesByFilesystem("${SOURCES}")

add_executable(tre_header_reader
    ${SOURCES}
)
target_link_libraries(tre_header_reader
    swgtk
    ${ZLIB_LIBRARIES}
)
if(WIN32)
    configure_file(${PROJECT_SOURCE_DIR}/build-aux/windows/user_project.vcxproj.in
        ${CMAKE_CURRENT_BINARY_DIR}/tre_header_reader.vcxproj.user @ONLY
    )
endif()
