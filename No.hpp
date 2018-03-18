#pragma once

template <typename Tipo> class No {

private:

	Tipo info_;
	No<Tipo> *ant_;
	No<Tipo> *prox_;

public:

	No(Tipo info, No<Tipo> *ant, No<Tipo> *prox);
	//No(const No<Tipo> &cpy);
	~No();

	No<Tipo> *getAnt();
	No<Tipo> *getProx();
	Tipo getInfo();
	Tipo& getInfoRef();
};