cmake_minimum_required(VERSION 3.10)
project(Percolation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory for headers
include_directories(include)

# Collect source files (all .cpp in src folder)
file(GLOB SOURCES src/*.cpp)

# Create the executable
add_executable(Percolation ${SOURCES})

# Platform-specific hints for SFML
if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew" "/usr/local")
elseif(UNIX)
    list(APPEND CMAKE_PREFIX_PATH "/usr" "/usr/local")
elseif(WIN32)
    if(DEFINED ENV{SFML_DIR})
        set(CMAKE_PREFIX_PATH "$ENV{SFML_DIR}")
    endif()
endif()

# Find SFML (Graphics, Window, System modules)
find_package(SFML 3.0 REQUIRED COMPONENTS Graphics Window System)

# Link SFML libraries
target_link_libraries(Percolation PRIVATE SFML::Graphics SFML::Window SFML::System)
