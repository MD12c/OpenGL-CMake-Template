
add_executable (${PROJECT_NAME} 
    "src/main.cpp" "src/main.h"
    "include/libraries/Benchmarks/Instrumentatior.h"
    "src/Graphics/EBO.cpp" "src/Graphics/EBO.h"
    "src/Graphics/VAO.cpp" "src/Graphics/VAO.h"
    "src/Graphics/VBO.cpp" "src/Graphics/VBO.h"
    "src/Graphics/ShaderClass.cpp" "src/Graphics/ShaderClass.h"
    "src/Graphics/Texture.cpp" "src/Graphics/Texture.h"
    "src/Graphics/Window.cpp" "src/Graphics/Window.h"
    "src/Graphics/Camera.cpp" "src/Graphics/Camera.h"
    "src/Graphics/ImguiSetup.cpp" "src/Graphics/ImguiSetup.h"
)

message(STATUS "Shader source dir: ${CMAKE_CURRENT_SOURCE_DIR}/src/Assets/Shaders")

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Assets"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets"
)


add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/include/lib/glfw3.lib"
)
target_include_directories(glfw INTERFACE
    include/libraries
)


add_library(glad STATIC
    src/glad.c
    include/libraries/glad/glad.h
)
target_include_directories(glad PRIVATE
    include/libraries
)


add_library(stb STATIC src/stb.cpp)
target_include_directories(stb PRIVATE
    include/libraries
)


add_library(glm INTERFACE)
target_include_directories(glm INTERFACE
    include/libraries
)

add_library(imgui STATIC
    include/imgui/imgui.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_demo.cpp
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/imgui_impl_glfw.cpp
    include/imgui/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    include/imgui
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE glad glfw stb glm imgui  
)
target_link_libraries(imgui PUBLIC glfw glad)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
