
file(GLOB_RECURSE SOURCES *.h *.cpp *.cc)

GroupSourcesByFilesystem("${SOURCES}")

add_executable(toc_reader
    ${SOURCES}
)
target_link_libraries(toc_reader
    swgtk
    ${ZLIB_LIBRARIES}
)
if(WIN32)
    configure_file(${PROJECT_SOURCE_DIR}/build-aux/windows/user_project.vcxproj.in
        ${CMAKE_CURRENT_BINARY_DIR}/header_reader.vcxproj.user @ONLY
    )
endif()