#ifndef SKYBOX_HPP
#define SKYBOX_HPP


#include "cubemap.hpp"
#include "camera.hpp"
#include "shaderprogram.hpp"
#include "object3d.hpp"

class SkyboxProgram : public ShaderProgram {
private:
	// uniforms
	GLuint textureLocation;
	GLuint modelViewMatrixLocation;
	GLuint projectionMatrixLocation;
public:
	void load(const std::string& vShader, const std::string& fShader) {
		loadShader(vShader, fShader);
		positionAttribute = getAttribLocation("position");
		textureLocation = getUniformLocation("environmentMap");
		modelViewMatrixLocation = getUniformLocation("modelViewMatrix");
		projectionMatrixLocation = getUniformLocation("projectionMatrix");
	}
	void setModelViewMatrix(Matrix4 modelViewMatrix) {
		GLfloat glMatrix[16];
		modelViewMatrix.writeToColumnMajorMatrix(glMatrix);
		glUniformMatrix4fv(modelViewMatrixLocation, 1, false, glMatrix);
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

class Skybox {
private:
	Camera* camera;
	Cubemap* cubemap;
	SkyboxProgram* skyboxProgram;
	Object3D* box;
public:
	Skybox(Camera* camera) {
		this->camera = camera;
		this->cubemap = nullptr;
	}
	~Skybox() {	}
	void initial(Cubemap* faces) {
		cubemap = faces;
		skyboxProgram = new SkyboxProgram();
		skyboxProgram->initial();
		skyboxProgram->load("./skybox.vertex.glsl", "./skybox.fragment.glsl");
		skyboxProgram->enable();
		skyboxProgram->setTexture(0);
		box = new Object3D();
		box->getGeometry()->cube(800);

	}
	void Draw() {
		skyboxProgram->enable();
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		Matrix4 modelViewMatrix = inv(camera->getEyeMatrix()) * box->getTransformMatrix();
		skyboxProgram->setModelViewMatrix(modelViewMatrix);
		skyboxProgram->setProjectionMatrix(camera);
		cubemap->bind(GL_TEXTURE0);
		box->Draw();

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	}
	Cubemap* getCubemap() {
		return cubemap;
	}
};
#endif // !SKYBOX_HPP