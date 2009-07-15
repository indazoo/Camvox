#ifndef CAMVOX_MILLCOORD_H
#define CAMVOX_MILLCOORD_H

#include <camvox/VoxCoord.h>
#include <camvox/Vector.h>

namespace camvox {

/** This specifies the 6DOF coord on the material.
 */
class MillCoord {
public:
	VoxCoord	voxel;		///< This voxel is the center of the tool.
	Vector		position;	///< The position of the tool in cartesion coord
	Vector		orientation;	///< The orientation of the tool in cartesion coord.

	MillCoord();
	MillCoord(const VoxCoord &_voxel, const Vector &_position, const Vector &_orientation);
};

inline MillCoord::MillCoord() :
	voxel(), position(), orientation()
{
}

inline MillCoord::MillCoord(const VoxCoord &_voxel, const Vector &_position, const Vector &_orientation) :
	voxel(_voxel), position(_position), orientation(_orientation)
{
}

}
#endif
