
add_executable(file_info
    file_info.cc
)
target_link_libraries(file_info
    tre
    ${ZLIB_LIBRARIES}
)
if(WIN32)
    configure_file(${CMAKE_SOURCE_DIR}/build-aux/windows/user_project.vcxproj.in
        ${CMAKE_CURRENT_BINARY_DIR}/file_info.vcxproj.user @ONLY
    )
endif()