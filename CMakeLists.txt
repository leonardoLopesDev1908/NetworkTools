cmake_minimum_required(VERSION 3.16)
project(NetworkTools)

set(CMAKE_CXX_STANDARD 23)

if(MSVC)
    add_compile_options(/std:c++latest) 
endif()

include(FetchContent)
 
FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
    GIT_TAG main
)
 
FetchContent_MakeAvailable(ftxui)

find_package(Boost REQUIRED COMPONENTS program_options)
find_package(PkgConfig REQUIRED) 
pkg_check_modules(PCAP REQUIRED libpcap)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(Common)
add_subdirectory(Proxy-Interceptor)
add_subdirectory(Traffic-Analyzer)

add_executable(NetworkTools Main.cpp)

target_link_libraries(NetworkTools PRIVATE 
	Boost::program_options
	ProxyTool
	TrafficTool
)
