add_library(Proxy STATIC 
    "../connection_handler/src/Intercept.cpp"
    "src/Proxy.cpp"
    "src/ThreadPool.cpp"
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(Proxy PRIVATE ws2_32)
endif()

target_include_directories(Proxy PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Proxy PUBLIC CHandler CommonUtilities)
