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
find_package(glfw3 3.4 REQUIRED)

# Include directories
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(libs/glfw-3.4.bin.WIN64/include)

# Create executable
add_executable(raytracer src/main.cpp src/Model.cpp src/Triangle.cpp src/Vec3.cpp)

# Link libraries to the executable
target_link_libraries(raytracer OpenGL::GL glfw)