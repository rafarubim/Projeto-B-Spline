#include "Iluminacao.hpp"

#include "glut.h"
#include "iostream"
using namespace std;

Iluminacao::Iluminacao() {

	for(int i = 0; i < 8; i ++)
		luzes_[i] = false;
}

int Iluminacao::getNextLight() {

	for(int i = 0; i < 8; i++) {

		if (!luzes_[i])
			switch(i) {
			case 0: return GL_LIGHT0;
				break;
			case 1: return GL_LIGHT1;
				break;
			case 2: return GL_LIGHT2;
				break;
			case 3: return GL_LIGHT3;
				break;
			case 4: return GL_LIGHT4;
				break;
			case 5: return GL_LIGHT5;
				break;
			case 6: return GL_LIGHT6;
				break;
			case 7: return GL_LIGHT7;
				break;
			}
	}

	return -1;
}

void Iluminacao::ligar() {

	glEnable(GL_LIGHTING);
}

void Iluminacao::desligar() {

	glDisable(GL_LIGHTING);
}

void Iluminacao::obsLocal(bool sim) {

	if (sim)
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	else
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
}

void Iluminacao::doisLados(bool sim) {

	if (sim)
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE); // false e true estao trocados por motivos que não faço ideia, mas é assim que funciona...
	else
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

void Iluminacao::suave(bool sim) {

	if (sim)
		glShadeModel(GL_SMOOTH);
	else
		glShadeModel(GL_FLAT);
}

void Iluminacao::brilhoPadrao(bool sim) {

	if (sim)
		glEnable(GL_NORMALIZE);
	else
		glDisable(GL_NORMALIZE);
}

void Iluminacao::colorir() {

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
}

void Iluminacao::newLight(int light) {

	switch(light) {
			case GL_LIGHT0: light = 0;
				break;
			case GL_LIGHT1: light = 1;
				break;
			case GL_LIGHT2: light = 2;
				break;
			case GL_LIGHT3: light = 3;
				break;
			case GL_LIGHT4: light = 4;
				break;
			case GL_LIGHT5: light = 5;
				break;
			case GL_LIGHT6: light = 6;
				break;
			case GL_LIGHT7: light = 7;
				break;
			default:
				cout << "Erro em criacao de luz\n\n";
				exit(1);
	}

	luzes_[light] = true;
}

void Iluminacao::lightDestroyed(int light) {


	switch(light) {
			case GL_LIGHT0: light = 0;
				break;
			case GL_LIGHT1: light = 1;
				break;
			case GL_LIGHT2: light = 2;
				break;
			case GL_LIGHT3: light = 3;
				break;
			case GL_LIGHT4: light = 4;
				break;
			case GL_LIGHT5: light = 5;
				break;
			case GL_LIGHT6: light = 6;
				break;
			case GL_LIGHT7: light = 7;
				break;
			default:
				cout << "Erro em destruicao de luz\n\n";
				exit(1);
		}

	luzes_[light] = false;
}