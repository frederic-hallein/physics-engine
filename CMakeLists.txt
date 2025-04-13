cmake_minimum_required(VERSION 3.28.3)
set (CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(physics-engine VERSION 1.0)
cmake_policy(SET CMP0074 NEW)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.c )
add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)