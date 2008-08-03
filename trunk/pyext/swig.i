%module camvox
%{
#include <camvox/camvox.h>
using namespace camvox;
%}

class Interval {
public:
	double low, high;
	Interval(double low, double high);
	Interval(double value);
	~Interval();
};

class Vector {
public:
	Vector();
	Vector(double x, double y, double z);
	Vector(double x, double y, double z, double w);
	~Vector();
};

class Matrix {
public:
	Matrix();
	~Matrix();

	%extend {
		char *__str__() {
			static char temp[256];
			sprintf(temp, "[[%.02lf, %.02lf, %.02lf, %.02lf], [%.02lf, %.02lf, %.02lf, %.02lf], [%.02lf, %.02lf, %.02lf, %.02lf], [%.02lf, %.02lf, %.02lf, %.02lf]]",
				$self->m[0][0], $self->m[0][1], $self->m[0][2], $self->m[0][3],
				$self->m[1][0], $self->m[1][1], $self->m[1][2], $self->m[1][3],
				$self->m[2][0], $self->m[2][1], $self->m[2][2], $self->m[2][3],
				$self->m[3][0], $self->m[3][1], $self->m[3][2], $self->m[3][3]
			);
			return &temp[0];
				
		}
	}
};

class CSGObject {
public:
	Matrix transform;
	Matrix total_transform;
	Matrix total_inv_transform;

	CSGObject();
	~CSGObject();
	virtual void translate(const Vector &a);
	virtual void scale(const Vector &a);

};

class CSGPrimative : public CSGObject {
public:
	CSGPrimative();
	~CSGPrimative();
};

class CSGSphere : public CSGPrimative {
public:
	CSGSphere(double diameter);
	~CSGSphere();
};

class CSGOperation : public CSGObject {
public:
	CSGOperation();
	~CSGOperation();
	virtual void add(CSGObject *obj);
};

class CSGUnion : public CSGOperation {
public:
	CSGUnion();
	~CSGUnion();
};

class CSGDifference : public CSGOperation {
public:
	CSGDifference();
	~CSGDifference();
};

class CSGInvert : public CSGOperation {
public:
	CSGInvert();
	~CSGInvert();
};

class VoxCoord {
public:
	uint32_t	v[3];
	int		depth;

	VoxCoord();
	~VoxCoord();
	VoxCoord nextNeighbour(int x, int y, int z);
	VoxCoord childCoord(int child_index);
	IntervalVector boundingBox(double scale);
};

class VoxTree {
public:
	int	max_depth;
	int	nr_nodes_created;
	int	nr_nodes_destroyed;
	double	scale;

	VoxTree();
	~VoxTree();
	void generatePOVCode();
	long addCSGObject(CSGObject &obj, int layer);
	long prune();
};

class VoxNode {
public:
	VoxNode(long data);
	~VoxNode();
	long prune(VoxTree &tree, VoxCoord &coord);
	long addCSGObject(VoxTree &tree, VoxCoord &coord, CSGObject &obj, int layer);
	void generatePOVCode(VoxTree &tree, VoxCoord &coord);
};

