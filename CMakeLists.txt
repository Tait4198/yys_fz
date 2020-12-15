cmake_minimum_required(VERSION 3.17)
project(yys_fz)

set(CMAKE_CXX_STANDARD 17)

message($ENV{LIB_PATH})
message($ENV{VCPKG_PATH})

include_directories("$ENV{LIB_PATH}/opencv_vc16/include")
include_directories("$ENV{LIB_PATH}/onnxruntime/include")
include_directories("$ENV{LIB_PATH}/drogon/include")
include_directories("$ENV{VCPKG_PATH}/include")

link_directories("$ENV{LIB_PATH}/opencv_vc16/x64/vc16/lib")
link_directories("$ENV{LIB_PATH}/onnxruntime/lib")
link_directories("$ENV{LIB_PATH}/drogon/lib")
link_directories("$ENV{VCPKG_PATH}/lib")

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

add_executable(yys_fz main.cpp ${OCR} ${COMMON} ${GAME} ${CONTROLLER} game/ClientManager.cpp game/ClientManager.h main.cpp main.cpp)