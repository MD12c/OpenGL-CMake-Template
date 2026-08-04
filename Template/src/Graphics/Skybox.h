#ifndef SKYBOX_CLASS_H
#define SKYBOX_CLASS_H

#include <string>
#include <iostream>

#include "VertexBuffers/VAO.h"
#include "VertexBuffers/VBO.h"
#include "VertexBuffers/EBO.h"

#include "Shaders/ShaderManager.h"

#include "stb/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

class Skybox
{
public:
    // VAO    skyboxVAO;
    // VBO    skyboxVBO;
    // EBO    skyboxEBO;
    // GLuint cubemapTexture;
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	unsigned int cubemapTexture;

    const std::string facesCubemap[6] = {
        "Assets/Textures/Skybox/right.jpg",
        "Assets/Textures/Skybox/left.jpg",
        "Assets/Textures/Skybox/top.jpg",
        "Assets/Textures/Skybox/bottom.jpg",
        "Assets/Textures/Skybox/front.jpg",
        "Assets/Textures/Skybox/back.jpg"
    };

    const GLfloat skyboxVertices[24] = {
        -1.0f, -1.0f, 1.0f,   //        7--------6
        1.0f, -1.0f, 1.0f,    //       /|       /|
        1.0f, -1.0f, -1.0f,   //      4--------5 |
        -1.0f, -1.0f, -1.0f,  //      | |      | |
        -1.0f, 1.0f, 1.0f,    //      | 3------|-2
        1.0f, 1.0f, 1.0f,     //      |/       |/
        1.0f, 1.0f, -1.0f,    //      0--------1
        -1.0f, 1.0f, -1.0f
    };

    const GLuint skyboxIndices[36] = {
        // Right
        6, 2, 1,
        1, 5, 6,
        // Left
        7, 4, 0,
        0, 3, 7,
        // Top
        6, 5, 4,
        4, 7, 6,
        // Bottom
        2, 3, 0,
        0, 1, 2,
        // Back
        5, 1, 0,
        0, 4, 5,
        // Front
        6, 7, 3,
        3, 2, 6
    };

public:
    Skybox();
    ~Skybox();
    void Draw(const std::string& shaderName, glm::mat4 cameraMatrix);
};

#endif