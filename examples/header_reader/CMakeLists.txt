
add_executable(header_reader
    header_reader.cc
)
target_link_libraries(header_reader
    tre
    ${ZLIB_LIBRARIES}
)
if(WIN32)
    configure_file(${CMAKE_SOURCE_DIR}/build-aux/windows/user_project.vcxproj.in
        ${CMAKE_CURRENT_BINARY_DIR}/header_reader.vcxproj.user @ONLY
    )
endif()