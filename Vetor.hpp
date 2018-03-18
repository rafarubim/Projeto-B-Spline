#pragma once

template <typename Tipo> class Vetor {

private:

	Tipo x_, y_, z_, w_;

public:

	Vetor(Tipo x = 0, Tipo y = 0, Tipo z = 0, Tipo w = 0);
	Vetor(const Vetor<Tipo> &cpy);
	//~Vetor(); // Destrutor desnecessário

	void set(Tipo x, Tipo y) {

		set(x, y, z_);
	}
	void set(Tipo x, Tipo y, Tipo z) {

		set(x, y, z, w_);
	}
	void set(Tipo x, Tipo y, Tipo z, Tipo w);

	Tipo getX();
	Tipo getY();
	Tipo getZ();
	Tipo getW();
	double getNorm2D();
	double getNorm3D();

	Vetor<Tipo> normalize2D();
	Vetor<Tipo> normalize3D();

	Vetor<Tipo> operator+(const Vetor<Tipo> &v);
	Vetor<Tipo> operator-(const Vetor<Tipo> &v);
	void operator+=(const Vetor<Tipo> &v);
	void operator-=(const Vetor<Tipo> &v);
	void operator*=(double x);
	void operator/=(double x);
	/*friend Vetor<Tipo> operator*(const Vetor<Tipo> &v, double x);
	friend Vetor<Tipo> operator*(double x, const Vetor<Tipo> &v);
	friend Vetor<Tipo> operator/(const Vetor<Tipo> &v, double x);
	friend Vetor<Tipo> operator/(double x, const Vetor<Tipo> &v);*/
	Tipo& operator[](int n);

	void print(int i = 4);
};



template <typename Tipo> Vetor<Tipo> operator*(Vetor<Tipo> &v, double x);

template <typename Tipo> Vetor<Tipo> operator*(double x, Vetor<Tipo> &v);

template <typename Tipo> Vetor<Tipo> operator/(Vetor<Tipo> &v, double x);

template <typename Tipo> Vetor<Tipo> operator/(double x, Vetor<Tipo> &v);

template <typename Tipo> Vetor<Tipo> vecProd3D(Vetor<Tipo> &v1, Vetor<Tipo> &v2);

template <typename Tipo> Tipo dotProd3D(Vetor<Tipo> &v1, Vetor<Tipo> &v2);

template <typename Tipo> double getCosBetween3D(Vetor<Tipo> &v1, Vetor<Tipo> &v2);

template <typename Tipo> double getAngleBetween3D(Vetor<Tipo> &v1, Vetor<Tipo> &v2);

template <typename Tipo> double getCosBetween2D(Vetor<Tipo> &v1, Vetor<Tipo> &v2);

template <typename Tipo> double getAngleBetween2D(Vetor<Tipo> &v1, Vetor<Tipo> &v2);

template <typename Tipo> bool isInLimits2D(Vetor<Tipo> &vec2D, Vetor<Tipo> &limits4D);