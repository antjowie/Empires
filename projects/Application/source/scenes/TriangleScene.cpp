#include "scenes/TriangleScene.h"
#include "stb_image.h"

#undef countof
#include <glm/glm/ext/matrix_transform.hpp>
#include <glm/glm/ext/matrix_clip_space.hpp>

#include <cassert>

static constexpr GLfloat vertices[] ={
	-0.5f,
	-0.5f,
	-0.5f,
	0.0f,
	0.0f,
	0.5f,
	-0.5f,
	-0.5f,
	1.0f,
	0.0f,
	0.5f,
	0.5f,
	-0.5f,
	1.0f,
	1.0f,
	0.5f,
	0.5f,
	-0.5f,
	1.0f,
	1.0f,
	-0.5f,
	0.5f,
	-0.5f,
	0.0f,
	1.0f,
	-0.5f,
	-0.5f,
	-0.5f,
	0.0f,
	0.0f,
	-0.5f,
	-0.5f,
	0.5f,
	0.0f,
	0.0f,
	0.5f,
	-0.5f,
	0.5f,
	1.0f,
	0.0f,
	0.5f,
	0.5f,
	0.5f,
	1.0f,
	1.0f,
	0.5f,
	0.5f,
	0.5f,
	1.0f,
	1.0f,
	-0.5f,
	0.5f,
	0.5f,
	0.0f,
	1.0f,
	-0.5f,
	-0.5f,
	0.5f,
	0.0f,
	0.0f,
	-0.5f,
	0.5f,
	0.5f,
	1.0f,
	0.0f,
	-0.5f,
	0.5f,
	-0.5f,
	1.0f,
	1.0f,
	-0.5f,
	-0.5f,
	-0.5f,
	0.0f,
	1.0f,
	-0.5f,
	-0.5f,
	-0.5f,
	0.0f,
	1.0f,
	-0.5f,
	-0.5f,
	0.5f,
	0.0f,
	0.0f,
	-0.5f,
	0.5f,
	0.5f,
	1.0f,
	0.0f,
	0.5f,
	0.5f,
	0.5f,
	1.0f,
	0.0f,
	0.5f,
	0.5f,
	-0.5f,
	1.0f,
	1.0f,
	0.5f,
	-0.5f,
	-0.5f,
	0.0f,
	1.0f,
	0.5f,
	-0.5f,
	-0.5f,
	0.0f,
	1.0f,
	0.5f,
	-0.5f,
	0.5f,
	0.0f,
	0.0f,
	0.5f,
	0.5f,
	0.5f,
	1.0f,
	0.0f,
	-0.5f,
	-0.5f,
	-0.5f,
	0.0f,
	1.0f,
	0.5f,
	-0.5f,
	-0.5f,
	1.0f,
	1.0f,
	0.5f,
	-0.5f,
	0.5f,
	1.0f,
	0.0f,
	0.5f,
	-0.5f,
	0.5f,
	1.0f,
	0.0f,
	-0.5f,
	-0.5f,
	0.5f,
	0.0f,
	0.0f,
	-0.5f,
	-0.5f,
	-0.5f,
	0.0f,
	1.0f,

	-0.5f,
	0.5f,
	-0.5f,
	0.0f,
	1.0f,
	0.5f,
	0.5f,
	-0.5f,
	1.0f,
	1.0f,
	0.5f,
	0.5f,
	0.5f,
	1.0f,
	0.0f,
	0.5f,
	0.5f,
	0.5f,
	1.0f,
	0.0f,
	-0.5f,
	0.5f,
	0.5f,
	0.0f,
	0.0f,
	-0.5f,
	0.5f,
	-0.5f,
	0.0f,
	1.0f
};

void TriangleScene::onCreate(const DrawableFactory & drawableFactory)
{
	glGenBuffers(1, &m_vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glGenBuffers(1, &m_ebo);
	//
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLuint tex;
	glGenTextures(1, &tex);
	int w, h, c;
	stbi_set_flip_vertically_on_load(true);
	unsigned char * data = stbi_load("resources/textures/Harvey2.jpg", &w, &h, &c, 4);

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	printf("Texture error: %i\n",glGetError());

	glUniform1i(0, 0);
	m_time = 0;
}

Scene * TriangleScene::run(const DrawableFactory & factory, float elapsedTime)
{
	//glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	
	m_rotations.x += 1.1f;
	m_rotations.x = glm::radians(m_rotations.x);

	m_rotations.y -= 1.1f;
	m_rotations.y = glm::radians(m_rotations.y);

	m_rotations.z += 1.1f;
	m_rotations.z = glm::radians(m_rotations.z);

	m_time += elapsedTime;
	glm::mat4 transform(1.f);
	glm::mat4 project = glm::perspective(45.f, 4.f / 3.f, 0.1f, 10.f);
	
	transform = glm::rotate(transform, glm::radians(m_time * 90.f), m_rotations);
	transform = project * glm::translate(glm::mat4(1.f),glm::vec3(0.f,0.f,-5.f)) * transform;

	glUniformMatrix4fv(1, 1, GL_FALSE, &transform[0][0]);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glGetError();
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	return nullptr;
}