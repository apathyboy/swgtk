file(GLOB_RECURSE SOURCES *.h *.cpp *.cc)

GroupSourcesByFilesystem("${SOURCES}")

add_executable(getheight
    ${SOURCES}
)

include_directories(
	${GLM_INCLUDE_DIR}
)

target_link_libraries(getheight
    swgtk
    ${ZLIB_LIBRARIES}
)

if(WIN32)
    configure_file(${PROJECT_SOURCE_DIR}/tools/windows/user_project.vcxproj.in
        ${CMAKE_CURRENT_BINARY_DIR}/getheight.vcxproj.user @ONLY
    )
endif()