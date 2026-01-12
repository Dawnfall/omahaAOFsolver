cmake_minimum_required(VERSION 4.1.1)
project(Solver LANGUAGES CXX)

# ------------------------------
# C++ standard
# ------------------------------

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------------
# Enforce 64-bit build
# ------------------------------

# Require 64-bit build
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "This project only supports x64 architecture.")
endif()

# ------------------------------
# Default build type
# ------------------------------

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type" FORCE)
endif()

# ------------------------------
# Check vcpkg environment
# ------------------------------

if(NOT DEFINED ENV{VCPKG_R})
    message(FATAL_ERROR "Please set VCPKG_R to your vcpkg installation")
endif()

# ------------------------------
# Output directories (organized by build type and platform)
# ------------------------------

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>-Windows-x64)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>-Windows-x64)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>-Windows-x64)

# ------------------------------
# External dependencies (vcpkg)
# ------------------------------

find_package(FLTK CONFIG REQUIRED)

# ------------------------------
# Third-party subprojects
# ------------------------------

add_subdirectory(SolverLib)
add_subdirectory(SolverApp)

# ------------------------------
# IDE and folder settings
# ------------------------------

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SolverApp)

# ------------------------------
# Define allowed configurations (optional, helps IDEs)
# ------------------------------

set(CMAKE_CONFIGURATION_TYPES Debug RelWithDebInfo Release CACHE STRING "" FORCE)
