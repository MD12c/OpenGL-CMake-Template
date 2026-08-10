set(CORE_SOURCES src/App.h src/App.cpp src/main.cpp)

set(GRAPHICS_SOURCES
    src/Graphics/Window.h
    src/Graphics/Window.cpp
    src/Graphics/ImguiSetup.h
    src/Graphics/ImguiSetup.cpp
    src/Graphics/LightSystem.h
    src/Graphics/LightSystem.cpp
    src/Graphics/Scene.h
    src/Graphics/Scene.cpp
    src/Graphics/Renderer.h
    src/Graphics/Renderer.cpp)

set(VERTEX_BUFFERS_SOURCES
    src/Graphics/VertexBuffers/EBO.cpp
    src/Graphics/VertexBuffers/EBO.h
    src/Graphics/VertexBuffers/VAO.cpp
    src/Graphics/VertexBuffers/VAO.h
    src/Graphics/VertexBuffers/VBO.cpp
    src/Graphics/VertexBuffers/VBO.h
    src/Graphics/VertexBuffers/SSBO.h
    src/Graphics/VertexBuffers/SSBO.cpp)

set(SHADERS_SOURCES
    src/Graphics/Shaders/Shader.h src/Graphics/Shaders/Shader.cpp
    src/Graphics/Shaders/ShaderManager.h src/Graphics/Shaders/ShaderManager.cpp
    src/Graphics/Shaders/ShaderLoader.cpp)

set(CAMERA_SOURCES
    src/Graphics/Cameras/Camera.cpp
    src/Graphics/Cameras/Camera.h
    src/Graphics/Cameras/Fly.h
    src/Graphics/Cameras/Fly.cpp
    src/Graphics/Cameras/2Dcam.h
    src/Graphics/Cameras/2Dcam.cpp
    src/Graphics/Cameras/Orbit.h
    src/Graphics/Cameras/Orbit.cpp)

set(MODEL_SOURCES
    src/Graphics/Models/Mesh.h src/Graphics/Models/Mesh.cpp
    src/Graphics/Models/Model.h src/Graphics/Models/Model.cpp
    src/Graphics/Skybox.h src/Graphics/Skybox.cpp)

set(SHADOW_SOURCES
    src/Graphics/FrameBuffers/ShadowMaps/ShadowMap2D.h
    src/Graphics/FrameBuffers/ShadowMaps/ShadowMap2D.cpp
    src/Graphics/FrameBuffers/ShadowMaps/ShadowCaster.h
    src/Graphics/FrameBuffers/ShadowMaps/ShadowMapCube.h
    src/Graphics/FrameBuffers/ShadowMaps/ShadowMapCube.cpp
    src/Graphics/FrameBuffers/ShadowMaps/ShadowSystem.h
    src/Graphics/FrameBuffers/ShadowMaps/ShadowSystem.cpp)

set(FRAMEBUFFER_SOURCES
    ${SHADOW_SOURCES}
    src/Graphics/FrameBuffers/Texture.cpp
    src/Graphics/FrameBuffers/Texture.h
    src/Graphics/FrameBuffers/PostProcess.h
    src/Graphics/FrameBuffers/PostProcess.cpp
    src/Graphics/FrameBuffers/RBO.h
    src/Graphics/FrameBuffers/RBO.cpp
    src/Graphics/FrameBuffers/MSAABuffer.h
    src/Graphics/FrameBuffers/MSAABuffer.cpp)

add_executable(
  ${PROJECT_NAME}
  ${CORE_SOURCES}
  ${GRAPHICS_SOURCES}
  ${VERTEX_BUFFERS_SOURCES}
  ${SHADERS_SOURCES}
  ${CAMERA_SOURCES}
  ${MODEL_SOURCES}
  ${FRAMEBUFFER_SOURCES}
  include/libraries/Benchmarks/Instrumentatior.h)

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include src/)

# Copy Assets folder into .exe directory
add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/src/Assets"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets")

# Copy binary file into .exe directory
add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/include/binaries/assimp-vc143-mt.dll"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>")

# GLFW
add_library(glfw STATIC IMPORTED)
set_target_properties(
  glfw PROPERTIES IMPORTED_LOCATION
                  "${CMAKE_CURRENT_SOURCE_DIR}/include/lib/glfw3.lib")
target_include_directories(glfw INTERFACE include/libraries)

# GLAD
add_library(glad STATIC src/glad.c include/libraries/glad/glad.h)
target_include_directories(glad PRIVATE include/libraries)

# STB image
add_library(stb STATIC src/stb.cpp)
target_include_directories(stb PRIVATE include/libraries)

# GLM
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE include/libraries)

# ImGui
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

# Assimp
add_library(assimp STATIC IMPORTED)
set_target_properties(
  assimp
  PROPERTIES IMPORTED_LOCATION
             "${CMAKE_CURRENT_SOURCE_DIR}/include/lib/assimp-vc143-mt.lib")
target_include_directories(assimp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Linking
target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw stb glm imgui assimp)

if(CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()
