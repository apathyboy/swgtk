
include(GroupSourcesByFilesystem)

file(GLOB_RECURSE SOURCES *.h *.cpp *.cc)

GroupSourcesByFilesystem("${SOURCES}")

add_executable(toc_header_reader
    ${SOURCES}
)

target_link_libraries(toc_header_reader
    swgtk
)

set_target_properties(toc_header_reader PROPERTIES FOLDER "examples")

if(WIN32)
    configure_file(${PROJECT_SOURCE_DIR}/tools/windows/user_project.vcxproj.in
        ${CMAKE_CURRENT_BINARY_DIR}/toc_header_reader.vcxproj.user @ONLY
    )
endif()
