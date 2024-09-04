cmake_minimum_required(VERSION 3.10)

project(GIRGCliques)

# setting the build type to debug when in the debug directory and to
# release otherwise
get_filename_component(build_dir ${CMAKE_CURRENT_BINARY_DIR} NAME)
if (${build_dir} MATCHES "debug")
  set(CMAKE_BUILD_TYPE Debug)
else()
  set(CMAKE_BUILD_TYPE Release)
endif()

# c++ version 20
set(CMAKE_CXX_STANDARD 20)

# export compile_commands.json, which can be used by editors for auto
# completion (e.g., using lsp)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# helping the compiler to find header files
include_directories(include lib)

# header, source and executable files
set(HEADERS
  lib/CLI11.hpp
  include/app.hpp
  include/timer.hpp
  include/random.hpp
  include/random_graph.hpp
  include/cliques.hpp
  )

set(SOURCE_FILES
  src/timer.cpp
  src/random.cpp
  src/random_graph.cpp
  src/cliques.cpp
  )

set(EXECUTABLE_FILES
  cli/cliques_in_core.cpp
  cli/cliques_in_gnp.cpp
  )

######################################################################
# build quick_cliques as library and use it
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -DALLOW_ALLOC_ZERO_BYTES)
set(QUICK_CLIQUES_SOURCE_DIR ../quick-cliques/src)
include_directories(${QUICK_CLIQUES_SOURCE_DIR})
set(QUICK_CLIQUES_SOURCES
    ${QUICK_CLIQUES_SOURCE_DIR}/CliqueTools.cpp
    ${QUICK_CLIQUES_SOURCE_DIR}/MemoryManager.cpp
    ${QUICK_CLIQUES_SOURCE_DIR}/Algorithm.cpp
    ${QUICK_CLIQUES_SOURCE_DIR}/TomitaAlgorithm.cpp
    ${QUICK_CLIQUES_SOURCE_DIR}/AdjacencyListAlgorithm.cpp
    ${QUICK_CLIQUES_SOURCE_DIR}/HybridAlgorithm.cpp
    ${QUICK_CLIQUES_SOURCE_DIR}/DegeneracyAlgorithm.cpp
    ${QUICK_CLIQUES_SOURCE_DIR}/DegeneracyTools.cpp
    ${QUICK_CLIQUES_SOURCE_DIR}/Tools.cpp
    )
add_library(quick_cliques SHARED ${QUICK_CLIQUES_SOURCES})

######################################################################
# create a target for every executable linking all source files and
# the cliques library
add_library(code STATIC ${SOURCE_FILES} ${HEADERS})
foreach (file ${EXECUTABLE_FILES})
    get_filename_component(TARGET ${file} NAME_WE)
    add_executable(${TARGET} ${file})
    target_include_directories(${TARGET} PRIVATE ${QUICK_CLIQUES_SOURCE_DIR})
    target_link_libraries(${TARGET} code quick_cliques)

    # show all warnings
    if(MSVC)
      target_compile_options(${TARGET} PRIVATE /W4 /WX)
    else()
      target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endforeach ()



