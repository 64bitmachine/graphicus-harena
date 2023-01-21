cmake_minimum_required(VERSION 3.0.0)
project(HelloWorld VERSION 0.1.0)

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


add_executable(HelloWorld main.cpp)

# copy shaders folder to build folder
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if (APPLE)
    message("APPLE : Build for MacOS")
    target_link_libraries(HelloWorld glfw GLEW::GLEW "-framework OpenGL")
else()
    message("LINUX : Build for Linux")
    target_link_libraries(HelloWorld ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} glfw)
endif()

# freeglut
# find_package(GLUT REQUIRED)
# include_directories(${GLUT_INCLUDE_DIRS})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
