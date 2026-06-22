set(SOURCES)
set(CHANDLER_INCLUDE_DIR)

list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/CHandler.cpp)
set(CHANDLER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(CHandler ${SOURCES})

target_include_directories(CHandler PUBLIC
	${CHANDLER_INCLUDE_DIR}
)

target_link_libraries(CHandler PUBLIC 
	CommonUtilities Message 
	Parser Platform
)