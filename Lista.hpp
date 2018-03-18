#pragma once

#include "No.hpp"
#include "No.cpp"

#include "iostream"
using namespace std;

template <typename Tipo> class Lista {

private:

	No<Tipo> *prim_;

public:

	Lista();
	Lista(const Lista &cpy);
	~Lista();

	void operator=(const Lista &cpy);

	bool vazia();
	int size();

	void push(Tipo novo);
	void insere(int index, Tipo novo);
	void retira(int index);

	Tipo& operator[](int index);

	No<Tipo> *testeLst(int index);
};