#ifndef SKYBOX_CLASS_H
#define SKYBOX_CLASS_H

#include <string>

#include "..\GLObjects\VAO.h"
#include "..\GLObjects\VBO.h"
#include "..\GLObjects\EBO.h"
#include "..\GLObjects/CubeTexture.h"
#include "..\GLObjects/Texture.h"

class Skybox
{
private:
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

    VAO skyboxVAO;
    VBO skyboxVBO;
    EBO skyboxEBO;

    std::unique_ptr<CubeTexture> cubemapTexture        = nullptr;
    std::unique_ptr<CubeTexture> irradiancemapTexture  = nullptr;
    std::unique_ptr<CubeTexture> prefilteredmapTexture = nullptr;

    void HDRtoCube(float* data, int widthImg, int heightImg, int resolution);
    void CubeToIrradiance(int resolution);
    void CubeToPrefiltered(int baseResolution);
    void DrawCaptureCube(ShaderID shaderID, glm::mat4 cameraMatrix, Texture& inputTexture) const;
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
    void Draw(ShaderID shaderID, glm::mat4 cameraMatrix) const;
    void ExportUniformsTo(ShaderID shaderID) const;
};

#endif