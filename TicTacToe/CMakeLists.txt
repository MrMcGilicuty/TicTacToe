cmake_minimum_required(VERSION 3.12)
project(TicTacToe VERSION 1.0.0 LANGUAGES CXX)

# Set the path to the directory containing SFMLConfig.cmake
set(SFML_DIR "C:SFML-2.5.1/lib/cmake/SFML")

file(GLOB SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} C:TicTacToe/TicTacToe/MainClass.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE "include" "lib")
target_link_libraries(${PROJECT_NAME} sfml-graphics-d sfml-window-d sfml-system-d)

if (UNIX)
   target_link_libraries(${PROJECT_NAME} pthread)
endif (UNIX)

if(MSVC)
   foreach(lib ${SFML_LIBS})
      get_target_property(lib_path ${lib} LOCATION)
      file(COPY ${lib_path} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
   endforeach()
endif()