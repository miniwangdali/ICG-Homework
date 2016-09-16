#include "glsupport.h"
#include <iostream>
// global variables
GLuint program;
GLuint myTexture;
// VBOs
GLuint vertexPositionVBO;
GLuint textureCoordinateVBO;
// attributes
GLuint positionAttribute;
GLuint textureCoordinateAttribute;
// uniform
GLuint positionUniform;
GLuint mouseUniform;
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionVBO);
	glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0); // each point has 2 value for position
	glEnableVertexAttribArray(positionAttribute);
	
	glBindBuffer(GL_ARRAY_BUFFER, textureCoordinateVBO);
	glVertexAttribPointer(textureCoordinateAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(textureCoordinateAttribute);
	glBindTexture(GL_TEXTURE_2D, myTexture);

	glUniform2f(positionUniform, -0.5, 0.0);
	glDrawArrays(GL_TRIANGLES, 0, 6); // draw

	glUniform2f(positionUniform, 0.5, 0.0);
	glDrawArrays(GL_TRIANGLES, 0, 6); // draw

	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(textureCoordinateAttribute);

	glutSwapBuffers();
}
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
}
void idle(void) {
	glutPostRedisplay();
}
void mouse(int button, int state, int x, int y) {
	float mouseX = 1.0f - (float)x / 250.0f;
	float mouseY = 1.0f - (float)y / 250.0f;
	glUniform2f(mouseUniform, mouseX, mouseY);
}
void mouseMove(int x, int y) {
	float mouseX = 1.0f - (float)x / 250.0f;
	float mouseY = 1.0f - (float)y / 250.0f;
	glUniform2f(mouseUniform, mouseX, mouseY);
}

void init() {
	// need to initialize glew before create program
	GLenum err = glewInit();
	if (err != GLEW_OK)
		throw std::runtime_error("glewInit failed");

	program = glCreateProgram();
	readAndCompileShader(program, "vertex.glsl", "fragment.glsl");

	glUseProgram(program);

	myTexture = loadGLTexture("Microsoft.png");

	// get attributes
	positionAttribute = glGetAttribLocation(program, "position");
	textureCoordinateAttribute = glGetAttribLocation(program, "texCoord");

	//get uniform
	positionUniform = glGetUniformLocation(program, "modelPosition");
	mouseUniform = glGetUniformLocation(program, "mousePos");

	// create vertex position VBOs
	glGenBuffers(1, &vertexPositionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionVBO);
	GLfloat rectangleVertex[12] = { // 2 * 3 * 2
		// first triangle
		-0.5f, -0.5f,
		0.5f, 0.5f,
		0.5, -0.5f,
		// second triangle
		-0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, 0.5f
	};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), rectangleVertex, GL_STATIC_DRAW);

	// create texture coodinate VBOs
	glGenBuffers(1, &textureCoordinateVBO);
	glBindBuffer(GL_ARRAY_BUFFER, textureCoordinateVBO);
	GLfloat textureCoordinate[12] = { // 2 * 3 * 2
		// first triangle
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0, 1.0f,
		// second triangle
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), textureCoordinate, GL_STATIC_DRAW);

}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Assignment1");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);

	// enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1, 1, 1, 1.0);

	init();
	glutMainLoop();
	return 0;
}