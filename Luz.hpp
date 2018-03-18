#pragma once

#include "Iluminacao.hpp"
#include "Vetor.hpp"
#include "Vetor.cpp"

class Luz { //Não criar luz fora de uma função, ou seu construtor não funcionará!

private:

	int luz_;
	Iluminacao &iluminacao_;

	Vetor<float> pos_;
	Vetor<float> cor_;

public:

	Luz();
	Luz(Iluminacao &iluminacao);
	~Luz();

	void ligar();
	void desligar();

	void posicao(float x, float y, float z) {

		posicao(x, y, z, pos_[3]);
	}

	void posicao(float x, float y, float z, float w);

	void cor(float r, float g, float b) {

		cor(r, g, b, cor_[3]);
	}

	void cor(float r, float g, float b, float a);

	void ambiente(bool sim, float alfa = 0);
};