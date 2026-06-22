add_library(Platform INTERFACE)

set(CMAKE_CXX_STANDARD 23)

if(MSVC)
    add_compile_options(/std:c++latest) 
endif()

target_include_directories(Platform INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)