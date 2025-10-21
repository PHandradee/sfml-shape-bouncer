include(FetchContent)

set(SFML_BUILD_AUDIO FALSE)   # 👈 Desativa áudio
set(SFML_BUILD_NETWORK FALSE) # 👈 Opcional: desativa rede também

# SFML
FetchContent_Declare(sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
)
add_subdirectory(sfml)

# Dear ImGui
FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.91.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(imgui)

# ImGui-SFML
FetchContent_Declare(imgui-sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG v3.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
)
add_subdirectory(imgui-sfml)

