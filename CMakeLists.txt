cmake_minimum_required(VERSION 3.0)

project(GAME)

###############################################################################

# Find SFML
find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Include ImGui files
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/lib/imgui")
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui-SFML.cpp
)

# Create a static library for ImGui
add_library(imgui STATIC ${IMGUI_SOURCES})

# Apply global compiler flags for the rest of the project
# -Wold-style-cast disabled due to errors occuring in ImGui
add_compile_options(
  -Wall
  -Wextra
  -Werror
  -pedantic-errors
  -Wconversion
  -Wsign-conversion
  -Wdouble-promotion
  -Wcast-align
  -Wformat=2
  -Wuninitialized
  -Wnull-dereference
  -Wnon-virtual-dtor
  -Woverloaded-virtual
  -Wdeprecated-copy-dtor
  # -Wold-style-cast
  -Wzero-as-null-pointer-constant
  -Wsuggest-override
  -fstack-protector-strong
)

# Add your source files
set(SOURCES src/main.cpp)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link the ImGui and ImGui-SFML libraries to your executable
target_link_libraries(${PROJECT_NAME} 
    imgui
    sfml-graphics
    sfml-window
    sfml-system
    OpenGL::GL
)

# Set SFML include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${SFML_INCLUDE_DIR})

# Set ImGui include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUI_DIR})

