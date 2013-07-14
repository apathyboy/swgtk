
include(GroupSourcesByFilesystem)

file(GLOB_RECURSE SOURCES *.h *.cpp *.cc)

GroupSourcesByFilesystem("${SOURCES}")

add_executable(tre_archiver
    ${SOURCES}
)

target_link_libraries(tre_archiver
    swgtk
)

set_target_properties(tre_archiver PROPERTIES FOLDER "examples")

if(WIN32)
    configure_file(${PROJECT_SOURCE_DIR}/tools/windows/user_project.vcxproj.in
        ${CMAKE_CURRENT_BINARY_DIR}/tre_archiver.vcxproj.user @ONLY
    )
endif()
