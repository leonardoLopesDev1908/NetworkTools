add_library(AppAnalyzer STATIC "src/AnalyzerApp.cpp")
target_include_directories(AppAnalyzer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
