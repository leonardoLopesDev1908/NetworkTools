add_library(Packet STATIC 
    "src/Packet.cpp"
    "src/IP.cpp"
)

target_include_directories(Packet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Packet PUBLIC Platform)