cmake_minimum_required(VERSION 3.20)

# Download and configure doctest
include(FetchContent)
FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/doctest/doctest.git
  GIT_TAG v2.4.11
)
FetchContent_MakeAvailable(doctest)

add_executable(shape_tests test_shapes.cpp)
target_link_libraries(shape_tests PRIVATE 
    shape_lib 
    sfml-graphics 
    sfml-window 
    sfml-system 
    doctest::doctest
)
target_include_directories(shape_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(shape_tests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Enable testing
enable_testing()
add_test(NAME ShapeTests COMMAND shape_tests)