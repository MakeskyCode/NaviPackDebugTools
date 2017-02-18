#compile just for linux

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT (SdkInterface)
SET(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(CMAKE_CXX_FLAGS "-std=c++11 -Werror ${CMAKE_CXX_FLAGS}")
SET(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/Example/Linux/Lib)

# Set Source Files
file(GLOB_RECURSE SRC_LIST
"${SRCDIR}/commonSrc/*.cpp"
"${SRCDIR}/SdkInterfaceAndDemo/NaviPackSDK.cpp"
"${SRCDIR}/SdkInterfaceAndDemo/NavipackInterface.cpp"
)

INCLUDE_DIRECTORIES(${SRCDIR}/commonSrc)

#ADD_LIBRARY(SdkInterface ${SRC_LIST})
ADD_LIBRARY(SdkInterface SHARED ${SRC_LIST})  
TARGET_LINK_LIBRARIES(SdkInterface pthread )
INSTALL(FILES ${SRCDIR}/SdkInterfaceAndDemo/NaviPackSDK.h DESTINATION include)
INSTALL(TARGETS SdkInterface LIBRARY DESTINATION lib)     

#添加动态库  
#ADD_LIBRARY(SdkInterface SHARED ${SRC_LIST})  
#添加静态库  
#ADD_LIBRARY(SdkInterface_static STATIC ${SRC_LIST})