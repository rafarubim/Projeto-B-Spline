#pragma once

#include "Lista.hpp"

#include "iostream"
using namespace std;

template <typename Tipo> Lista<Tipo>::Lista() {

	prim_ = NULL;
}

template <typename Tipo> Lista<Tipo>::Lista(const Lista<Tipo> &cpy) {

	No<Tipo> *temp = cpy.prim_;

	if (temp == NULL)
		prim_ = NULL;
	else {
		prim_ = new No<Tipo>(temp->getInfo(), NULL, NULL);
		temp = temp->getProx();
	}

	for(No<Tipo> *temp2 = prim_; temp != NULL; temp = temp->getProx())
		temp2 = new No<Tipo>(temp->getInfo(), temp2, NULL);
}

template <typename Tipo> Lista<Tipo>::~Lista() {

	if (prim_ != NULL) {

		for(No<Tipo> *temp = prim_->getProx(); temp != NULL; temp = prim_->getProx()) // deletar o segundo nó automaticamente joga o terceiro nó para a posição de segundo (como definido no destrutor de nó)
			delete temp;

		delete prim_;
		prim_ = NULL;
	}
}

template <typename Tipo> void Lista<Tipo>::operator=(const Lista &cpy) {

	if (prim_ != NULL) {

		for(No<Tipo> *temp = prim_->getProx(); temp != NULL; temp = prim_->getProx()) // deletar o segundo nó automaticamente joga o terceiro nó para a posição de segundo (como definido no destrutor de nó)
			delete temp;

		delete prim_;
		prim_ = NULL;
	}

	No<Tipo> *temp = cpy.prim_;

	if (temp == NULL)
		prim_ = NULL;
	else {
		prim_ = new No<Tipo>(temp->getInfo(), NULL, NULL);
		temp = temp->getProx();
	}

	for(No<Tipo> *temp2 = prim_; temp != NULL; temp = temp->getProx())
		temp2 = new No<Tipo>(temp->getInfo(), temp2, NULL);
}

template <typename Tipo> bool Lista<Tipo>::vazia() {

	return (prim_ == NULL);
}

template <typename Tipo> int Lista<Tipo>::size() {

	int count = 0;

	for(No<Tipo> *temp = prim_; temp != NULL; temp = temp->getProx())
		count++;
	
	return count;
}

template <typename Tipo> void Lista<Tipo>::push(Tipo novo) {

	No<Tipo> *temp = prim_;

	for(; temp != NULL && temp->getProx() != NULL; temp = temp->getProx());

	if (temp == NULL)
		prim_ = new No<Tipo>(novo, NULL, NULL);
	else
		new No<Tipo>(novo, temp, NULL);
}

template <typename Tipo> void Lista<Tipo>::insere(int index, Tipo novo) {

	if (index < 0 || index > size())
		cout << "Aviso: tentativa de insercao em indice invalido da lista encadeada. A insercao ocorrera na ultima posicao\n";

	No<Tipo> *temp = prim_;

	int i = 0;

	for(; index != i && temp != NULL && temp->getProx() != NULL; temp = temp->getProx(), i++);

	if (temp == NULL)
		prim_ = new No<Tipo>(novo, NULL, NULL);
	else if (index == 0)
		prim_ = new No<Tipo>(novo, NULL, temp);
	else if (i == index)
		new No<Tipo>(novo, temp->getAnt(), temp);
	else
		new No<Tipo>(novo, temp, NULL);
}

template <typename Tipo> void Lista<Tipo>::retira(int index) {

	if (index < 0 || index >= size())
		cout << "Aviso: tentativa de retiragem em indice invalido da lista encadeada. A retiragem ocorrera na ultima posicao\n";

	No<Tipo> *temp = prim_;

	int i = 0;

	for(; index != i && temp != NULL && temp->getProx() != NULL; temp = temp->getProx(), i++);

	if (temp == NULL)
		return;
	else if (index == 0) {
		prim_ = prim_->getProx();
		delete temp;
	} else
		delete temp;
}

template <typename Tipo> Tipo& Lista<Tipo>::operator[](int index) {

	if (index < 0 || index >= size()) {
		cout << "Erro: tentativa de acesso de indice invalido em lista encadeada\n";

		if (prim_ == NULL) {
			cout << "A lista esta vazia, portanto nao ha como continuar o programa...\n\n";
			exit(1);
		} else {
			cout << "O ultimo elemento da lista foi acessado\n";
		}
	}

	No<Tipo> *temp = prim_;

	for(int i = 0; index != i && temp->getProx() != NULL; temp = temp->getProx(), i++);

	return temp->getInfoRef();
}

template <typename Tipo> No<Tipo> *Lista<Tipo>::testeLst(int index) {

	if (index < 0 || index >= size()) {
		cout << "Erro: tentativa de acesso de indice invalido em No de lista encadeada\n";

		if (prim_ == NULL) {
			cout << "A lista esta vazia, portanto nao ha como continuar o programa...\n\n";
			exit(1);
		} else {
			cout << "O ultimo No da lista foi acessado\n";
		}
	}

	No<Tipo> *temp = prim_;

	for(int i = 0; index != i && temp->getProx() != NULL; temp = temp->getProx(), i++);

	return temp;
}