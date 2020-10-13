cmake_minimum_required (VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
project(fx-rack)

OPTION (BUILD_SHARED_LIBS "Build shared libraries." ON)
include_directories(${CMAKE_SOURCE_DIR}/avlib/include)
include_directories(${CMAKE_JS_INC})

add_definitions("-D_SCL_SECURE_NO_WARNINGS")
find_package(SDL)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(avlib/src)
add_subdirectory(avlib/include)

add_library(${PROJECT_NAME} SHARED main.cpp SineGen.cpp ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries (fx-rack iimav ${CMAKE_JS_LIB})
