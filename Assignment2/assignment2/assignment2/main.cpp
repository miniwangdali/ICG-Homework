#include "glsupport.h"
#include "matrix4.h"
#include "geometrymaker.h"
#include "quat.h"

#include <iostream>
#include <direct.h>
#include <cstring>
#include <vector>
#include <math.h>

// global variables
GLuint program;

// attributes
GLuint positionAttribute;
GLuint normalAttribute;
// uniform
GLuint projectionMatrixUniformLocation;
GLuint modelViewMatrixUniformLocation;
GLuint normalMatrixUniformLocation;
GLuint colorUniformLocation;

struct VertexPN {
	Cvec3f position, normal;
	VertexPN() {};
	VertexPN(float posX, float posY, float posZ, float normalX, float normalY, float normalZ) : position(posX, posY, posZ), normal(normalX, normalY, normalZ) {};
	VertexPN& operator = (const GenericVertex& v) {
		position = v.pos;
		normal = v.normal;
		return *this;
	}
};
struct Transform {
	Quat rotation;
	//float rotationX;
	//float rotationY;
	//float rotationZ;
	Cvec3 scale;
	Cvec3 translation;
	Transform() : scale(1.0, 1.0, 1.0), translation(0.0, 0.0, 0.0) {};
	Matrix4 createMatrix() {
		Matrix4 origin;
		origin = Matrix4::makeScale(scale) * origin;
		//origin = Matrix4::makeXRotation(rotationX) * origin;
		//origin = Matrix4::makeYRotation(rotationY) * origin;
		//origin = Matrix4::makeZRotation(rotationZ) * origin;
		origin = quatToMatrix(rotation) * origin;
		origin = Matrix4::makeTranslation(translation) * origin;
		return origin;
	}
};
class Geometry{
protected:
	GLuint vertexBufferObject;
	GLuint indexBufferObject;
	int numIndices;
public:
	void Draw(GLuint positionAttribute, GLuint normalAttribute) {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glEnableVertexAttribArray(positionAttribute);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, position));

		glEnableVertexAttribArray(normalAttribute);
		glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, normal));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

		glDisableVertexAttribArray(positionAttribute);
		glDisableVertexAttribArray(normalAttribute);
	};
};
struct Entity {
	Transform transform;
	Geometry *geometry;
	Entity *parent;
	Entity() : parent(nullptr) {};
	void Draw(Matrix4 &eyeInverse, GLuint positionAttribute, GLuint normalAttribute,
		GLuint modelviewMatrixUniformLocation, GLuint normalMatrixUniformLocation) {
		Matrix4 modelViewMatrix = eyeInverse * getTransformMatrix();

		GLfloat glMatrix[16];
		modelViewMatrix.writeToColumnMajorMatrix(glMatrix);
		glUniformMatrix4fv(modelViewMatrixUniformLocation, 1, false, glMatrix);

		Matrix4 normalMatrix = transpose(inv(modelViewMatrix));
		GLfloat glNormalMatrix[16];
		normalMatrix.writeToColumnMajorMatrix(glNormalMatrix);
		glUniformMatrix4fv(normalMatrixUniformLocation, 1, false, glNormalMatrix);

		geometry->Draw(positionAttribute, normalAttribute);
	}
	Matrix4 getTransformMatrix() {
		if (parent == nullptr) return transform.createMatrix();
		else {
			return parent->getTransformMatrix() * transform.createMatrix();
		}
	}
};
class Cube : public Geometry {
public:
	Cube(float length) {
		int vertexBufferLength, indexBufferLength;
		getCubeVbIbLen(vertexBufferLength, indexBufferLength);
		numIndices = indexBufferLength;
		std::vector<VertexPN> vertices(vertexBufferLength);
		std::vector<unsigned short> indices(indexBufferLength);
		makeCube(length, vertices.begin(), indices.begin());
		// create vertex position VBOs
		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
		// create vertex index VBOs
		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), indices.data(), GL_STATIC_DRAW);
	}
};


Cube* cubeLv1 = nullptr;
Cube* cubeLv2 = nullptr;
Cube* cubeLv3 = nullptr;

Entity entityLv1;
Entity entityLv2;
Entity entityLv3;

