add_library(Packet STATIC 
    src/Packet.cpp
    src/IPClass.cpp
)

target_include_directories(Packet PRIVATE ${CMAKE_CURRENT_SOURCE_DIR/include)
