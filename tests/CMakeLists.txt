# 
#                     Copyright (C) 2019 Clovis Durand
# 
# -----------------------------------------------------------------------------

# Definitions ---------------------------------------------
add_definitions(-DTEST)

# Requirements --------------------------------------------

# Header files --------------------------------------------
file(GLOB_RECURSE PUBLIC_HEADERS 
    ${CMAKE_SOURCE_DIR}/inc/*.h
    ${CMAKE_SOURCE_DIR}/inc/*.hpp
)
set(HEADERS
    ${PUBLIC_HEADERS}
)

include_directories(
    ${CMAKE_SOURCE_DIR}/inc
)

# Source files --------------------------------------------
set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/Gaussian.cpp
)

file(GLOB_RECURSE TEST_SOURCES
    ${CMAKE_SOURCE_DIR}/tests/main.cpp
)

# Target definition ---------------------------------------
add_executable(${CMAKE_PROJECT_NAME}-tests
    ${SOURCES}
    ${TEST_SOURCES}
)

# Test definition -----------------------------------------
#add_test( testname Exename arg1 arg2 ... )
add_test( gaussian_test_default ${CMAKE_PROJECT_NAME}-tests -1 )
