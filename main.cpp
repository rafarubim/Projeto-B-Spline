#pragma comment(lib, "IL/DevIL.lib")
#pragma comment(lib, "IL/ILUT.lib")
#pragma comment(lib, "IL/ILU.lib")

#define WINDOW_WIDTH 850
#define WINDOW_HEIGHT 850

#include <stdlib.h>
#include "glut.h"
#include "IL/ilut.h"
#include "iostream"
using namespace std;

#include "Window.hpp"
#include "Graphic.hpp"
#include "Iluminacao.hpp"
#include "Luz.hpp"

#include "No.hpp"
#include "No.cpp"

#include "Lista.cpp"

Window window;
Graphic graphic;
Iluminacao iluminacao;
Lista<Lista<Vetor<double>>> listaCurvas; // Uma lista que guarda listas. Cada lista interna é uma lista de pontos que representa uma curva B-Spline


void mainLoop() {

	graphic.updateKeyboardDisplay();
	graphic.updateMousePos3D(window);
	graphic.updateMovingPoint(listaCurvas);
	graphic.updatePointSelection(listaCurvas, window);
}

void display() {

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	window.updateSize();

	graphic.config();

	Luz luz1(iluminacao);

	luz1.cor(0.7, 0.7, 0.7, 1);
	luz1.posicao(2, 1, 4, 01);
	luz1.ambiente(true, 0.5);

	iluminacao.desligar();
		graphic.eixos();
	iluminacao.ligar();

	//graphic.drawF(100);

	glColor3f(0.5, 0.5, 0.1);

	//glTranslatef(2, -2, 2);
		//glutSolidCube(2);
	//graphic.config();

	glColor3f(0, 0, 1);

	iluminacao.desligar();
		//glTranslatef(2, 2, 2);
			//glutSolidSphere(0.05, 40, 38);
		//graphic.config();
		graphic.drawPoints(listaCurvas, window);
	iluminacao.ligar();

	//luz1.desligar();
		graphic.drawPlano(iluminacao, listaCurvas);
	//luz1.ligar();

	iluminacao.desligar();
		graphic.drawTrackingBall(window);

		graphic.drawIcons(window);
		
		graphic.drawMessage(window);
	iluminacao.ligar();

	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {

	window.updateMouseClick(button, state);
	window.updateMousePos(x, y);
	graphic.updateMousePos3D(window);
	graphic.updateScale(button);
	graphic.updateButtonClick(window, button, state, listaCurvas);
	graphic.createPoints(listaCurvas, window, button, state);
	graphic.selectPoints(listaCurvas, button, state);
}

void motion(int x, int y) {

	window.updateMousePos(x, y);
	graphic.updateMousePos3D(window);
	graphic.updateRotation(window);
	graphic.updatePointSelection(listaCurvas, window);
	graphic.updateMovingPoint(listaCurvas);
	glutPostRedisplay();
}

void passiveMotion(int x, int y) {

	window.updateMousePos(x, y);
	graphic.updateMousePos3D(window);
	graphic.updatePointSelection(listaCurvas, window);
}

void keyboard(unsigned char key, int x, int y) {

	window.updateMousePos(x, y);
	graphic.updateMousePos3D(window);
	graphic.updatePointSelection(listaCurvas, window);
	graphic.updateKeyboard(key);
}

void keyboardRelease(unsigned char key, int x, int y) {

	window.updateMousePos(x, y);
	graphic.updateMousePos3D(window);
	graphic.updateKeyboardRelease(key);
	graphic.updatePointSelection(listaCurvas, window);
}

void specialKeyboard(int key, int x, int y) {

	window.updateMousePos(x, y);
	graphic.updateMousePos3D(window);
	graphic.updateKeyboard(key);
	graphic.updatePointSelection(listaCurvas, window);
}

void specialKeyboardRelease(int key, int x, int y) {

	window.updateMousePos(x, y);
	graphic.updateMousePos3D(window);
	graphic.updateKeyboardRelease(key);
	graphic.updatePointSelection(listaCurvas, window);
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	Vetor<int> initPos((glutGet(GLUT_SCREEN_WIDTH) - WINDOW_WIDTH)/2, (glutGet(GLUT_SCREEN_HEIGHT) - WINDOW_HEIGHT)/2);

	if (initPos[0] < 0)
		initPos[0] = 0;

	if (initPos[1] < 0)
		initPos[1] = 0;

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(initPos[0], initPos[1]);
	
	glutCreateWindow("Projeto B-Spline");

	glutDisplayFunc(display);
	glutIdleFunc(mainLoop);

	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passiveMotion);
	glutIgnoreKeyRepeat(true);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardRelease);
	glutSpecialFunc(specialKeyboard);
	glutSpecialUpFunc(specialKeyboardRelease);

	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	graphic.config();
	
	iluminacao.ligar();
	iluminacao.colorir();
	iluminacao.brilhoPadrao(true);
	iluminacao.obsLocal(true);
	iluminacao.doisLados(true);
	iluminacao.suave(true);

	glEnable(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //GL_LINEAR ou GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	graphic.carregarImagens();

	glutMainLoop();

	//glDeleteTextures(/*numero a deletar*/, GLuint* vetor);

	return 0;
}