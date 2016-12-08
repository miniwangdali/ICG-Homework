#include "glsupport.h"
#include "matrix4.h"
#include "quat.h"
#include "skybox.hpp"
#include "camera.hpp"
#include "Sphere.hpp"

#include <iostream>
#include <direct.h>
#include <vector>
#include <math.h>
#include <algorithm>

int windowHeight = 1024;
int windowWidth = 1024;

struct Light {
	GLuint lightPositionUniformLocation;
	GLuint lightColorUniformLocation;
	GLuint specularLightColorUniformLocation;
};

Light lights[2];

Camera* eye;
Skybox* skybox;

std::vector<Object3D*> myObjects;

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox->Draw();
	for (size_t i = 0; i < myObjects.size(); i++) {
		myObjects[i]->Render();
	}

	glutSwapBuffers();
}
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
}
int previousX = 0, previousY = 0, currentX = 0, currentY = 0;
bool controllerOn = false;


void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		controllerOn = true;
		previousX = x;
		previousY = y;
		currentX = x;
		currentY = y;
	}
	else {
		controllerOn = false;
	}
}
void mouseMove(int x, int y) {
	if (controllerOn) {
		currentX = x;
		currentY = y;
		eye->onMouseMove(currentX, currentY, previousX, previousY);
	}
}
void idle(void) {
	glutPostRedisplay();
}

void init() {
	// need to initialize glew before create program
	GLenum err = glewInit();
	if (err != GLEW_OK)
		throw std::runtime_error("glewInit failed");
	glClearDepth(0.0f);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
	glReadBuffer(GL_BACK);

	eye = new Camera(windowWidth, windowHeight, Cvec3(0.0, 0.0, 30.0), Cvec3(0.0, 0.0, 0.0), Cvec3(0.0, 1.0, 0.0));
	
	Cubemap* faces = new Cubemap("./right.png", "./left.png", "./top.png", "./bottom.png", "./front.png", "./back.png");
	skybox = new Skybox(eye);
	skybox->initial(faces);
	
	// add Objects
	CubemapReflectionSphere* cmrflSphere = new CubemapReflectionSphere();
	cmrflSphere->bindCamera(eye);
	cmrflSphere->bindCubemap(skybox->getCubemap());
	cmrflSphere->getTransform()->translation = Cvec3(-5.0, 0.0, 0.0);
	myObjects.push_back(cmrflSphere);

	CubemapRefractionSphere* cmrfrSphere = new CubemapRefractionSphere();
	cmrfrSphere->bindCamera(eye);
	cmrfrSphere->bindCubemap(skybox->getCubemap());
	cmrfrSphere->getTransform()->translation = Cvec3(5.0, 0.0, 0.0);
	myObjects.push_back(cmrfrSphere);

}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Assignment3");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	// enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1, 1, 1, 1.0);

	// enable culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);

	init();
	glutMainLoop();
	system("pause");
	return 0;
}