cmake_minimum_required(VERSION 3.16)
project(raytracer)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Suppress deprecated warnings on Apple platforms
if(APPLE)
    add_definitions(-Wno-deprecated-declarations)
endif()

# Find required packages
find_package(OpenGL REQUIRED)

add_subdirectory(external/glfw)
# Include directories
include_directories(${OPENGL_INCLUDE_DIR})

# Create executable
add_executable(raytracer src/main.c src/scene.c)

# Link libraries to the executable
target_link_libraries(raytracer OpenGL::GL glfw)