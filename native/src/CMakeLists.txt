cmake_minimum_required (VERSION 3.25.1)

project(native)
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_BUILD_TYPE Release)

include(cmake/CPM.cmake)
CPMAddPackage("gh:grpc/grpc@1.51.1")
CPMAddPackage("gh:SergiusTheBest/plog#1.1.9")

message(STATUS ${grpc_BINARY_DIR})


find_package(Java REQUIRED)

set(JAVA_AWT_LIBRARY NotNeeded)
set(JAVA_JVM_LIBRARY NotNeeded)
find_package(JNI REQUIRED)
if (JNI_FOUND)
    message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
endif()


add_subdirectory(cpp)
add_subdirectory(generated)