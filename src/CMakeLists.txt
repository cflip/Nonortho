SET(SOURCES
	bitmap.cpp
	level.cpp
	main.cpp
	train.cpp
	util.cpp
	window.cpp
)

add_executable(
	Nonortho
	${SOURCES}
)

target_include_directories(
	Nonortho
	PUBLIC
	${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
	Nonortho
	PRIVATE
	SDL2::SDL2
)

add_custom_command(
	TARGET Nonortho POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/res $<TARGET_FILE_DIR:Nonortho>/res
	COMMENT "Copying resources" VERBATIM
)
