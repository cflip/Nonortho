SET(SOURCES
	bitmap.cpp
	level.cpp
	main.cpp
	train.cpp
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
