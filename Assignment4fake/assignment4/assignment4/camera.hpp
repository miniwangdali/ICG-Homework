#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "cvec.h"
#include "matrix4.h"
#include "quat.h"

class Camera {
private:
	Cvec3 position;
	Cvec3 target;
	Cvec3 up;
	Matrix4 matrix;
	int windowWidth, windowHeight;
	float aspect;
	float fov;
	double zNear, zFar;
	Matrix4 projectionMatrix;
private:
	void initial() {
		projectionMatrix = projectionMatrix.makeProjection(fov, aspect, zNear, zFar);
		matrix = lookAt(position, target, up);
	}
	Cvec3 getArcBallVector(int x, int y) {
		Cvec3 point = Cvec3((double)x / (double)windowWidth - 0.5,
			(double)y / (double)windowHeight - 0.5, 0);
		point[1] = -point[1];
		double square = point[0] * point[0] + point[1] * point[1];
		if (square <= 0.25) {
			point[2] = sqrt(0.25 - square);
		}
		else {
			normalize(point);
		}
		return point;
	}
public:
	Camera(int width, int height) : position(0.0, 0.0, 1.0), target(0.0, 0.0, 0.0), up(0.0, 1.0, 0.0), fov(45.0), zNear(-0.1), zFar(-100.0) {
		windowHeight = height;
		windowWidth = width;
		aspect = (double)windowWidth / (double)windowHeight;
		initial();
	}
	Camera(int width, int height, Cvec3& position, Cvec3& target, Cvec3& up) : fov(45.0), zNear(-0.1), zFar(-1000.0) {
		windowHeight = height;
		windowWidth = width;
		this->position = position;
		this->target = target;
		this->up = up;
		normalize(this->up);
		aspect = (double)windowWidth / (double)windowHeight;
		initial();
	}
	Matrix4 getEyeMatrix() {
		matrix = lookAt(position, target, up);
		return matrix;
	}
	Matrix4 lookAt(Cvec3 eye, Cvec3 center, Cvec3 up) {
		Matrix4 eyeMatrix;
		Cvec3 z = normalize(eye - center);
		Cvec3 y = normalize(up);
		Cvec3 s = cross(y, z);
		normalize(s);
		Cvec3 x = cross(z, s);
		eyeMatrix[0] = s[0];
		eyeMatrix[1] = x[0];
		eyeMatrix[2] = z[0];
		eyeMatrix[3] = eye[0];
		eyeMatrix[4] = s[1];
		eyeMatrix[5] = x[1];
		eyeMatrix[6] = z[1];
		eyeMatrix[7] = eye[1];
		eyeMatrix[8] = s[2];
		eyeMatrix[9] = x[2];
		eyeMatrix[10] = z[2];
		eyeMatrix[11] = eye[2];
		eyeMatrix[12] = 0.0;
		eyeMatrix[13] = 0.0;
		eyeMatrix[14] = 0.0;
		eyeMatrix[15] = 1.0;
		
		return eyeMatrix;
	}
	void onMouseMove(int currentX, int currentY, int& previousX, int& previousY) {
		if (currentX != previousX || currentY != previousY) {
			Cvec3 vA = getArcBallVector(previousX, previousY);
			Cvec3 vB = getArcBallVector(currentX, currentY);
			Quat qA(0, vB);
			Quat qB(0, -vA);
			Quat rotation(qB * qA);
			Cvec4 newUp = rotation * Cvec4(up, 0.0);
			up = Cvec3(newUp[0], newUp[1], newUp[2]);
			Cvec4 newPos = rotation * Cvec4(position, 1.0);
			position = Cvec3(newPos[0], newPos[1], newPos[2]);
			previousX = currentX;
			previousY = currentY;
		}

	}
	void setFov(float v) {
		fov = v;
	}
	void setZNear(double v) {
		zNear = v;
	}
	void setZFar(double v) {
		zFar = v;
	}
	Matrix4 getProjectionMatrix() {
		return projectionMatrix;
	}
};

#endif // !CAMERA_HPP