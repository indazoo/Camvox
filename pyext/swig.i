%module camvox
%{
#include <camvox/camvox.h>
using namespace camvox;
%}

typedef unsigned int uint32_t;

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
	virtual void rotate(const Vector &a, double angle);

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

class CSGCylinder : public CSGPrimative {
public:
	CSGCylinder(double diameter);
	~CSGCylinder();
};

class CSGPlane : public CSGPrimative {
public:
	CSGPlane();
	~CSGPlane();
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

class CSGIntersection : public CSGOperation {
public:
	CSGIntersection();
	~CSGIntersection();
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

typedef uint32_t voxel_t;

typedef enum {
        VOX_OP_OR = 0,
        VOX_OP_AND,
        VOX_OP_XOR,
        VOX_OP_TST
} vox_op_t;

bool voxIsNodeNr(voxel_t data);
uint32_t voxGetNodeNr(voxel_t data);
voxel_t voxSetNodeNr(uint32_t node_nr);
voxel_t voxSetDontPrune(void);
bool voxIsDontPrune(voxel_t data);
vox_op_t voxGetOperation(voxel_t data);
uint32_t voxGetLayers(voxel_t data);
voxel_t voxSetLayersAndOperation(uint32_t mask, vox_op_t op);

class VoxTree {
public:
	int	max_depth;
	int	nr_nodes_created;
	int	nr_nodes_destroyed;
	double	scale;

	VoxTree();
	~VoxTree();
	void generatePOVCode();
	void addCSGObject(CSGObject *obj, voxel_t *new_data);
	void addCSGObjectOR(const CSGObject *obj, uint32_t layers);
	void addCSGObjectAND(const CSGObject *obj, uint32_t layers);
	void addCSGObjectXOR(const CSGObject *obj, uint32_t layers);
	uint32_t addCSGObjectTST(const CSGObject *obj);
	void prune();
};

