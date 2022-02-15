# project definitions

cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME "cslice lib")
set(BINARY_NAME "cslice")
project(${PROJECT_NAME} C)

set(CMAKE_C_STANDARD 99)
set(LIBRARY_VERSION_MAJOR 0)
set(LIBRARY_VERSION_STRING 0.1)

include(GNUInstallDirs)

set(LIBRARY_BASE_PATH "${PROJECT_SOURCE_DIR}/lib")

# project configuration

include_directories (
        ${LIBRARY_BASE_PATH}
)

set(SOURCE_FILES ${LIBRARY_BASE_PATH}/src/cslice/cslice.c)
set(HEADER_FILES ${LIBRARY_BASE_PATH}/include/cslice/cslice.h)

add_library(${BINARY_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})

# project installation details
set_target_properties(
        ${BINARY_NAME} PROPERTIES
        VERSION		${LIBRARY_VERSION_STRING}
        SOVERSION	${LIBRARY_VERSION_MAJOR}
        PUBLIC_HEADER ${HEADER_FILES}
)

add_definitions("-g")

install(
        TARGETS ${BINARY_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)