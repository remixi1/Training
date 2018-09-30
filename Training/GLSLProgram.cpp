#include "GLSLProgram.h"
#include "Errors.h"
#include <fstream>
# include <vector>

GLSLProgram::GLSLProgram() : m_numAttributes(0), m_programID(0), m_vertexShaderID(0), m_fragmentShaderID(0)
{
}


GLSLProgram::~GLSLProgram()
{
}
void GLSLProgram::compileShaders(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath) {

	m_programID = glCreateProgram();

	m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (m_vertexShaderID == 0) {
		fatalError(" VertexShader could not be created !");
	}
	m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (m_fragmentShaderID == 0) {
		fatalError("fragmentShader could not be created !");
	}
	compileShader(vertexShaderFilepath, m_vertexShaderID);
	compileShader(fragmentShaderFilepath, m_fragmentShaderID);

}

void GLSLProgram::linkShaders() {



	glAttachShader(m_programID, m_vertexShaderID);
	glAttachShader(m_programID, m_fragmentShaderID);

	glLinkProgram(m_programID);

	GLint isLinked = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE) {

		GLint maxLenght = 0;

		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLenght);

		std::vector<char> errorLog(maxLenght);
		glGetProgramInfoLog(m_programID, maxLenght, &maxLenght, &errorLog[0]);

		glDeleteProgram(m_programID);

		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);

		std::printf("%s\n", &errorLog[0]);
		fatalError("Shaders failed to link ! \n");
	}

	glDetachShader(m_programID, m_vertexShaderID);
	glDetachShader(m_programID, m_fragmentShaderID);

	glDeleteShader(m_vertexShaderID);
	glDeleteShader(m_fragmentShaderID);
}
void GLSLProgram::addAttribute(const std::string& attributeName) {
	glBindAttribLocation(m_programID, m_numAttributes++, attributeName.c_str());

}
GLuint GLSLProgram::getUniformLocation(const std::string& uniformName) {

	GLuint location = glGetUniformLocation(m_programID, uniformName.c_str());
	if (location == GL_INVALID_INDEX) {
		fatalError("Uniform" + uniformName + " not found in shader");
	}
	return location;
}
void GLSLProgram::use() {
	glUseProgram(m_programID);
	for (int i = 0; i < m_numAttributes; i++) {
		glEnableVertexAttribArray(i);
	}
}

void GLSLProgram::unuse() {
	glUseProgram(0);
	for (int i = 0; i < m_numAttributes; i++) {
		glDisableVertexAttribArray(i);
	}
}

void GLSLProgram::compileShader(const std::string & filePath, GLuint id) {

	std::ifstream vertexFile(filePath);
	if (vertexFile.fail()) {
		fatalError("Failed to open" + filePath);
	}
	std::string fileContents = "";
	std::string line;

	while (std::getline(vertexFile, line)) {
		fileContents += line + "\n";
	}
	vertexFile.close();

	const char* contentsPtr = fileContents.c_str();
	glShaderSource(id, 1, &contentsPtr, nullptr);

	glCompileShader(id);

	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		GLint maxLenght = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLenght);

		std::vector<char> errorLog(maxLenght);

		glGetShaderInfoLog(id, maxLenght, &maxLenght, &errorLog[0]);

		glDeleteShader(id);

		std::printf("%s\n", &errorLog[0]);
		fatalError("Shader" + filePath + " failed to compile ! \n");

	}
}
