cmake_minimum_required(VERSION 3.16)

project(ProxyInterceptor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(app)
add_subdirectory(common_utilities)
add_subdirectory(connection_handler)
add_subdirectory(httpparser)
add_subdirectory(message)
add_subdirectory(proxy)

add_executable(ProxyInterceptor Main.cpp)

target_link_libraries(ProxyInterceptor PRIVATE AppInterceptor)
