cmake_minimum_required(VERSION 3.16)
project(ChordTool VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Automatically run moc, uic, and rcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find the Qt6 package
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Add the executable
add_executable(ChordTool
    src/main.cpp
    src/Chord.cpp
)

# Link against the Qt libraries
target_link_libraries(ChordTool PRIVATE Qt6::Widgets)
