add_library(Tui STATIC "src/View.cpp")
target_include_directories(Tui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Tui PUBLIC Stats ftxui::screen ftxui::dom ftxui::component)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(Tui PRIVATE ws2_32)
endif()

target_precompile_headers(Tui PRIVATE 
    <ftxui/screen/screen.hpp> 
    <ftxui/dom/elements.hpp> 
    <ftxui/component/component.hpp>
)

