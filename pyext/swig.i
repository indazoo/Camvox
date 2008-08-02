%module camvox
%{
#include <camvox/camvox.h>
using namespace camvox;
%}

class Interval {
public:
	Interval(double low, double high);
	~Interval();
	double lower();
	double upper();
};

class Vector {
public:
	Vector(double x, double y, double z);
	Vector(double x, double y, double z, double w);
	~Vector();
};

class Matrix {
public:
	Matrix();
	~Matrix();
};

class CSGObject {
public:
	CSGObject();
	~CSGObject();
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
};

class CSGUnion : public CSGOperation {
public:
	CSGUnion();
	~CSGUnion();
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

