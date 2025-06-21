cmake_minimum_required(VERSION 3.16)
project(tsp_solver CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -Wall -O2 -fPIC -Wno-format-truncation -Wno-sign-compare -Wno-reorder")

# CPLEX Library settings
set(CPLEX_DIR "/opt/ibm/ILOG/CPLEX_Studio2211/cplex")
set(CPLEX_INCLUDE_DIR "${CPLEX_DIR}/include")
set(CPLEX_LIB_DIR "${CPLEX_DIR}/lib/x86-64_linux/static_pic")

# Include directories
include_directories(${CPLEX_INCLUDE_DIR} include)

# Link directories
link_directories(${CPLEX_LIB_DIR})

# Add all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(main ${SOURCES})

# Link libraries
target_link_libraries(main cplex m pthread dl)
