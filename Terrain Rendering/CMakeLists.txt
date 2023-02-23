cmake_minimum_required(VERSION 3.2)

# every cmake project should contain project() command
project(Terrain_Rendering 
VERSION 0.1.0
DESCRIPTION "Terrain Rendering"
LANGUAGES CXX)

# c++17 flag
# set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# glfw
find_package(glfw3 3.3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

# glm
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})


add_executable(Terrain_Rendering
main.cpp
source/terrain.cpp
source/camera.cpp
source/shader.cpp
source/array2d.cpp)

# copy shaders folder to build folder
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if (APPLE)
    message("APPLE : Build for MacOS")
    target_link_libraries(Terrain_Rendering glfw GLEW::GLEW "-framework OpenGL")
else()
    message("LINUX : Build for Linux")
    target_link_libraries(Terrain_Rendering ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${GLM_LIBRARIES} glfw)
endif()

# freeglut
# find_package(GLUT REQUIRED)
# include_directories(${GLUT_INCLUDE_DIRS})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
