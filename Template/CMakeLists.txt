
add_executable (${PROJECT_NAME} 
    "src/main.cpp" "src/main.h"
    "src/Graphics/EBO.cpp" "src/Graphics/EBO.h"
    "src/Graphics/VAO.cpp" "src/Graphics/VAO.h"
    "src/Graphics/VBO.cpp" "src/Graphics/VBO.h"
    "src/Graphics/ShaderClass.cpp" "src/Graphics/ShaderClass.h"
    "src/Graphics/Texture.cpp" "src/Graphics/Texture.h"
    "src/Graphics/Window.cpp" "src/Graphics/Window.h"
)

message(STATUS "Shader source dir: ${CMAKE_CURRENT_SOURCE_DIR}/src/Assets/Shaders")

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Assets"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets"
)


add_library(glfw STATIC IMPORTED)
add_library(glad STATIC src/glad.c)
add_library(stb STATIC src/stb.cpp)
add_library(glm INTERFACE)

set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/include/lib/glfw3.lib"
)
target_include_directories(glfw INTERFACE
    include/libraries
)

target_include_directories(glad PUBLIC
    include/libraries
)

target_include_directories(stb PUBLIC
    include/libraries
)

target_include_directories(glm INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/libraries"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE glad glfw stb glm
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
