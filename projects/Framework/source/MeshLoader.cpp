#include "MeshLoader.h"
#include "tiny_obj_loader.h"

#include <iostream>
#include <cmath>
#include <cassert>

void objLoader(const char *filename, std::vector<GLfloat> &vboBuffer, std::vector<GLushort> &eboBuffer, GLsizei &indicesCount)
{
	tinyobj::attrib_t attrib;
	// I'm not sure if .obj files can contain more than one mesh so the size of shapes should always be 1
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err = "File can't be loaded";

	if (tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename))
	{
		//assert("OBJ file contains more than one obj, observe the file" && shapes.size() == 1);
		
		// Convert vertex positions to range 1 and -1
		float max = 0.f;
		for (size_t i = 0; i < attrib.vertices.size(); i++)
		{
			GLfloat &pos = attrib.vertices[i];
			if (std::fabs(pos) > max)
				max = std::fabs(pos);
		}

		max = 1.f / max;

		for (size_t i = 0; i < attrib.vertices.size(); i++)
		{
			attrib.vertices[i] *= max;
			assert(attrib.vertices[i] <= 1.f && attrib.vertices[i] >= -1.f);
		}

		//vboBuffer.reserve(attrib.vertices.size() + attrib.texcoords.size());
		//vboBuffer = attrib.vertices;
		vboBuffer.clear();
		eboBuffer.clear();

		//vboBuffer.insert(vboBuffer.end(), attrib.vertices.begin(), attrib.vertices.end());
		//vboBuffer.insert(vboBuffer.end(), attrib.texcoords.begin(), attrib.texcoords.end());
		//for (size_t i = 0; i < attrib.vertices.size(); i += 3)
		//{
		//	//vboBuffer.push_back(attrib.vertices[i]);
		//	//vboBuffer.push_back(attrib.vertices[i+1]);
		//	//vboBuffer.push_back(attrib.vertices[i+2]);
		//	//vboBuffer.push_back(attrib.texcoords[texOffset++]);
		//	//vboBuffer.push_back(attrib.texcoords[texOffset++]);
		//}
		// I assume this to always only run ones
		for (size_t s = 0; s < shapes.size(); s++)
		{
			size_t vertexOffset = 0;

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				const unsigned sz = shapes[s].mesh.num_face_vertices[f];

				// Triangulate should be on so sz will always be 3
				// This loops over every vertex in the face
				for (size_t v = 0; v < sz; v++)
				{
					const tinyobj::index_t &index = shapes[s].mesh.indices[vertexOffset + v];
					// Position
					if (index.vertex_index != -1)
					{
						vboBuffer.push_back(attrib.vertices [3 * index.vertex_index + 0]);
						vboBuffer.push_back(attrib.vertices [3 * index.vertex_index + 1]);
						vboBuffer.push_back(attrib.vertices [3 * index.vertex_index + 2]);
					}

					if (index.texcoord_index != -1)
					{
						vboBuffer.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
						vboBuffer.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
					}
				}
				vertexOffset += sz;
			}
		}
		indicesCount = vboBuffer.size() / 5;
	}
	else
	{
		std::cout << "Mesh loader status: " << err << '\n';
	}
}