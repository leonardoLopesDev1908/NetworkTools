add_library(Capture STATIC src/Capture.cpp)
target_include_directories(Capture PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PCAP_INCLUDE_DIRS}    
)
target_link_libraries(Capture PUBLIC
    ${PCAP_LIBRARIES}
    Packet
    Platform
    Stats
)