
add_executable(sfml-shape-bouncer main.cpp)
#target_link_libraries(sfml-shape-bouncer sfml-graphics sfml-window)
target_link_libraries(sfml-shape-bouncer PRIVATE ImGui-SFML::ImGui-SFML)
