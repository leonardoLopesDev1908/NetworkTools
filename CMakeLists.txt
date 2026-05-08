cmake_minimum_required(VERSION 3.16)

project(ProxyInterceptor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(app)
add_subdirectory(proxy)
add_subdirectory(connectionhandler)
add_subdirectory(httpparser)
add_subdirectory(message)

add_executable(ProxyInterceptor Main.cpp)

target_link_libraries(ProxyInterceptor PRIVATE AppInterceptor)
