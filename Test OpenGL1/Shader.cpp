#include <fstream>
#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include "Shader.h"
#include "FileReader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	std::string vertexShaderSource = FileReader::readAllFiles(vertexShaderPath);
	std::string fragmentShaderSource = FileReader::readAllFiles(fragmentShaderPath);

	unsigned int vertexShader = createVertexShader(vertexShaderSource);
	unsigned int fragmentShader = createFragmentShader(fragmentShaderSource);
	id = createShaderProgram(vertexShader, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use() {
	glUseProgram(id);
}

unsigned int Shader::getId() const {
	return id;
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float x, float y, float z) const {
	GLint location = glGetUniformLocation(id, name.c_str());
	glUniform4f(location, x, y, z, 1.0f);
}

unsigned int Shader::createVertexShader(const std::string& source) {
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* cString = source.c_str();
	glShaderSource(vertexShader, 1, &cString, NULL);
	glCompileShader(vertexShader);

	std::string error = getError(vertexShader, GL_COMPILE_STATUS);
	if (error != "") {
		throw std::exception(error.c_str());
	}

	return vertexShader;
}

unsigned int Shader::createFragmentShader(const std::string& source) {
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* cString = source.c_str();
	glShaderSource(fragmentShader, 1, &cString, NULL);
	glCompileShader(fragmentShader);

	std::string error = getError(fragmentShader, GL_COMPILE_STATUS);
	if (error != "") {
		throw std::exception(error.c_str());
	}

	return fragmentShader;
}

unsigned int Shader::createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader) {
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	std::string error = getError(shaderProgram, GL_LINK_STATUS);
	if (error != "") {
		throw std::exception(error.c_str());
	}

	return shaderProgram;
}

std::string Shader::getError(GLuint program, unsigned int statusType) {
	int  success;
	GLchar buffer[512];
	GLsizei size = sizeof(buffer);
	glGetProgramiv(program, statusType, &success);
	if (!success) {
		glGetProgramInfoLog(program, size, NULL, buffer);
		return std::string(buffer);
	}
	return "";
}