add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  src/main.h
  include/libraries/Benchmarks/Instrumentatior.h
  src/Graphics/VertexBuffers/EBO.cpp
  src/Graphics/VertexBuffers/EBO.h
  src/Graphics/VertexBuffers/VAO.cpp
  src/Graphics/VertexBuffers/VAO.h
  src/Graphics/VertexBuffers/VBO.cpp
  src/Graphics/VertexBuffers/VBO.h
  src/Graphics/Shader.cpp
  src/Graphics/Shader.h
  src/Graphics/FrameBuffers/Texture.cpp
  src/Graphics/FrameBuffers/Texture.h
  src/Graphics/Window.cpp
  src/Graphics/Window.h
  src/Graphics/Cameras/Camera.cpp
  src/Graphics/Cameras/Camera.h
  src/Graphics/Cameras/2Dcam.h
  src/Graphics/Cameras/2Dcam.cpp
  src/Graphics/Cameras/Orbit.h
  src/Graphics/Cameras/Orbit.cpp
  src/Graphics/ImguiSetup.cpp
  src/Graphics/ImguiSetup.h
  src/Graphics/SSBO.h
  src/Graphics/SSBO.cpp
  src/Graphics/Models/Mesh.h
  src/Graphics/Models/Mesh.cpp
  src/Graphics/Models/Model.h
  src/Graphics/Models/Model.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src/)

message(
  STATUS "Shader source dir: ${CMAKE_CURRENT_SOURCE_DIR}/src/Assets/Shaders")

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/src/Assets"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets")

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/include/binaries/assimp-vc143-mt.dll"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>")

add_library(glfw STATIC IMPORTED)
set_target_properties(
  glfw PROPERTIES IMPORTED_LOCATION
                  "${CMAKE_CURRENT_SOURCE_DIR}/include/lib/glfw3.lib")
target_include_directories(glfw INTERFACE include/libraries)

add_library(glad STATIC src/glad.c include/libraries/glad/glad.h)
target_include_directories(glad PRIVATE include/libraries)

add_library(stb STATIC src/stb.cpp)
target_include_directories(stb PRIVATE include/libraries)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE include/libraries)

add_library(
  imgui STATIC
  include/libraries/imgui/imgui.cpp
  include/libraries/imgui/imgui_draw.cpp
  include/libraries/imgui/imgui_demo.cpp
  include/libraries/imgui/imgui_tables.cpp
  include/libraries/imgui/imgui_widgets.cpp
  include/libraries/imgui/imgui_impl_glfw.cpp
  include/libraries/imgui/imgui_impl_opengl3.cpp)
target_include_directories(imgui PUBLIC include/libraries/imgui)
target_link_libraries(imgui PUBLIC glfw glad)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/lib/assimp-vc143-mt.lib)
target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw stb glm imgui)

if(CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()
