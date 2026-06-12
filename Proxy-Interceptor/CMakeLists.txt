add_subdirectory(app)
add_subdirectory(common_utilities)
add_subdirectory(connection_handler)
add_subdirectory(httpparser)
add_subdirectory(message)
add_subdirectory(proxy)

add_library(ProxyTool INTERFACE)
target_link_libraries(ProxyTool INTERFACE AppInterceptor)