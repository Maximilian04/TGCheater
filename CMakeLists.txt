cmake_minimum_required(VERSION 3.0.0)
project(OpenCVExample VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(TGCheater
               main.cpp
               WindowsApi.cpp WindowsApi.h
               BrickStacker.cpp BrickStacker.h)

include_directories("D:/programming/libraries/opencv-4.5.5/build-release/install/include")
target_link_libraries(TGCheater "D:/programming/libraries/opencv-4.5.5/build-release/install/x64/mingw/bin/libopencv_world455.dll"
                                    "D:/programming/libraries/opencv-4.5.5/build-release/install/x64/mingw/bin/opencv_videoio_ffmpeg455_64.dll")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
