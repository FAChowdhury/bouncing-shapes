cmake_minimum_required(VERSION 3.5)

project(GAME)

if (NOT WIN32)

  # Set the C++ standard to C++11
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  ###############################################################################

  # Set the SFML paths explicitly
  set(SFML_INCLUDE_DIR "/opt/homebrew/Cellar/sfml/2.6.1/include") # needed for MACOS 
  # set(SFML_LIBRARY_DIR "/opt/homebrew/Cellar/sfml/2.6.1/lib")

endif()

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

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

# Ensure ImGui includes SFML headers
target_include_directories(imgui PRIVATE ${SFML_INCLUDE_DIR})

# Apply global compiler flags for the rest of the project
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
  #-Wzero-as-null-pointer-constant
  #-Wsuggest-override
  -fstack-protector-strong
)

if (WIN32)
  add_compile_options(
    -Wzero-as-null-pointer-constant
    -Wsuggest-override
  )
endif()

# Add your source files
set(SOURCES src/main.cpp)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include the SFML directories for the project executable
target_include_directories(${PROJECT_NAME} PRIVATE ${SFML_INCLUDE_DIR})

# Link the SFML library directories for the project executable
# target_link_directories(${PROJECT_NAME} PRIVATE ${SFML_LIBRARY_DIR})

# Link the ImGui and SFML libraries to your executable
target_link_libraries(${PROJECT_NAME} 
    imgui
    sfml-graphics
    sfml-window
    sfml-system
    OpenGL::GL
)

# Set ImGui include directories for the project executable
target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUI_DIR})
