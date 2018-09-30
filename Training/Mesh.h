#pragma once

#include <GL/glew.h>


class Mesh
{
public:
	Mesh();
	~Mesh();

	void init();

	void draw();


private:

	GLuint m_vboID;
	GLuint m_colorBuffer;
};

