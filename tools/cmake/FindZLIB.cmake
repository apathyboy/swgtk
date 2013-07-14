
find_path(ZLIB_ROOT
    NAMES include/zlib.h
)

find_path(ZLIB_INCLUDE_DIR zlib.h
    HINTS
        $ENV{ZLIB_ROOT}
    PATH_SUFFIXES include
    PATHS
        ${ZLIB_ROOT}
        ${ZLIB_INCLUDEDIR}
)

find_library(ZLIB_LIBRARY
    NAMES z zlib
    PATH_SUFFIXES lib Release lib/Release
    HINTS
        $ENV{ZLIB_ROOT}
        ${ZLIB_ROOT}
        ${ZLIB_LIBRARYDIR}
)

# handle the QUIETLY and REQUIRED arguments and set OPENAL_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ZLIB DEFAULT_MSG ZLIB_LIBRARY ZLIB_INCLUDE_DIR)

mark_as_advanced(ZLIB_ROOT ZLIB_INCLUDE_DIR ZLIB_LIBRARY)
