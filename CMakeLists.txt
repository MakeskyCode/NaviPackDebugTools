#compile just for linux

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT (SdkInterface)
SET(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(CMAKE_CXX_FLAGS "-std=c++11 -Werror ${CMAKE_CXX_FLAGS}")

# Set Source Files
file(GLOB_RECURSE SRC_LIST
"${SRCDIR}/commonSrc/*.cpp"
"${SRCDIR}/SdkInterfaceAndDemo/NaviPackSDK.cpp"
"${SRCDIR}/SdkInterfaceAndDemo/NavipackInterface.cpp"
)

INCLUDE_DIRECTORIES(${SRCDIR}/commonSrc)

ADD_LIBRARY(SdkInterface ${SRC_LIST})
TARGET_LINK_LIBRARIES(SdkInterface pthread )
