include(FetchContent)
 
FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
    GIT_TAG main
)
 
FetchContent_MakeAvailable(ftxui)

add_library(CommonUtilities INTERFACE)

target_include_directories(CommonUtilities INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(CommonUtilities INTERFACE ftxui::screen ftxui::dom ftxui::component)