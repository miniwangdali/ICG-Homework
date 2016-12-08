#ifndef CUBEMAPREFRACTION_HPP
#define CUBEMAPREFRACTION_HPP

#include "matrix4.h"
#include "camera.hpp"
#include "shaderprogram.hpp"

class CubemapRefractionProgram : public ShaderProgram {
private:
	// attribute
	GLuint normalAttribute;
	// uniform
	GLuint textureLocation;
	GLuint modelViewMatrixLocation;
	GLuint projectionMatrixLocation;
	GLuint normalMatrixLocation;
public:
	void load(const std::string& vShader, const std::string& fShader) {
		loadShader(vShader, fShader);
		positionAttribute = getAttribLocation("position");
		normalAttribute = getAttribLocation("normal");
		textureLocation = getUniformLocation("environmentMap");
		modelViewMatrixLocation = getUniformLocation("modelViewMatrix");
		projectionMatrixLocation = getUniformLocation("projectionMatrix");
		normalMatrixLocation = getUniformLocation("normalMatrix");
	}
	void setModelViewMatrix(Matrix4 modelViewMatrix) {
		GLfloat glMatrix[16];
		modelViewMatrix.writeToColumnMajorMatrix(glMatrix);
		glUniformMatrix4fv(modelViewMatrixLocation, 1, false, glMatrix);

		Matrix4 normalMatrix = transpose(inv(modelViewMatrix));
		GLfloat glNormalMatrix[16];
		normalMatrix.writeToColumnMajorMatrix(glNormalMatrix);
		glUniformMatrix4fv(normalMatrixLocation, 1, false, glNormalMatrix);
	}
	void setProjectionMatrix(Camera* camera) {
		GLfloat glMatrixProjection[16];
		camera->getProjectionMatrix().writeToColumnMajorMatrix(glMatrixProjection);
		glUniformMatrix4fv(projectionMatrixLocation, 1, false, glMatrixProjection);
	}
	void setTexture(unsigned int textureUnit) {
		glUniform1i(textureLocation, textureUnit);
	}
};

#endif // !CUBEMAPREFRACTION_HPP
