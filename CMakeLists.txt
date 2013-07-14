cmake_minimum_required(VERSION 2.8.6)

project(swgtk CXX)

# current source version
set(swgtk_VERSION_MAJOR 1)
set(swgtk_VERSION_MINOR 2)
set(swgtk_VERSION_PATCH 0)
set(swgtk_VERSION "${swgtk_VERSION_MAJOR}.${swgtk_VERSION_MINOR}.${swgtk_VERSION_PATCH}")

# set the path to the custom cmake rules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake_targets")

option(BUILD_EXAMPLES
    "Explicitly force building of examples" OFF)

# On gcc platforms the c++11 flag is needed to enable features used by this project.
if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

    add_definitions(-DHAS_STD_REGEX)

    if(CMAKE_GENERATOR STREQUAL Xcode)
        set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
        set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    endif()

    set(GUI_TYPE MACOSX_BUNDLE)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

elseif(WIN32)
    # If using visual studio's compiler make sure we're using at least version 10
    if(MSVC_VERSION LESS 1700)
        message(FATAL_ERROR "MSVC 11 or higher is required to build this library")
    endif()

    set(_WIN32_WINNT 0x0501 CACHE INTERNAL "Setting _WIN32_WINNT to 0x0501 for Windows XP minimum APIs")
    set(WINVER 0x0501 CACHE INTERNAL "Setting WINVER to 0x0501 for Windows XP minimum APIs")

    add_definitions(-D_WIN32_WINNT=${_WIN32_WINNT})
    message(STATUS "- MSVC: Set minimum Windows API version")

    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
    message(STATUS "- MSVC: Disabled NON-SECURE warnings")

    add_definitions(-DHAS_STD_REGEX)

    set(GUI_TYPE WIN32)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(DEFINED VENDOR_PREFIX)
    set(BOOST_ROOT ${VENDOR_PREFIX})
    set(ZLIB_ROOT ${VENDOR_PREFIX})
endif()

add_definitions(-DBOOST_ALL_NO_LIB)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost 1.54.0 COMPONENTS filesystem REQUIRED)
find_package(ZLIB REQUIRED)

add_subdirectory(swgtk)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
