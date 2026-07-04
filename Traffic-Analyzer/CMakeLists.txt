add_subdirectory(app)
add_subdirectory(tui)
add_subdirectory(capture)
add_subdirectory(packet)
add_subdirectory(stats)

add_library(TrafficTool INTERFACE)

target_link_libraries(TrafficTool INTERFACE 
	AppAnalyzer
)