Matrix4 makeLookAt(Cvec3 eye, Cvec3 center, Cvec3 up) {
	Matrix4 eyeMatrix;
	Cvec3 z = normalize(eye - center);
	Cvec3 y = normalize(up);
	Cvec3 x = cross(y, z);
	eyeMatrix[0] = x[0];
	eyeMatrix[1] = y[0];
	eyeMatrix[2] = z[0];
	eyeMatrix[3] = eye[0];
	eyeMatrix[4] = x[1];
	eyeMatrix[5] = y[1];
	eyeMatrix[6] = z[1];
	eyeMatrix[7] = eye[1];
	eyeMatrix[8] = x[2];
	eyeMatrix[9] = y[2];
	eyeMatrix[10] = z[2];
	eyeMatrix[11] = eye[2];
	eyeMatrix[12] = 0.0;
	eyeMatrix[13] = 0.0;
	eyeMatrix[14] = 0.0;
	eyeMatrix[15] = 1.0;
	return eyeMatrix;
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	if (cubeLv1 == nullptr) {
		cubeLv1 = new Cube(1.0f);
	}
	if (cubeLv2 == nullptr) {
		cubeLv2 = new Cube(1.0f);
	}
	if (cubeLv3 == nullptr) {
		cubeLv3 = new Cube(1.0f);
	}
	
	int time = glutGet(GLUT_ELAPSED_TIME);
	
	entityLv1.geometry = cubeLv1;
	//Quat rotationX = Quat::makeXRotation(45.0 * (float)time / 1000.0f);
	//entity.transform.rotation = rotationX;
	//entityLv1.transform.rotationX = 45.0 * (float)time / 1000.0f;
	entityLv1.transform.rotation = Quat::makeXRotation(45.0 * (float)time / 1000.0f);
	

	entityLv2.geometry = cubeLv2;
	entityLv2.transform.translation = Cvec3(2.0, 0.0, 0.0);
	entityLv2.parent = &entityLv1;
	//entityLv2.transform.rotationY = 45.0 * (float)time / 1000.0f;
	entityLv2.transform.rotation = Quat::makeYRotation(45.0 * (float)time / 1000.0f);
	entityLv2.transform.scale = Cvec3(1.5, 1.0, 1.0);

	entityLv3.geometry = cubeLv3;
	entityLv3.transform.translation = Cvec3(0.0, 2.0, 0.0);
	entityLv3.parent = &entityLv2;
	//entityLv3.transform.rotationZ = 45.0 * (float)time / 1000.0f;
	entityLv3.transform.rotation = Quat::makeZRotation(45.0 * (float)time / 1000.0f);

	Matrix4 eyeMatrix;
	// look at origin point (0, 0, 0)
	//eyeMatrix = eyeMatrix.makeTranslation(Cvec3(0.0, 0.0, 10.0));
	eyeMatrix = makeLookAt(Cvec3(10.0, 10.0, 10.0), Cvec3(0.0, 0.0, 0.0), Cvec3(0.0, 1.0, 0.0));


	Matrix4 projectionMatrix;
	projectionMatrix = projectionMatrix.makeProjection(45.0, 1.0, -0.1, -100.0);

	GLfloat glMatrixProjection[16];
	projectionMatrix.writeToColumnMajorMatrix(glMatrixProjection);
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, false, glMatrixProjection);

	
	glUniform3f(colorUniformLocation, 0.4f, 0.8f, 1.0f);

	//cube->Draw(positionAttribute, normalAttribute);

	entityLv1.Draw(inv(eyeMatrix), positionAttribute, normalAttribute, modelViewMatrixUniformLocation, normalMatrixUniformLocation);
	entityLv2.Draw(inv(eyeMatrix), positionAttribute, normalAttribute, modelViewMatrixUniformLocation, normalMatrixUniformLocation);
	entityLv3.Draw(inv(eyeMatrix), positionAttribute, normalAttribute, modelViewMatrixUniformLocation, normalMatrixUniformLocation);


	glutSwapBuffers();
}
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
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

	program = glCreateProgram();
	readAndCompileShader(program, "vertex.glsl", "fragment.glsl");

	glUseProgram(program);

	// get attributes
	positionAttribute = glGetAttribLocation(program, "position");
	normalAttribute = glGetAttribLocation(program, "normal");
	//get uniform
	modelViewMatrixUniformLocation = glGetUniformLocation(program, "modelViewMatrix");
	projectionMatrixUniformLocation = glGetUniformLocation(program, "projectionMatrix");
	normalMatrixUniformLocation = glGetUniformLocation(program, "normalMatrix");
	colorUniformLocation = glGetUniformLocation(program, "uniformColor");
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Assignment2");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	// enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1, 1, 1, 1.0);

	init();
	glutMainLoop();
	system("pause");
	return 0;
}