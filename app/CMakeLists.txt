include(FetchContent)
 
FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
    GIT_TAG main
)
 
FetchContent_MakeAvailable(ftxui)

add_library(AppInterceptor STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/ProxyApp.cpp)
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
