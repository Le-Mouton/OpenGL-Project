cmake_minimum_required(VERSION 3.27)

project(LearningProject)
set(CMAKE_CXX_STANDARD 17)

add_executable(LearningProject main.cpp
        Icosphere.cpp
        Scene.cpp
        Scene.h
        Shader.h
        glad.c
        Camera.h
        Plane.cpp
        Capsule.cpp
        Mesh.h
        MeshLoad.h
        Light.h
)

target_include_directories(LearningProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)


add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Include/GLFW/lib/libglfw3.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Include/GLFW")
target_link_libraries(LearningProject glfw)

