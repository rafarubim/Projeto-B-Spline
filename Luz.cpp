#include "Luz.hpp"

#include "glut.h"
#include "iostream"
using namespace std;

Luz::Luz(): iluminacao_(Iluminacao()) {

	cout << "Erro: Nao se pode criar uma luz sem a referencia da iluminacao geral!\n\n";
	exit(1);
}

Luz::Luz(Iluminacao &iluminacao): iluminacao_(iluminacao) {

	luz_ = iluminacao_.getNextLight();

	if (luz_ == -1) {

		cout << "Erro: Luz incriavel, limite atingido\n\n";
		exit(1);
	}
	
	iluminacao_.newLight(luz_);

	ligar();

	pos_.set(0, 0, 0, 1);
	cor_.set(1, 1, 1, 1);

	posicao(pos_[X], pos_[Y], pos_[Z], pos_[W]);
	cor(pos_[0], pos_[1], pos_[2], pos_[3]);
}

Luz::~Luz() {

	desligar();
	iluminacao_.lightDestroyed(luz_);
}

void Luz::ligar() {

	glEnable(luz_);
}

void Luz::desligar() {

	glDisable(luz_);
}

void Luz::posicao(float x, float y, float z, float w) {

	pos_.set(x, y, z, w);

	float v[4];

	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;

	glLightfv(luz_, GL_POSITION, v);
}

void Luz::cor(float r, float g, float b, float a) {

	cor_.set(r, g, b, a);

	float v[4];

	v[0] = r;
	v[1] = g;
	v[2] = b;
	v[3] = a;

	glLightfv(luz_, GL_DIFFUSE, v);
}

void Luz::ambiente(bool sim, float alfa) {

	float v[4];

	v[0] = cor_[0] * alfa;
	v[1] = cor_[1] * alfa;
	v[2] = cor_[2] * alfa;
	v[3] = cor_[3] * alfa;

	glLightfv(luz_, GL_AMBIENT, v);
}