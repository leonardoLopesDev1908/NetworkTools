include(FetchContent)
 
FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
    GIT_TAG main
)
 
FetchContent_MakeAvailable(ftxui)

add_library(Message STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/QueueMessage.cpp)

target_include_directories(Message PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Message PUBLIC ftxui::screen ftxui::dom ftxui::component)

target_precompile_headers(Message PRIVATE 
    <ftxui/screen/screen.hpp> 
    <ftxui/dom/elements.hpp> 
    <ftxui/component/component.hpp>
)