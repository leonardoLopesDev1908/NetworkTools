add_library(Stats STATIC "src/Stats.cpp")

target_include_directories(Stats PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Stats PUBLIC Packet Alerts)