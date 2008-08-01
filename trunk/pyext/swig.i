%module camvox
%{
#include <camvox/camvox.h>
using namespace camvox;
%}

class Interval {
public:
	Interval(double low, double high);
	~Interval();
};

class Vector {
public:
	Vector(double x, double y, double z);
	Vector(double x, double y, double z, double w);
	~Vector();
	%extend {
		double x_get() {
			return (*self)[0];
		}
		double x_set(double value) {
			(*self)[0] = value;
			return value;
		}
		double y_get() {
			return (*self)[1];
		}
		double y_set(double value) {
			(*self)[1] = value;
			return value;
		}
		double z_get() {
			return (*self)[2];
		}
		double z_set(double value) {
			(*self)[2] = value;
			return value;
		}
		double w_get() {
			return (*self)[3];
		}
		double w_set(double value) {
			(*self)[3] = value;
			return value;
		}
	}
};

class Matrix {
public:
	Matrix(void);
	~Matrix();
};


