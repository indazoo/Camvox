/* VoxTree.cc - Management of all octtree voxel nodes.
 * Copyright (C) 2008  Take Vos
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <fenv.h>
#include <camvox/VoxTree.h>

namespace camvox {

void VoxTree::generatePOVCodeForVoxel(uint32_t node_nr, const VoxCoord &coord, int voxel_index)
{
	VoxCoord	voxel_coord = coord.childCoord(voxel_index);
	Voxel	 	voxel_data = nodes[node_nr].voxels[voxel_index];

	if (voxel_data.isNodeNr()) {
		// There already exists a child object. Walk the childtree.
		generatePOVCodeForNode(voxel_data.getNodeNr(), voxel_coord);

	} else {
		if (voxel_data.getLayers() & 1) {
			IntervalVector voxel_bound = voxel_coord.boundingBox(scale);

			// Our Y and Z axis is switched compared to almost everyone else.
			fprintf(stdout, "box {\n");
			fprintf(stdout, "  <%lf, %lf, %lf>, <%lf, %lf, %lf>\n",
				voxel_bound.x.low,
				voxel_bound.z.low,
				voxel_bound.y.low,
				voxel_bound.x.high,
				voxel_bound.z.high,
				voxel_bound.y.high
			);
			fprintf(stdout, "}\n");
		}
	}
}

void VoxTree::generatePOVCodeForNode(uint32_t node_nr, const VoxCoord &coord)
{
	for (int voxel_index = 0; voxel_index < 8; voxel_index++) {
		generatePOVCodeForVoxel(node_nr, coord, voxel_index);
	}
}

void VoxTree::generatePOVCode(void)
{
	fprintf(stdout, "#include \"colors.inc\"\n\n");
	fprintf(stdout, "#include \"glass.inc\"\n\n");

	fprintf(stdout, "background { color Black }\n");
	fprintf(stdout, "global_settings { ambient_light <1.2, 1.2, 1.2> }\n");
	fprintf(stdout, "camera {\n");
	fprintf(stdout, "  orthographic\n");
	fprintf(stdout, "  location <%lf, %lf, %lf>\n", -size * 0.4, size * 0.4, -size * 0.8);
	fprintf(stdout, "  look_at <0.0, 0.0, 0.0>\n");
	fprintf(stdout, "}\n");
	fprintf(stdout, "light_source { <%lf, %lf, %lf> color <%lf, %lf, %lf> }\n",
		-size * 0.5, size * 0.4, -size,
		2.0, 2.0, 2.0
	);


	fprintf(stdout, "cylinder {\n");
	fprintf(stdout, " <0, 0, 0>, <%lf, 0, 0>, 0.1\n", size);
	fprintf(stdout, "  texture {\n");
	fprintf(stdout, "    pigment { color Red }\n");
	fprintf(stdout, "  }\n");
	fprintf(stdout, "}\n");

	fprintf(stdout, "cylinder {\n");
	fprintf(stdout, " <0, 0, 0>, <0.0, 0.0, %lf>, 0.1\n", size);
	fprintf(stdout, "  texture {\n");
	fprintf(stdout, "    pigment { color Green }\n");
	fprintf(stdout, "  }\n");
	fprintf(stdout, "}\n");

	fprintf(stdout, "cylinder {\n");
	fprintf(stdout, " <0, 0, 0>, <0.0, %lf, 0.0>, 0.1\n", size);
	fprintf(stdout, "  texture {\n");
	fprintf(stdout, "    pigment { color Blue }\n");
	fprintf(stdout, "  }\n");
	fprintf(stdout, "}\n");


	fprintf(stdout, "union {\n");

	generatePOVCodeForNode(root, VoxCoord());

	fprintf(stdout, "  texture {\n");
	fprintf(stdout, "    pigment { color Green }\n");
	fprintf(stdout, "  }\n");
	//fprintf(stdout, "  interior {I_Glass_Dispersion1}\n");
	fprintf(stdout, "}\n");
}

}
