#pragma once

class Iluminacao {

private:

	bool luzes_[8];

public:

	Iluminacao();

	int getNextLight();

	void ligar();
	void desligar();

	void obsLocal(bool sim);
	void doisLados(bool sim);
	void suave(bool sim);
	void brilhoPadrao(bool sim);

	void colorir();

	void newLight(int light);
	void lightDestroyed(int light);
};