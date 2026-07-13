add_library(Platform INTERFACE)

set(CMAKE_CXX_STANDARD 23)

target_include_directories(Platform INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
 ${PCAP_INCLUDE_DIRS}
)

target_link_libraries(Platform INTERFACE ${PCAP_LIBRARIES})
