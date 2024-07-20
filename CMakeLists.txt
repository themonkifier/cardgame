cmake_minimum_required(VERSION 3.22)
project(Cards)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.c ${PROJECT_SOURCE_DIR}/src/*.cc ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/run.hh ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/include/*.hh ${PROJECT_SOURCE_DIR}/include/*.hpp)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(/home/obie/Apps/GLFW/glfw-3.3.9 /home/obie/Apps/GLFW/glfw-3.3.9-build)
add_subdirectory(/home/obie/Apps/FreeType/freetype-2.13.2 /home/obie/Apps/FreeType/freetype-2.13.2-build)
include_directories(/home/obie/Apps/glad/include)
find_package(OpenGL REQUIRED)

add_executable(cards run.cc /home/obie/Apps/glad/src/glad.c ${sources} ${headers})

target_compile_options(cards PRIVATE -Wall -Wextra -Wpedantic -std=c++20 -g)

target_link_libraries(cards z)
target_link_libraries(cards X11)
target_link_libraries(cards glfw)
target_link_libraries(cards OpenGL::GL)
target_link_libraries(cards freetype)

add_custom_target(Cards DEPENDS cards)
