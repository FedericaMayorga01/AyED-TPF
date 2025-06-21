cmake_minimum_required(VERSION 3.16)
project(NetworkSimulator VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find nlohmann/json
find_package(nlohmann_json 3.2.0 REQUIRED)

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Include directories
include_directories(models/include)
include_directories(application/include)
include_directories(${Boost_INCLUDE_DIRS})

# Models library
file(GLOB MODELS_SOURCES "models/src/*.cpp")
add_library(models STATIC ${MODELS_SOURCES})

# Application library
file(GLOB APPLICATION_SOURCES "application/src/*.cpp")
add_library(application STATIC ${APPLICATION_SOURCES})

# Main executable
add_executable(network_simulator main.cpp)

# Link libraries
target_link_libraries(network_simulator models application)
# Link nlohmann_json
target_link_libraries(network_simulator models application nlohmann_json::nlohmann_json ${Boost_LIBRARIES})

# Set output directories
set_target_properties(network_simulator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

set_target_properties(models application PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Create directories
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Custom targets
add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/bin/network_simulator
    DEPENDS network_simulator
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running the network simulator"
)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "Cleaning all build files"
)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")