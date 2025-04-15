cmake_minimum_required(VERSION 3.10)
project(main)

set(CMAKE_CXX_STANDARD 14)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    /opt/ibm/ILOG/CPLEX_Studio2211/cplex/include
)

# Find source files recursively
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Add executable
add_executable(main ${SOURCES}
        include/utils/time_logger.h
        include/graph/graph.h
        include/constants.h
        include/utils/measurements.h
)

# Link libraries
target_link_libraries(main cplex m pthread dl)
