
file(GLOB_RECURSE SOURCES *.h *.cpp *.cc)

GroupSourcesByFilesystem("${SOURCES}")

include_directories(
    ${ZLIB_INCLUDE_DIRS}
)

add_library(swgtk
    ${SOURCES}
)
