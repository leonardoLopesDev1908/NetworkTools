set(SOURCES)
set(CHANDLER_INCLUDE_DIR)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/CHandlerLinux.cpp)
	set(CHANDLER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/linux)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/CHandlerWindows.cpp)
	set(CHANDLER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/windows)
endif()

add_library(CHandler STATIC ${SOURCES})

target_include_directories(CHandler PUBLIC
	${CHANDLER_INCLUDE_DIR}
)

target_link_libraries(CHandler PUBLIC Parser Message)