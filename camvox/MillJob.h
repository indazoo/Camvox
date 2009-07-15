#ifndef CAMVOX_MILLJOB_H
#define CAMVOX_MILLJOB_H

#include "MillCoord.h"
#include "CSGObject.h"

namespace camvox {

/** An instance of this class defines a single layer of cutting.
 */
class MillJob {
public:
	CSGObject		*tool;		///< cutting part of the tool.
	CSGObject		*clamp;		///< non-cutting part of the tool.

	std::vector<MillCoord>	coords;		///< The voxels to be cut.

	/** Check for collisions.
	 * Set normal within tool limits.
	 * Retry with other normals.
	 * Or remove coord from list.
	 */
	void checkCollisions();

	/** Generate movements.
	 * Elliminate coords by creating movements.
	 */
	void
};



}

#endif
