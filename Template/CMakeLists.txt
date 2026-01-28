# CMakeList.txt : CMake project for CMakeTry, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (${PROJECT_NAME} "src/main.cpp" "src/main.h")
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
