cmake_minimum_required(VERSION 3.0)

project(GAME)

# Find SFML
find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)

# Add your source files
set(SOURCES
    src/main.cpp
)

# Add an executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML libraries to your executable
target_link_libraries(${PROJECT_NAME} 
    sfml-graphics
    sfml-window
    sfml-system
)
