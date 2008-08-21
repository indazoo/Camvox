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
        uint32_t id;
	Matrix transform;
	Matrix total_transform;
	Matrix total_inv_transform;
        double resolution;
        double total_resolution;

	CSGObject();
	~CSGObject();
	virtual void translate(const Vector &a);
	virtual void scale(const Vector &a);
	virtual void rotate(const Vector &a, double angle);
	virtual void setResolution(double _resolution);

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
	double size(double scale);
};


typedef enum {
        VOX_OP_OR = 0,
        VOX_OP_AND,
        VOX_OP_XOR,
        VOX_OP_TST,
        VOX_OP_NOP
} vox_op_t;

class VoxOperation {
public: 
        Voxel           inside_mask;
        Voxel           outside_mask;
        Voxel           edge_mask;
        vox_op_t        inside_op;
        vox_op_t        outside_op;
        vox_op_t        edge_op;
        bool            set_edge_obj;
        double          collision_size[32];
};

class Voxel {
public:
        uint32_t        value;

        Voxel();
        Voxel(uint32_t _value);
        Voxel(Voxel &other);

        bool operator!=(Voxel &other);

        bool isNodeNr(void);
        bool isLayers(void);
        bool isDontPrune(void);

        uint32_t getNodeNr(void);
        uint32_t getLayers(void);

        void setNodeNr(uint32_t node_nr);
        void setLayers(uint32_t layers);
};

class VoxTree {
public:
	int	nr_nodes_created;
	int	nr_nodes_destroyed;

	VoxTree(double _size);
	~VoxTree();
	void generatePOVCode();
	void addCSGObject(CSGObject *obj, VoxOperation &operation);
	void prune();
};

