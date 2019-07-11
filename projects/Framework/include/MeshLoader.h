#pragma once
#include <GLES2/gl2.h>
#include <vector>

using MeshLoaderFunction = void(*)(const char *filename, std::vector<GLfloat> &vboBuffer, std::vector<GLushort> &eboBuffer, GLsizei &indicesCount);

void objLoader(const char *filename, std::vector<GLfloat> &vboBuffer, std::vector<GLushort> &eboBuffer, GLsizei &indicesCount);