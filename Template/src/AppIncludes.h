// Main includes
#pragma once

// System
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <vector>

// Graphic setup
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Benchmarks
#include "Benchmarks/Instrumentatior.h"

// Graphics
#include "Graphics/Window.h"
#include "Graphics/ShaderClass.h"
#include "Graphics/VAO.h"
#include "Graphics/VBO.h"
#include "Graphics/EBO.h"
#include "Graphics/Texture.h"
#include "Graphics/Camera.h"
#include "Graphics/ImguiSetup.h"