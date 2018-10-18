# Project definition
cmake_minimum_required(VERSION 3.1)
project(Voxagen)
set(PROJECT_NAME "Voxagen")

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

# Source files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

# Executable definition and properties
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

# GLFW
set(GLFW_DIR "${LIB_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

# glad
set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")
  
#imgui
#set(IMGUI_DIR "${LIB_DIR}/imgui")

#set(IMGUI_SOURCES
#"${IMGUI_DIR}/imgui.cpp"
#"${IMGUI_DIR}/imgui_demo.cpp"
#"${IMGUI_DIR}/imgui_draw.cpp"
#"${IMGUI_DIR}/imgui_impl_opengl3.cpp"
#"${IMGUI_DIR}/imgui_impl_glfw.cpp")

#set(IMGUI_HEADERS
#"${IMGUI_DIR}/imconfig.h"
#"${IMGUI_DIR}/imgui.h"
#"${IMGUI_DIR}/imgui_internal.h"
#"${IMGUI_DIR}/imstb_rectpack.h"
#"${IMGUI_DIR}/imstb_textedit.h"
#"${IMGUI_DIR}/imstb_truetype.h"
#"${IMGUI_DIR}/imgui_impl_opengl3.h"
#"${IMGUI_DIR}/imgui_impl_glfw.h")

#add_library(imgui ${IMGUI_SOURCES} ${IMGUI_HEADERS})

#target_link_libraries(${PROJECT_NAME} imgui)

