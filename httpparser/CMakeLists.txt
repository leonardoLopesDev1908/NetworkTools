add_library(Parser STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/HttpParser.cpp)

target_include_directories(Parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Parser PRIVATE Message)
