cmake_minimum_required(VERSION 3.2)

# every cmake project should contain project() command
project(HelloWorld 
VERSION 0.1.0
DESCRIPTION "Hello World"
LANGUAGES CXX)

# cmake type 
set(CMAKE_BUILD_TYPE Debug)

# set (CMAKE_CXX_STANDARD 11)

include(CTest)
enable_testing()

# use c++17
set(CMAKE_CXX_STANDARD 17)

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# glew
find_package(GLEW REQUIRED)
if(WIN32)
    # If the operating system is Windows, manually set the include directory to the GLEW folder
    set(GLEW_INCLUDE_DIRS "C:/OpenGL/glew-2.1.0/include")
else()
    # Otherwise, use the include directories provided by the GLEW package
    include_directories(${GLEW_INCLUDE_DIRS})
endif()

# glfw
find_package(glfw3 3.3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

# glm
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# /opt/local/include/SOIL include
include_directories("/opt/local/include/SOIL")
link_directories("/opt/local/lib")

add_executable(HelloWorld 
main.cpp
source/camera.cpp
source/shader.cpp)

# copy shaders folder to build folder
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if (APPLE)
    message("APPLE : Build for MacOS")
    target_link_libraries(HelloWorld glfw GLEW::GLEW "-framework OpenGL" "/opt/local/lib/libSOIL.dylib")
else()
    message("LINUX : Build for Linux")
    target_link_libraries(HelloWorld ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${GLM_LIBRARIES} glfw)
endif()

# freeglut
# find_package(GLUT REQUIRED)
# include_directories(${GLUT_INCLUDE_DIRS})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
