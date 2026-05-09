add_library(Message STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/QueueMessage.cpp)

target_include_directories(Message PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
