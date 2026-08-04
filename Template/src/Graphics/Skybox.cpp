#include "Skybox.h"
#include "Globals.h"

Skybox::Skybox()
    // : skyboxVAO(),
    //   skyboxVBO(square, sizeof(square), GL_STATIC_DRAW)
      //skyboxEBO(squareIndices, sizeof(squareIndices))
{   
    
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (unsigned int i = 0; i < 6; i++)
	{
		int widthImg, heightImg, numColCh;
		unsigned char* bytes = stbi_load(facesCubemap[i].c_str(), &widthImg, &heightImg, &numColCh, 0);
		if (bytes)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
			stbi_image_free(bytes);
		}
		else
		{
			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(bytes);
		}
	}
}

void Skybox::Draw(const std::string& shaderName, glm::mat4 cameraMatrix)
{
    ShaderManager::Activate(shaderName);
    glUniformMatrix4fv(ShaderManager::GetUniformLoc(shaderName, "cameraMatrix"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
    glUniform1i(ShaderManager::GetUniformLoc(shaderName, "skybox"), 0);

    glDisable(GL_CULL_FACE);
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}

Skybox::~Skybox()
{
    // skyboxVAO.Delete();
    // skyboxEBO.Delete();
    // skyboxVBO.Delete();
    //glDeleteTextures(1, &cubemapTexture);
}