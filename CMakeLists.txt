cmake_minimum_required(VERSION 3.0)
project(glMinimal2)

# compile the source
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src )
include_directories(${SRC_DIR})
set(SRC_FILES  
${SRC_DIR}/Camera.cpp 
${SRC_DIR}/Camera.h 
${SRC_DIR}/glad.c 
${SRC_DIR}/main.cpp
${SRC_DIR}/shader.h
${SRC_DIR}/particle.frag
${SRC_DIR}/particle.frag
)
add_executable(main ${SRC_FILES})

# set the 3rd party dependencies
set(TRD ${PROJECT_SOURCE_DIR}/3rd)

include_directories(${TRD}/glew/include)
include_directories(${TRD}/glfw3/include)
include_directories(${TRD}/glad/include)
include_directories(${TRD}/glm)

target_link_directories(main PRIVATE ${TRD}/glew/lib)
target_link_directories(main PRIVATE ${TRD}/glfw3/lib)

target_link_libraries(main glew32.lib glfw3.lib)