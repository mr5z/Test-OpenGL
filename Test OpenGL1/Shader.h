#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader
{
public:

	// constructor reads and builds the shader
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	// the program ID
	unsigned int getId() const;
	// use/activate the shader
	void use();
	// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setFloat(const std::string& name, float x, float y, float z) const;

private:
	unsigned int createVertexShader(const std::string&);
	unsigned int createFragmentShader(const std::string&);
	unsigned int createShaderProgram(unsigned int, unsigned int);
	std::string getError(GLuint program, unsigned int statusType);

	// the program ID
	unsigned int id;
};

#endif