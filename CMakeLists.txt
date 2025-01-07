cmake_minimum_required(VERSION 3.30)
project(file_encoding)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
  Core
  Gui
  Widgets
  REQUIRED)

add_executable(
        file_encoding
        main.cpp
        huffman/Huffman.cpp
        huffman/Huffman.h
)
target_link_libraries(file_encoding
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
)

