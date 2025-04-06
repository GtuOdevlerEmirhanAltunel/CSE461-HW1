cmake_minimum_required(VERSION 3.10)
project(RayTracer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable OpenMP for multithreading
find_package(OpenMP)

# if(OpenMP_CXX_FOUND)
# message(STATUS "OpenMP found. Enabling multithreading.")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
# endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/inc)

# Add source files
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add executable
add_executable(RayTracer ${SOURCES})

# add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies/pugixml)
# target_link_libraries(RayTracer pugixml)

# Set optimization flags
if(CMAKE_BUILD_TYPE MATCHES Release)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")
endif()
