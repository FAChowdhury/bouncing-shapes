cmake_minimum_required(VERSION 3.0)

project(GAME)

# helpful compiler flags for gcc/clang
# the descriptions for these flags can be found on the GNU Compiler Collection's webpage.
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
  -Wold-style-cast
  -Wzero-as-null-pointer-constant
  -Wsuggest-override
  -fstack-protector-strong
)

###############################################################################

# Find SFML
find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)

# Add your source files
set(SOURCES src/main.cpp)

# Add an executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML libraries to your executable
target_link_libraries(${PROJECT_NAME} 
    sfml-graphics
    sfml-window
    sfml-system
)
