cmake_minimum_required(VERSION 4.1.1)

# ------------------------------
# Collect source files
# ------------------------------
file(GLOB_RECURSE SOLVER_APP_SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.h"
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOLVER_APP_SOURCES})


# ------------------------------
# Create executable
# ------------------------------

add_executable(SolverApp ${SOLVER_APP_SOURCES})

set_target_properties(SolverApp PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    #OUTPUT_NAME "SolverApp$<$<CONFIG:Debug>:_d>"
)

# ------------------------------
# Include directories
# ------------------------------

target_include_directories(SolverApp PRIVATE
    "${CMAKE_SOURCE_DIR}/SolverApp/src"
    "${CMAKE_SOURCE_DIR}/SolverLib/include"
    "${CMAKE_SOURCE_DIR}/3rd/PHeval/include"
)

# Third-party includes (suppress warnings)
target_include_directories(SolverApp SYSTEM PRIVATE
    "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include"
)

# ------------------------------
# Link libraries
# ------------------------------

target_link_libraries(SolverApp PRIVATE
    SolverLib
    fltk
)


# ------------------------------
# Compile definitions
# ------------------------------

#add_compile_definitions(PROJECT_ROOT_PATH="${CMAKE_SOURCE_DIR}")

target_compile_definitions(SolverApp PRIVATE
)

# ------------------------------
# Compiler options
# ------------------------------

if(MSVC)
    target_compile_options(SolverApp PRIVATE
        /W4                     # Warning level 4
        #/WX                     # Treat warnings as errors (your code only)
        /external:W0            # Suppress warnings from external headers
        /experimental:external  # Required for /external:W0
        /analyze-               # Disable full static analysis
        /wd4251
        /wd26495

        $<$<CONFIG:Debug>:/Od /Zi>
        $<$<CONFIG:RelWithDebInfo>:/O2 /Zi>
        $<$<CONFIG:Release>:/Ox>
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(SolverApp PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        #-Werror

        $<$<CONFIG:Debug>:-O0 -g>
        $<$<CONFIG:RelWithDebInfo>:-O2 -g>
        $<$<CONFIG:Release>:-O3>
    )
endif()



















