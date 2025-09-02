cmake_minimum_required(VERSION 3.28)

project(cpp_core VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(VIPS REQUIRED vips)

# ---------------------------
# Sources
# ---------------------------
file(GLOB TRANSFORM_SOURCES src/transforms/*.cpp)

set(CPP_CORE_SOURCES
    src/core.cpp
    src/errors.cpp
    src/image_io.cpp
    src/image_processor.cpp
    src/vimage.cpp
    ${TRANSFORM_SOURCES}
)

# ---------------------------
# Library
# ---------------------------
add_library(cpp_core_lib ${CPP_CORE_SOURCES})
add_library(cpp_core::cpp_core_lib ALIAS cpp_core_lib)

target_include_directories(cpp_core_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${VIPS_INCLUDE_DIRS}   # include của libvips
)

target_link_libraries(cpp_core_lib
    PUBLIC
        ${VIPS_LIBRARIES}      # link libvips
)

if(MSVC)
    target_compile_options(cpp_core_lib PRIVATE /W4 /permissive-)
else()
    target_compile_options(cpp_core_lib PRIVATE -Wall -Wextra -Wpedantic -Wconversion)
endif()

# ---------------------------
# Executable
# ---------------------------
add_executable(cpp_core src/main.cpp)
target_link_libraries(cpp_core PRIVATE cpp_core_lib)

# ---------------------------
# Tests
# ---------------------------
enable_testing()
add_executable(test_image_processor tests/image_processor_test.cpp)
target_link_libraries(test_image_processor PRIVATE cpp_core_lib)
add_test(NAME ImageProcessorTest COMMAND test_image_processor)

# ---------------------------
# Install rules
# ---------------------------
include(GNUInstallDirs)

install(TARGETS cpp_core_lib
    EXPORT cpp_coreTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT cpp_coreTargets
    FILE cpp_coreTargets.cmake
    NAMESPACE cpp_core::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp_core
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/cpp_coreConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cpp_coreConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cpp_coreConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp_core
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/cpp_coreConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cpp_coreConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp_core
)
