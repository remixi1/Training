#pragma once

#include <string>
#include <GL/glew.h>

class GLSLProgram
{
public:
	GLSLProgram();
	~GLSLProgram();

	void compileShaders(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath);

	void linkShaders();

	void addAttribute(const std::string& attributeName);

	GLuint getUniformLocation(const std::string& uniformName);

	void use();

	void unuse();

private:

	int m_numAttributes;

	void compileShader(const std::string & filePath, GLuint id);

	GLuint m_programID;

	GLuint m_vertexShaderID;
	GLuint m_fragmentShaderID;

};

