configure_file(CMakeLists.txt.in sdl2/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/sdl2 )
if(result)
  message(FATAL_ERROR "CMake step for sdl2 failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/sdl2 )
if(result)
  message(FATAL_ERROR "Build step for sdl2 failed: ${result}")
endif()

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/sdl2-src
                 ${CMAKE_CURRENT_BINARY_DIR}/sdl2-build)

