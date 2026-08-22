set(CORE_SOURCES src/App.h src/App.cpp src/main.cpp)

set(GRAPHICS_SOURCES
    src/Graphics/Window.h
    src/Graphics/Window.cpp
    src/Graphics/ImguiSetup.h
    src/Graphics/ImguiSetup.cpp
    src/Graphics/Scene.h
    src/Graphics/Scene.cpp
    src/Graphics/Renderer.h
    src/Graphics/Renderer.cpp)

set(VERTEX_BUFFERS_SOURCES
    src/Graphics/GLObjects/EBO.cpp
    src/Graphics/GLObjects/EBO.h
    src/Graphics/GLObjects/VAO.cpp
    src/Graphics/GLObjects/VAO.h
    src/Graphics/GLObjects/VBO.cpp
    src/Graphics/GLObjects/VBO.h
    src/Graphics/GLObjects/SSBO.h
    src/Graphics/GLObjects/SSBO.cpp
    src/Graphics/GLObjects/RBO.cpp
    src/Graphics/GLObjects/RBO.h
    src/Graphics/GLObjects/Texture.cpp
    src/Graphics/GLObjects/Texture.h
    src/Graphics/GLObjects/CubeTexture.cpp
    src/Graphics/GLObjects/CubeTexture.h)

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
    src/Graphics/Models/Skybox.h src/Graphics/Models/Skybox.cpp)

set(MATERIAL_SOURCES
    src/Graphics/Materials/Material.h
    src/Graphics/Materials/Material.cpp
    src/Graphics/Materials/SpecularMaterial.h
    src/Graphics/Materials/SpecularMaterial.cpp
    src/Graphics/Materials/PBRMaterial.h
    src/Graphics/Materials/PBRMaterial.cpp
    src/Graphics/Materials/MaterialManager.h
    src/Graphics/Materials/MaterialManager.cpp)

set(LIGHTING_SOURCES
    src/Graphics/Lighting/LightSystem.h
    src/Graphics/Lighting/LightSystem.cpp
    src/Graphics/Lighting/ShadowMap2D.h
    src/Graphics/Lighting/ShadowMap2D.cpp
    src/Graphics/Lighting/ShadowCaster.h
    src/Graphics/Lighting/ShadowMapCube.h
    src/Graphics/Lighting/ShadowMapCube.cpp
    src/Graphics/Lighting/ShadowSystem.h
    src/Graphics/Lighting/ShadowSystem.cpp)

set(FRAMEBUFFER_SOURCES
    src/Graphics/FrameBuffers/Bloom.h
    src/Graphics/FrameBuffers/Bloom.cpp
    src/Graphics/FrameBuffers/FrameBuffer.h
    src/Graphics/FrameBuffers/FrameBuffer.cpp
    src/Graphics/FrameBuffers/MSAABuffer.h
    src/Graphics/FrameBuffers/MSAABuffer.cpp
    src/Graphics/FrameBuffers/Square.h
    src/Graphics/FrameBuffers/Square.cpp
    src/Graphics/FrameBuffers/LUT.h
    src/Graphics/FrameBuffers/LUT.cpp
    src/Graphics/FrameBuffers/PostProcessSystem.h
    src/Graphics/FrameBuffers/PostProcessSystem.cpp)

set(UTILITY_SOURCES include/libraries/Benchmarks/Instrumentatior.h
                    include/libraries/Benchmarks/GPUtimer.h)

add_executable(
  ${PROJECT_NAME}
  ${CORE_SOURCES}
  ${GRAPHICS_SOURCES}
  ${LIGHTING_SOURCES}
  ${VERTEX_BUFFERS_SOURCES}
  ${SHADERS_SOURCES}
  ${CAMERA_SOURCES}
  ${MODEL_SOURCES}
  ${MATERIAL_SOURCES}
  ${FRAMEBUFFER_SOURCES}
  ${UTILITY_SOURCES})

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include src/)

set(TEMPLATE_OUTPUT_DIR "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")

# Copy Assets folder into .exe directory
add_custom_target(
  copy_assets ALL
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/src/Assets"
    "${TEMPLATE_OUTPUT_DIR}/Assets"
  COMMENT "Copying assets")

# Copy binary file into .exe directory
add_custom_target(
  copy_assimp_dll ALL
  COMMAND
    ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/include/binaries/assimp-vc143-mt.dll"
    "${TEMPLATE_OUTPUT_DIR}"
  COMMENT "Copying assimp DLL")
add_dependencies(${PROJECT_NAME} copy_assets copy_assimp_dll)

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
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
endif()
