cmake_minimum_required(VERSION 3.16)
project(NetworkTools)
add_compile_definitions(NOMINMAX)

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
	
if(WIN32)
    set(PCAP_INCLUDE_DIRS "C:/Program Files/Npcap/Include")
    set(PCAP_LIBRARIES 
        "C:/Program Files/Npcap/Lib/x64/Packet.lib"
        "C:/Program Files/Npcap/Lib/x64/wpcap.lib"
    )
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PCAP REQUIRED libpcap)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(Common)
add_subdirectory(Proxy-Interceptor)
add_subdirectory(Traffic-Analyzer)

add_executable(NetworkTools Main.cpp "Common/include/Pcap.h")

target_link_libraries(NetworkTools PRIVATE 
	Boost::program_options
	ProxyTool
	TrafficTool
)
