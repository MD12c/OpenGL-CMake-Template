#ifndef SKYBOX_CLASS_H
#define SKYBOX_CLASS_H

#include <string>
#include <iostream>

#include "..\GLObjects\VAO.h"
#include "..\GLObjects\VBO.h"
#include "..\GLObjects\EBO.h"

#include "..\Shaders\ShaderManager.h"

#include "stb/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

class Skybox
{
public:
    static constexpr GLfloat skyboxVertices[24] = {
        -1.0f, -1.0f, 1.0f,   //        7--------6
        1.0f, -1.0f, 1.0f,    //       /|       /|
        1.0f, -1.0f, -1.0f,   //      4--------5 |
        -1.0f, -1.0f, -1.0f,  //      | |      | |
        -1.0f, 1.0f, 1.0f,    //      | 3------|-2
        1.0f, 1.0f, 1.0f,     //      |/       |/
        1.0f, 1.0f, -1.0f,    //      0--------1
        -1.0f, 1.0f, -1.0f
    };

    static constexpr GLuint skyboxIndices[36] = {
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

    VAO     skyboxVAO;
    VBO     skyboxVBO;
    EBO     skyboxEBO;
    GLuint* cubemapTexture       = nullptr;
    GLuint* irradiancemapTexture = nullptr;

    void genCube(GLuint* cube);
    void HDRtoCube(float* data, int widthImg, int heightImg, int resolution, GLuint cubeTexture);
    void CubeToIrradiance(GLuint srcCubemap, int resolution, GLuint dstCubemap);
    void DrawCaptureCube(int shaderID, glm::mat4 cameraMatrix, GLuint inputTexture) const;
    void bind() const
    {
        skyboxVAO.Bind();
        skyboxVBO.Bind();
        skyboxEBO.Bind();
    }
    void unbind() const
    {
        skyboxVAO.Unbind();
        skyboxEBO.Unbind();
        skyboxVBO.Unbind();
    }

public:
    Skybox(std::string facesCubemap[6]);
    Skybox(std::string HDRimage);
    ~Skybox();
    void Draw(int shaderID, glm::mat4 cameraMatrix) const;
    void ExportUniformsTo(int shaderID) const;
};

#endif