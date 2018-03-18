#pragma once

#define M_PI 3.14159265358979323846
#include "Vetor.hpp"

#include "iostream"
#include "cmath"

using namespace std;

enum XYZW {X = 0, Y, Z, W};

template <typename Tipo> Vetor<Tipo>::Vetor(Tipo x, Tipo y, Tipo z, Tipo w) {

	x_ = x;
	y_ = y;
	z_ = z;
	w_ = w;
}

template <typename Tipo> Vetor<Tipo>::Vetor(const Vetor<Tipo> &cpy) {

	x_ = cpy.x_;
	y_ = cpy.y_;
	z_ = cpy.z_;
	w_ = cpy.w_;
}

template <typename Tipo> void Vetor<Tipo>::set(Tipo x, Tipo y, Tipo z, Tipo w) {

	x_ = x;
	y_ = y;
	z_ = z;
	w_ = w;
}

template <typename Tipo> Tipo Vetor<Tipo>::getX() {

	return x_;
}

template <typename Tipo> Tipo Vetor<Tipo>::getY() {

	return y_;
}

template <typename Tipo> Tipo Vetor<Tipo>::getZ() {

	return z_;
}

template <typename Tipo> Tipo Vetor<Tipo>::getW() {

	return w_;
}

template <typename Tipo> double Vetor<Tipo>::getNorm2D() {

	return sqrt(x_ * x_ + y_ * y_);
}

template <typename Tipo> double Vetor<Tipo>::getNorm3D() {

	return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

template <typename Tipo> Vetor<Tipo> Vetor<Tipo>::normalize2D() {

	Tipo norm = getNorm3D();
	if (norm == 0) {
		//cout << "Erro: tentativa de normalizacao de vetor nulo.\n";
		norm = 1;
	}
	x_ /= norm;
	y_ /= norm;
	return *this;
}

template <typename Tipo> Vetor<Tipo> Vetor<Tipo>::normalize3D() {

	Tipo norm = getNorm3D();
	if (norm == 0) {
		//cout << "Erro: tentativa de normalizacao de vetor nulo.\n";
		norm = 1;
	}
	x_ /= norm;
	y_ /= norm;
	z_ /= norm;
	return *this;
}

template <typename Tipo> Vetor<Tipo> Vetor<Tipo>::operator+(const Vetor &v) {

	return Vetor<Tipo>(x_ + v.x_, y_ + v.y_, z_ + v.z_, (w_ || v.w_));
}

template <typename Tipo> Vetor<Tipo> Vetor<Tipo>::operator-(const Vetor &v) {

	return Vetor<Tipo>(x_ - v.x_, y_ - v.y_, z_ - v.z_, (w_ || v.w_));
}

template <typename Tipo> void Vetor<Tipo>::operator+=(const Vetor &v) {

	x_ += v.x_;
	y_ += v.y_;
	z_ += v.z_;
	w_ = (w_ || v.w_);
}

template <typename Tipo> void Vetor<Tipo>::operator-=(const Vetor &v) {

	x_ -= v.x_;
	y_ -= v.y_;
	z_ -= v.z_;
	w_ = (w_ || v.w_);
}

template <typename Tipo> void Vetor<Tipo>::operator*=(double x) {

	x_ *= x;
	y_ *= x;
	z_ *= x;
	w_ *= x;
}

template <typename Tipo> void Vetor<Tipo>::operator/=(double x) {

	x_ /= x;
	y_ /= x;
	z_ /= x;
	w_ /= x;
}

template <typename Tipo> Vetor<Tipo> operator*(Vetor<Tipo> &v, double x) {

	return Vetor<Tipo>(v[X] * x, v[Y] * x, v[Z] * x, v[W] * x);
}

template <typename Tipo> Vetor<Tipo> operator*(double x, Vetor<Tipo> &v) {

	return Vetor<Tipo>(v[X] * x, v[Y] * x, v[Z] * x, v[W] * x);
}

template <typename Tipo> Vetor<Tipo> operator/(Vetor<Tipo> &v, double x) {

	return Vetor<Tipo>(v[X] / x, v[Y] / x, v[Z] / x, v[W] / x);
}

template <typename Tipo> Vetor<Tipo> operator/(double x, Vetor<Tipo> &v) {

	return Vetor<Tipo>(v[X] / x, v[Y] / x, v[Z] / x, v[W] / x);
}

template <typename Tipo> Tipo& Vetor<Tipo>::operator[](int n) {

	if (n != 0 && n != 1 && n != 2 && n != 3) {

		cout << "Erro: indice passado ao vetor e invalido! A quarta coordenada foi acessada.\n\n";
		return z_;
	}

	if (n == X)
		return x_;
	else if (n == Y)
		return y_;
	else if (n == W)
		return w_;
	else
		return z_;
}

template <typename Tipo> Vetor<Tipo> vecProd3D(Vetor<Tipo> &v1, Vetor<Tipo> &v2) {

	return Vetor<Tipo>(v1[Y] * v2[Z] - v1[Z] * v2[Y], v1[Z] * v2[X] - v1[X] * v2[Z], v1[X] * v2[Y] - v1[Y] * v2[X]);
}

template <typename Tipo> Tipo dotProd3D(Vetor<Tipo> &v1, Vetor<Tipo> &v2) {

	return v1[X] * v2[X] + v1[Y] * v2[Y] + v1[Z] * v2[Z];
}

template <typename Tipo> double getCosBetween3D(Vetor<Tipo> &v1, Vetor<Tipo> &v2) {

	double x = dotProd3D(v1, v2) / v1.getNorm3D() / v2.getNorm3D();

	if (x > 1)
		x = 1;
	else if (x < -1)
		x = -1;

	return x;
}

template <typename Tipo> double getAngleBetween3D(Vetor<Tipo> &v1, Vetor<Tipo> &v2) {

	return acos(getCosBetween3D(v1, v2)) * 180 / M_PI;
}

template <typename Tipo> double getCosBetween2D(Vetor<Tipo> &v1, Vetor<Tipo> &v2) {

	double x = dotProd2D(v1, v2) / v1.getNorm2D() / v2.getNorm2D();

	if (x > 1)
		x = 1;
	else if (x < -1)
		x = -1;

	return x;
}

template <typename Tipo> double getAngleBetween2D(Vetor<Tipo> &v1, Vetor<Tipo> &v2) {

	return acos(getCosBetween2D(v1, v2)) * 180 / M_PI;
}

template <typename Tipo> void Vetor<Tipo>::print(int i) {

	cout << "(";

	if (i > 0)
		cout << x_;
	i--;
	if (i > 0)
		cout << ", " << y_;
	i--;
	if (i > 0)
		cout << ", " << z_;
	i--;
	if (i > 0)
		cout << ", " << w_;

	cout << ")" << endl;
}

template <typename Tipo> bool isInLimits2D(Vetor<Tipo> &vec2D, Vetor<Tipo> &limits4D) {
	
	return (vec2D[X] > limits4D[0] && vec2D[X] < limits4D[1] && vec2D[Y] > limits4D[2] && vec2D[Y] < limits4D[3]);
}