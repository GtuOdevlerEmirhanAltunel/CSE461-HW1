cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER g++)

project(RayTracer)

include_directories(${CMAKE_SOURCE_DIR}/inc)

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(RayTracer ${SOURCES})

add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies/pugixml)
target_link_libraries(RayTracer pugixml)

target_compile_options(RayTracer PRIVATE "-pthread")
