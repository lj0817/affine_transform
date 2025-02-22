cmake_minimum_required(VERSION 3.30)
project(affine_transformation)

set(CMAKE_CXX_STANDARD 17)
set(OpenCV_DIR E:\\Tools\\OpenCV\\opencv)
find_package(OpenCV REQUIRED)

aux_source_directory(./src SRC_LIST)
include_directories(${OpenCV_INCLUDE_DIRS})
#include_directories(include)
include_directories(./include)

add_executable(affine_transformation ${SRC_LIST} main.cpp
)


link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(affine_transformation ${OpenCV_LIBS})

