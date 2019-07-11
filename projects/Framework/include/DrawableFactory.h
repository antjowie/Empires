#pragma once
#include "MeshLoader.h"
#include "Drawable.h"

#include <GLES2/gl2.h>
#include <unordered_map>
#include <memory>
#include <cassert>

class DrawableFactory
{
private:
	std::unordered_map<std::string, DrawData> m_drawData;
	
	// These maps keep track of already existing items
	// Say we have two different models who both use the same texture,
	// instead of making another texture, we will use the one that
	// already exists
	struct ModelData
	{
		GLuint vbo;
		GLuint ebo;
		GLsizei indicesCount;
	};
	std::unordered_map<std::string, GLuint> m_textures;
	std::unordered_map<std::string, ModelData> m_meshes;

	GLuint loadTexture(const char *texturePath);
	ModelData loadModel(MeshLoaderFunction meshLoader, const char *modelPath);

public:
	void addDrawable(MeshLoaderFunction meshLoader, const char* objectName, const char* modelPath, const char* texturePath, GLuint program);

	template<class DrawableType, class ReturnType = DrawableType>
	std::unique_ptr<ReturnType> createDrawable(const char *name) const;

};

template<class DrawableType, class ReturnType>
inline std::unique_ptr<ReturnType> DrawableFactory::createDrawable(const char * name) const
{
	assert(m_drawData.count(std::string(name)) == 1 && "Drawable type name doesn't exist");
	return std::unique_ptr<ReturnType>(std::make_unique<DrawableType>(m_drawData.at(name)));
}