# 
#                     Copyright (C) 2019 Clovis Durand
# 
# -----------------------------------------------------------------------------

# CMake version required ----------------------------------
cmake_minimum_required(VERSION 3.0)
project(Gaussian)

# Project definition --------------------------------------
if(NOT DEFINED PRJ_VERSION)
    set(PRJ_VERSION "2.0.0")
endif(NOT DEFINED PRJ_VERSION)

#------------------------------------------------------------------------------
# Build definition (to do before calling 'project()')
#------------------------------------------------------------------------------

# Build type
if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE Release CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# CASE OF C PROJECT
set(CMAKE_C_STANDARD 99)
if(NOT CMAKE_C_FLAGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wformat-security " CACHE STRING "C99 compilation flags" FORCE)
endif(NOT CMAKE_C_FLAGS)

# CASE OF CPP PROJECT
set(CMAKE_CXX_STANDARD 11)
if(NOT CMAKE_CXX_FLAGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wformat-security" CACHE STRING "C++11 compilation flags" FORCE)
endif(NOT CMAKE_CXX_FLAGS)

#------------------------------------------------------------------------------
# Project definition, variable and dependencies 
#------------------------------------------------------------------------------

set(CMAKE_PROJECT_NAME ${PROJECT_NAME})
set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_PROJECT_BRIEF "Gaussian library")

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/image CACHE PATH "" FORCE)
endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

#set(PKG_CONFIG_PATH ${CMAKE_SOURCE_DIR}/../image/lib/pkgconfig CACHE PATH "")
#set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${PKG_CONFIG_PATH}")
message(STATUS "Pkgconfig extra path are: $ENV{PKG_CONFIG_PATH} ")

# Allow CTest
enable_testing()

# Allow subdirectory test and docs
option(ENABLE_TESTS "Enable Tests" 1)

find_package(Doxygen)
option(ENABLE_DOCS "Build API documentation" ${DOXYGEN_FOUND})

#------------------------------------------------------------------------------
# Project version
#------------------------------------------------------------------------------

# Get the latest abbreviated commit hash of the working branch
execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
  
string(REGEX REPLACE
    "^([\\.0-9]+).*"
    "\\1"
    tmp_VERSION
    "${PRJ_VERSION}"
)
  
string(REGEX REPLACE
    "^([0-9]+)\\..*"
    "\\1"
    tmp_SOVERSION
    "${tmp_VERSION}"
)

# Major version is for product/marketing purpose
# Minor version is incremented everytime it gets public
set(MAJ_MIN_VERSION ${tmp_VERSION} CACHE STRING "Major and minor version" FORCE)

# Release version is incremented everytime it gets use inside the company
set(RELEASE_VERSION ${tmp_SOVERSION} CACHE STRING "Release version" FORCE)

# Build version is generated using the hash of the current commit build with
set(BUILD_VERSION ${GIT_COMMIT_HASH} CACHE STRING "Build version" FORCE)

#------------------------------------------------------------------------------
# pkgconfig
#------------------------------------------------------------------------------

find_package(PkgConfig REQUIRED)
if(PKG_CONFIG_FOUND)
    # Produce a pkg-config file for Client project
    configure_file (
        ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}.pc.in
        ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}.pc
        @ONLY
    )
    install (
        FILES ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}.pc    
        DESTINATION lib/pkgconfig
    )  
endif (PKG_CONFIG_FOUND)

#------------------------------------------------------------------------------
# Project configuration 
#------------------------------------------------------------------------------

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/inc)

#------------------------------------------------------------------------------
# Sub-directories 
#------------------------------------------------------------------------------

# Main build
add_subdirectory(src)

# Builds dependent on main build
if(ENABLE_TESTS)
    message(STATUS "TESTS enabled")
    add_subdirectory(tests)
else()
    message(STATUS "TESTS disabled")
endif(ENABLE_TESTS)

if(ENABLE_DOCS)
    message(STATUS "DOCS enabled")
    add_subdirectory(docs)
else()
    message(STATUS "DOCS disabled")
endif (ENABLE_DOCS)

#------------------------------------------------------------------------------
# Gencov custom command
#------------------------------------------------------------------------------

set(COVERITY_FILES
  " ${CMAKE_SOURCE_DIR}/build-coverity/output/summary.txt"
)

add_custom_command(OUTPUT ${COVERITY_FILES}
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} mkdir -p build-coverity
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} cov-configure --gcc
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} cov-build --dir build-coverity make -j $(nproc)
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} cov-analyze --dir build-coverity --all --aggressiveness-level low --misra-config MISRA_c2012_MM.config
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} cov-commit-defects --dir build-coverity --host coverity3.freescale.net --user committer --password CoverityGuest --stream lib${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E touch ${COVERITY_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "run coverity"
)
add_custom_target(coverity DEPENDS ${COVERITY_FILES})
