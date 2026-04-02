
set(SOURCES
  main.cpp
  shape.cpp
  circle_shape.cpp
  rectangle_shape.cpp
  config_manager.cpp
  shape_loader.cpp
)

set(HEADERS
  shape.hpp
  circle_shape.hpp
  rectangle_shape.hpp
  config_manager.hpp
  shape_loader.hpp
)

# Biblioteca principal para o executável e testes
add_library(shape_lib STATIC ${SOURCES} ${HEADERS})
target_link_libraries(shape_lib PRIVATE ImGui-SFML::ImGui-SFML)
target_include_directories(shape_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(shape_lib PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Executável principal
add_executable(sfml-shape-bouncer main.cpp)
target_link_libraries(sfml-shape-bouncer PRIVATE shape_lib)
