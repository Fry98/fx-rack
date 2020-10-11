cmake_minimum_required (VERSION 2.6)

INCLUDE(CPack)
INCLUDE(FindPkgConfig)
project (iimav)

OPTION (BUILD_SHARED_LIBS "Build shared libraries." ON)
include_directories(${CMAKE_SOURCE_DIR}/avlib/include)

add_definitions("-D_SCL_SECURE_NO_WARNINGS")
find_package(SDL)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(avlib/src)
add_subdirectory(avlib/include)

SET(EX_LIBS iimav)
add_executable(fx-rack main.cpp)
target_link_libraries (fx-rack  ${EX_LIBS})
install(TARGETS fx-rack RUNTIME DESTINATION bin)
