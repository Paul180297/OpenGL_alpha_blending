#include <cstdio>
#include <cmath>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

static int WIN_WIDTH = 500;
static int WIN_HEIGHT = 500;
static const char *WIN_TITLE = "OpenGL Course";

static const double Pi = 4.0 * atan(1.0);

static float theta = 0.0f;

static const float positions[8][3] = {
	{ -1.0f, -1.0f, -1.0f },
	{  1.0f, -1.0f, -1.0f },
	{ -1.0f,  1.0f, -1.0f },
	{ -1.0f, -1.0f,  1.0f },
	{  1.0f,  1.0f, -1.0f },
	{ -1.0f,  1.0f,  1.0f },
	{  1.0f, -1.0f,  1.0f },
	{  1.0f,  1.0f,  1.0f }
};

static const float colors[6][4] = {
	{ 1.0f, 0.0f, 0.0f, 0.9f },
	{ 0.0f, 1.0f, 0.0f, 0.9f },
	{ 0.0f, 0.0f, 1.0f, 0.9f },
	{ 1.0f, 1.0f, 0.0f, 0.9f },
	{ 0.0f, 1.0f, 1.0f, 0.9f },
	{ 1.0f, 0.0f, 1.0f, 0.9f },
};

static const unsigned int indices[12][3] = {
	{ 1, 6, 7 }, { 1, 7, 4 },
	{ 2, 5, 7 }, { 2, 7, 4 },
	{ 3, 5, 7 }, { 3, 7, 6 },
	{ 0, 1, 4 }, { 0, 4, 2 },
	{ 0, 1, 6 }, { 0, 6, 3 },
	{ 0, 2, 5 }, { 0, 5, 3 },
};

static const unsigned int quadIndices[6][4] = {
	{ 1, 4, 7, 6 },
	{ 4, 2, 5, 7 },
	{ 2, 0, 3, 5 },
	{ 0, 1, 6, 3 },
	{ 1, 4, 2, 0 },
	{ 6, 7, 5, 3 }
};

void initializeGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);

}

void paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 1000.0f);
	//glOrtho(-2.0, 2.0, -2.0, 2.0, 0.1f, 1000.0f);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(4.0f, 6.0f, 8.0f,
		      0.0f, 0.0f, 0.0f,
		      0.0f, 1.0f, 0.0f);

	glRotatef(theta, 0.0f, 1.0f, 0.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
	//glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);


	glDepthMask(GL_FALSE);
	//glDisable(GL_DEPTH_TEST);

	//using triangles to draw a cube
	glPushMatrix();
	glTranslatef(1.0f, 0.0f, 1.0f);
	glScalef(0.5f, 0.5f, 0.5f);
	glRotatef(-1.0 * theta, 0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	for (int surface = 0; surface < 6; surface++) {
		glColor4fv(colors[surface]);

		for (int i = 0; i < 3; i++) {
			glVertex3fv(positions[indices[2 * surface + 0][i]]);
		}

		for (int j = 0; j < 3; j++) {
			glVertex3fv(positions[indices[2 * surface + 1][j]]);
		}
	}
	glEnd();
	glPopMatrix();

	//using quadrangles to draw a cube
	glPushMatrix();
	glTranslatef(-2.0f, -3.0f, -2.0f);
	glScalef(0.5f, 0.5f, 0.5f);
	glRotatef(5.0 * theta, 0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	for (int surface = 0; surface < 6; surface++) {
		glColor4fv(colors[surface]);

		for (int k = 0; k < 4; k++) {
			glVertex3fv(positions[quadIndices[surface][k]]);
		}
	}
	glEnd();
	glPopMatrix();

	glDisable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ZERO);
	glDepthMask(GL_TRUE);
	//glEnable(GL_DEPTH_TEST);
}

void resizeGL(GLFWwindow *window, int width, int height) {
	WIN_WIDTH = width;
	WIN_HEIGHT = height;

	glfwSetWindowSize(window, WIN_WIDTH, WIN_HEIGHT);

	int renderBufferWidth, renderBufferHeight;
	glfwGetFramebufferSize(window, &renderBufferWidth, &renderBufferHeight);

	glViewport(0, 0, renderBufferWidth, renderBufferHeight);
}

void animate() {
	theta += 1.0f;
}

int main(int argc, char **argv) {
	if (glfwInit() == GL_FALSE) {
		fprintf(stderr, "Initialization failed!\n");

		return 1;
	}

	GLFWwindow *window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "Window creation failed!");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, resizeGL);
	resizeGL(window, 1000.0f, 1000.0f);

	initializeGL();

	while (glfwWindowShouldClose(window) == GL_FALSE) {
		paintGL();

		animate();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
