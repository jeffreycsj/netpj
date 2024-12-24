cmake_minimum_required(VERSION 3.20)

project(network)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dep/asio/include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dep/kcp/)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# OPEN_LOG 是否打印日志
add_compile_definitions(OPEN_LOG)

file(GLOB common common/*.cpp)
file(GLOB server server/*.cpp)
file(GLOB client client/*.cpp)

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  /utf-8 /Zi")
    
elseif(UNIX)
    if(ANDROID)
    	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g -DASIO_DISABLE_STD_IOSTREAM_MOVE -D__BYTE_ORDER=1234")
    elseif(APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g -D__BYTE_ORDER=1234")
        set(platform_libs pthread)
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g")
    endif() 
endif()


if(APPLE)
    add_library(networkc ${common} ${client} ${CMAKE_CURRENT_SOURCE_DIR}/dep/kcp/ikcp.c)
else()
    if(UNIX)
        add_library(networkc ${client} ${common})
    else()
        add_library(networkc SHARED ${client} ${common})
    endif()
if(NOT MSVC)
    add_library(network_server ${common} ${server} ${CMAKE_CURRENT_SOURCE_DIR}/dep/kcp/ikcp.c)
endif()
    

    add_library(network_client ${common} ${client}  ${CMAKE_CURRENT_SOURCE_DIR}/dep/kcp/ikcp.c)
    target_link_libraries(networkc network_client ${platform_libs}
    )
endif()

if(MSVC)
    add_compile_options("-D_WIN32_WINNT=0x0601")
    add_subdirectory(tests/client)
endif()

