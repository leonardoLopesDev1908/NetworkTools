add_library(Capture STATIC "src/Capture.cpp")

target_include_directories(Capture PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Capture PUBLIC 
    ${PCAP_LIBRARIES}
    Packet
    Platform
    Stats
)

#target_include_directories(SeuTarget PRIVATE ${PCAP_INCLUDE_DIRS})
#target_link_libraries(SeuTarget PRIVATE ${PCAP_LIBRARIES})Link pcap
