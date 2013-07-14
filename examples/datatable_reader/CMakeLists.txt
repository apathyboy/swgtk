
include(GroupSourcesByFilesystem)

file(GLOB_RECURSE SOURCES *.h *.cpp *.cc)

GroupSourcesByFilesystem("${SOURCES}")

include_directories(
    ${Boost_INCLUDE_DIR}
)

add_executable(datatable_reader
    ${SOURCES}
)

target_link_libraries(datatable_reader
    swgtk
)

set_target_properties(datatable_reader PROPERTIES FOLDER "examples")

if(WIN32)
    configure_file(${PROJECT_SOURCE_DIR}/tools/windows/user_project.vcxproj.in
        ${CMAKE_CURRENT_BINARY_DIR}/datatable_reader.vcxproj.user @ONLY
    )
endif()
