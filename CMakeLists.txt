cmake_minimum_required(VERSION 3.22)
project (cloth_simulation)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Disable SFML audio, network, docs, and examples to save build time
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Build SFML from scratch
add_subdirectory(external/SFML)

# Add headers and source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Setup include paths
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link against the targets defined inside SFML's build script
target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)
