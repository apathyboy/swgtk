cmake_minimum_required(VERSION 2.8.6)

project(swgtoolkit CXX)

# set the path for custom cmake scripts
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build-aux/cmake")

# include common scripts
include(GroupSourcesByFilesystem)

if(DEFINED VENDOR_PREFIX)
    set(BOOST_ROOT ${VENDOR_PREFIX}/boost)
    set(ZLIB_ROOT ${VENDOR_PREFIX}/zlib)
endif()

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost 1.48.0 COMPONENTS chrono date_time regex thread system program_options filesystem REQUIRED)
find_package(ZLIB REQUIRED)

if(MSVC)
	get_filename_component(ZLIB_DEBUG_DLL_PATH ${ZLIB_LIBRARY_DEBUG} PATH)
	get_filename_component(ZLIB_RELEASE_DLL_PATH ${ZLIB_LIBRARY_RELEASE} PATH)
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${Boost_INCLUDE_DIR}
)

link_directories(
    ${Boost_LIBRARY_DIRS}
)

add_subdirectory(swgtk)
add_subdirectory(examples)