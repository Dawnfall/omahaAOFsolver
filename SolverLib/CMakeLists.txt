cmake_minimum_required(VERSION 4.1.1)

# ------------------------------
# Collect source files
# ------------------------------

file(GLOB_RECURSE SOLVER_LIB_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE SOLVER_LIB_HEADERS CONFIGURE_DEPENDS "include/*.h")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOLVER_LIB_SOURCES} ${SOLVER_LIB_HEADERS})

# ------------------------------
# Create shared library
# ------------------------------

add_library(SolverLib STATIC ${SOLVER_LIB_SOURCES} ${SOLVER_LIB_HEADERS})
set_target_properties(SolverLib PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO

    #OUTPUT_NAME "Solver$<$<CONFIG:Debug>:_d>"
)

# ------------------------------
# Include directories
# ------------------------------

target_include_directories(SolverLib PUBLIC
    "${CMAKE_SOURCE_DIR}/SolverLib/include"
    "${CMAKE_SOURCE_DIR}/3rd/PHeval/include"
    #"${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include"
)

# third party with suppressed warnings
#target_include_directories(TwistedEngine SYSTEM PUBLIC
#    
#    "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include"
#)

# ------------------------------
# Libraries
# ------------------------------

target_link_libraries(SolverLib PRIVATE
    $<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/3rd/PHeval/libs/Debug/PHevalplo4.lib>
    $<$<NOT:$<CONFIG:Debug>>:${CMAKE_SOURCE_DIR}/3rd/PHeval/libs/Release/PHevalplo4.lib>
)
# ------------------------------
# Compile definitions
# ------------------------------

target_compile_definitions(SolverLib PRIVATE
    SOLVER
    $<$<CONFIG:Debug>:SOLVER_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:SOLVER_RELEASE>
    $<$<CONFIG:Release>:SOLVER_DEPLOY>
)

# ------------------------------
# Compiler options
# ------------------------------

if(MSVC)
    target_compile_options(SolverLib PRIVATE
        /W4                     # Warning level 4
        #/WX                     # Treat warnings as errors (engine code only)
        /external:W0            # Suppress warnings from external headers
        /experimental:external  # Required for /external:W0 to work
        /analyze-               # Disable full static analysis on this target

        $<$<CONFIG:Debug>:/Od /Zi>
        $<$<CONFIG:RelWithDebInfo>:/O2 /Zi>
        $<$<CONFIG:Release>:/Ox>
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(SolverLib PRIVATE
        -Wall 
        -Wextra 
        -Wpedantic 
        #-Werror

        $<$<CONFIG:Debug>:-O0 -g>
        $<$<CONFIG:RelWithDebInfo>:-O2 -g>
        $<$<CONFIG:Release>:-O3>
    )
endif()