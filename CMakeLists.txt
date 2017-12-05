cmake_minimum_required(VERSION 2.8)

project(FifteenPuzzleGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
add_subdirectory(lib/soil)
include_directories(${GLUT_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ./lib/soil/src)

file(GLOB_RECURSE SOURCES "./src/*.cpp")
file(GLOB_RECURSE HEADERS "./src/*.h")

add_executable(app ${HEADERS} ${SOURCES})
target_link_libraries(app SOIL ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

add_custom_command(
        TARGET app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/res
                ${CMAKE_CURRENT_BINARY_DIR})
