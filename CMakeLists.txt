cmake_minimum_required(VERSION 3.28.3)
project(physics-engine VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0074 NEW)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    src/*.c
    src/*.hpp
)
file(GLOB IMGUI_SRC
    lib/imgui/*.cpp
    lib/imgui/backends/imgui_impl_glfw.cpp
    lib/imgui/backends/imgui_impl_opengl3.cpp
)
add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})
target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/backends
)
target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL assimp)
