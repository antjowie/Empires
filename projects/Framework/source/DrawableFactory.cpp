#include "DrawableFactory.h"
#include "Drawable.h"

#include "stb_image.h"

#include <iostream>

GLuint DrawableFactory::loadTexture(const char * texturePath)
{
	if (!texturePath)
		return 0;

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 4);
	
	if (!data)
	{
		std::printf("Failed to load texture: %s\n", texturePath);
		return 0;
	}

	GLuint &texID = m_textures[texturePath];
	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);

	return texID;
}

DrawableFactory::ModelData DrawableFactory::loadModel(MeshLoaderFunction meshLoader, const char * modelPath)
{
	ModelData data;

	if (!modelPath)
	{
		return data;
	}

	std::vector<GLfloat> vboBuffer;
	std::vector<GLushort> eboBuffer;
	meshLoader(modelPath, vboBuffer, eboBuffer, data.indicesCount);

	glGenBuffers(1, &data.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, data.vbo);
	glBufferData(GL_ARRAY_BUFFER, vboBuffer.size() * sizeof(GLfloat),&vboBuffer[0],GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glGenBuffers(1, &data.ebo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboBuffer.size(),&eboBuffer[0], GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return data;
}

void DrawableFactory::addDrawable(MeshLoaderFunction meshLoader, const char * objectName, const char * modelPath, const char * texturePath, GLuint program)
{
	// Create the draw data if it didn't exist
	if (!modelPath)
		modelPath = "nopath";
	if (!texturePath)
		texturePath = "nopath";
	if (m_drawData.count(std::string(objectName)) == 0)
	{
		if (m_meshes.count(std::string(modelPath)) == 0)
		{
			m_meshes[modelPath] = loadModel(meshLoader,modelPath);
		}
		if (m_textures.count(std::string(texturePath)) == 0)
		{
			m_textures[texturePath] = loadTexture(texturePath);
		}

		DrawData data;
		data.vbo = m_meshes[modelPath].vbo;
		data.ebo = m_meshes[modelPath].ebo;
		data.indicesCount = m_meshes[modelPath].indicesCount;
		data.textureID = m_textures[texturePath];
		data.program = program;
		data.posLoc = glGetAttribLocation (program, "aVerPos");
		if (glGetError() != GL_NO_ERROR)
			std::printf("Couldn't get vertex pos location while loading %s\n",objectName);

		data.texLoc = glGetAttribLocation (program, "aTexPos");
		if (glGetError() != GL_NO_ERROR)
			std::printf("Couldn't get texture pos location while loading %s\n", objectName);
		
		data.samLoc = glGetUniformLocation(program, "sampler"); // Don't know how to use multiple sampling so this is always 0
		if (glGetError() != GL_NO_ERROR)
			std::printf("Couldn't get sampler pos location while loading %s\n", objectName);
		
		m_drawData[objectName] = data;
	}
	else
	{
		std::printf("Type name \'%s\' already exists. Attempted to load twice\n",objectName);
		assert(false);
	}
}