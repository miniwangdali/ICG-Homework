#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "object3d.hpp"
#include "cubemap.hpp"
#include "cubemapreflectionprogram.hpp"
#include "cubemaprefractionprogram.hpp"


class CubemapReflectionSphere : public Object3D {
private:
	Camera* camera;
	Cubemap* cubemap;
	CubemapReflectionProgram* program;
public:
	CubemapReflectionSphere(float radius = 1.0f, int slices = 32, int stacks = 32) {
		this->camera = nullptr;
		this->cubemap = nullptr;
		this->program = new CubemapReflectionProgram();
		program->initial();
		program->load("./cubemapreflection.vertex.glsl", "./cubemapreflection.fragment.glsl");
		program->enable();
		program->setTexture(0);
		this->getGeometry()->sphere(radius, slices, stacks);
	}
	void bindCamera(Camera* camera) {
		this->camera = camera;
	}
	void bindCubemap(Cubemap* cubemap) {
		this->cubemap = cubemap;
	}
	void Render() {
		program->enable();
		Matrix4 modelViewMatrix = inv(camera->getEyeMatrix()) * getTransformMatrix();
		program->setModelViewMatrix(modelViewMatrix);
		program->setProjectionMatrix(camera);
		cubemap->bind(GL_TEXTURE0);
		Draw();

	}
};

class CubemapRefractionSphere : public Object3D {
private:
	Camera* camera;
	Cubemap* cubemap;
	CubemapRefractionProgram* program;
public:
	CubemapRefractionSphere(float radius = 1.0f, int slices = 32, int stacks = 32) {
		this->camera = nullptr;
		this->cubemap = nullptr;
		this->program = new CubemapRefractionProgram();
		program->initial();
		program->load("./cubemaprefraction.vertex.glsl", "./cubemaprefraction.fragment.glsl");
		program->enable();
		program->setTexture(0);
		this->getGeometry()->sphere(radius, slices, stacks);
	}
	void bindCamera(Camera* camera) {
		this->camera = camera;
	}
	void bindCubemap(Cubemap* cubemap) {
		this->cubemap = cubemap;
	}
	void Render() {
		program->enable();
		Matrix4 modelViewMatrix = inv(camera->getEyeMatrix()) * getTransformMatrix();
		program->setModelViewMatrix(modelViewMatrix);
		program->setProjectionMatrix(camera);
		cubemap->bind(GL_TEXTURE0);
		Draw();

	}
};

#endif // !SPHERE_HPP
