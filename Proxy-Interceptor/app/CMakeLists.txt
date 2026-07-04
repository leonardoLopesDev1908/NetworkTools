add_library(AppInterceptor STATIC "src/ProxyApp.cpp")
target_include_directories(AppInterceptor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(AppInterceptor PUBLIC Proxy Message ftxui::screen ftxui::dom ftxui::component)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(AppInterceptor PRIVATE ws2_32)
endif()

target_precompile_headers(AppInterceptor PRIVATE 
    <ftxui/screen/screen.hpp> 
    <ftxui/dom/elements.hpp> 
    <ftxui/component/component.hpp>
)

