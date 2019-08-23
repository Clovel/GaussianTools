# 
#                     Copyright (C) 2019 Clovis Durand
# 
# -----------------------------------------------------------------------------

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

set(UTILITY_SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/ConfigParser.cpp
)

# Target definition ---------------------------------------
add_library(${CMAKE_PROJECT_NAME} SHARED
    ${SOURCES}
)

add_executable(${CMAKE_PROJECT_NAME}Utility
    ${SOURCES}
    ${UTILITY_SOURCES}
)

#----------------------------------------------------------------------------
# The installation is prepended by the CMAKE_INSTALL_PREFIX variable
install(TARGETS ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)
