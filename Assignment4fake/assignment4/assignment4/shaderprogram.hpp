#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include "glsupport.h"

class ShaderProgram {
public:
	GLuint program;
	// attributes
	GLuint positionAttribute;
public:
	ShaderProgram() {
		program = 0;
	}
	
	void enable() {
		glUseProgram(program);
	}
	void initial() {
		program = glCreateProgram();
		if (program == 0) {
			std::cout << "Error creating shader program\n" << std::endl;
		}
	}
	void loadShader(const std::string& vShader, const std::string& fShader) {
		readAndCompileShader(program, vShader.c_str(), fShader.c_str());
	}
	GLuint getAttribLocation(std::string attributeName) {
		GLuint attribute = glGetAttribLocation(program, attributeName.c_str());
		return attribute;
	}
	GLuint getUniformLocation(std::string uniformName) {
		GLuint location = glGetUniformLocation(program, uniformName.c_str());
		return location;
	}
};

#endif // !SHADERPROGRAM_HPP