cmake_minimum_required(VERSION 3.1)

project(FifteenPuzzleGame)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_subdirectory(soil)
include_directories(
	${GLUT_INCLUDE_DIR} 
	${OPENGL_INCLUDE_DIR} 
	soil/src
)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

add_executable(app ${HEADERS} ${SOURCES})
target_link_libraries(app 
	SOIL 
	${GLUT_LIBRARIES} 
	${OPENGL_LIBRARIES} 
)

file(GLOB SHARED_FILES "shared/*")

install(TARGETS app DESTINATION bin CONFIGURATIONS Release)
install(FILES ${SHARED_FILES} DESTINATION shared)
