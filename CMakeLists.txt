cmake_minimum_required(VERSION 2.8)

project(FifteenPuzzleGame)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_definitions(-DWIN32)
include_directories(${GLUT_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

file(GLOB_RECURSE SOURCES "./src/*.cpp")
file(GLOB_RECURSE HEADERS "./src/*.h")

add_executable(app ${HEADERS} ${SOURCES})
target_link_libraries(app ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

