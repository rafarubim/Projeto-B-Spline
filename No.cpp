#pragma once

#include "No.hpp"

template <typename Tipo> No<Tipo>::No(Tipo info, No<Tipo> *ant, No<Tipo> *prox) {

	info_ = info;
	ant_ = ant;
	prox_ = prox;

	if (ant_ != NULL)
		ant_->prox_ = this;

	if (prox_ != NULL)
		prox_->ant_ = this;
}

/*template <typename Tipo> No::No(const No<Tipo> &cpy) {


}*/

template <typename Tipo> No<Tipo>::~No() {

	if (ant_ != NULL)
		ant_->prox_ = prox_;

	if (prox_ != NULL)
		prox_->ant_ = ant_;
}

template <typename Tipo> No<Tipo> *No<Tipo>::getAnt() {

	return ant_;
}

template <typename Tipo> No<Tipo> *No<Tipo>::getProx() {

	return prox_;
}

template <typename Tipo> Tipo No<Tipo>::getInfo() {

	return info_;
}

template <typename Tipo> Tipo& No<Tipo>::getInfoRef() {

	return info_;
}