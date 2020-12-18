cmake_minimum_required(VERSION 3.17)
project(yys_fz)

set(CMAKE_CXX_STANDARD 17)

#message($ENV{LIB_PATH})
#message($ENV{VCPKG_PATH})

include_directories("/Users/chenyitao/临时/ocr/opencv-3.4.12/include")
include_directories("/Users/chenyitao/临时/ocr/onnxruntime-osx-x64-1.5.2/onnxruntime/include")
#include_directories("$ENV{LIB_PATH}/drogon/include")
#include_directories("$ENV{VCPKG_PATH}/include")
include_directories("/usr/local/include")

link_directories("/Users/chenyitao/临时/ocr/opencv-3.4.12/include/lib")
link_directories("/Users/chenyitao/临时/ocr/onnxruntime-osx-x64-1.5.2/onnxruntime/lib")
#link_directories("$ENV{LIB_PATH}/drogon/lib")
#link_directories("$ENV{VCPKG_PATH}/lib")
link_directories("/usr/local/lib")

link_libraries(onnxruntime.lib
        opencv_core450.lib
        opencv_imgproc450.lib
        opencv_imgcodecs450.lib
        opencv_highgui450.lib
        drogon.lib
        trantor.lib
        jsoncpp.lib
        zlib.lib
        ws2_32.lib
        Rpcrt4.lib)

file(GLOB_RECURSE OCR ${PROJECT_SOURCE_DIR}/ocr/*.cpp ${PROJECT_SOURCE_DIR}/ocr/*.h)
file(GLOB_RECURSE COMMON ${PROJECT_SOURCE_DIR}/common/*.cpp ${PROJECT_SOURCE_DIR}/common/*.h)
file(GLOB_RECURSE GAME ${PROJECT_SOURCE_DIR}/game/*.cpp ${PROJECT_SOURCE_DIR}/game/*.h)
file(GLOB_RECURSE CONTROLLER ${PROJECT_SOURCE_DIR}/controller/*.cpp ${PROJECT_SOURCE_DIR}/controller/*.h)
file(GLOB_RECURSE TASK ${PROJECT_SOURCE_DIR}/game/task/*.cpp ${PROJECT_SOURCE_DIR}/game/task/*.h)

add_executable(yys_fz main.cpp ${OCR} ${COMMON} ${GAME} ${CONTROLLER} ${TASK})