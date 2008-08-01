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
#include <camvox/VoxTree.h>

namespace camvox {

VoxTree::VoxTree(void)
{
	max_depth = 3;
	nr_nodes_created = 1;
	nr_nodes_destroyed = 0;
	root = new VoxNode(0);
}

void VoxTree::generatePOVCode(void)
{
	fprintf(stdout, "#include \"colors.inc\"\n\n");
	fprintf(stdout, "#include \"glass.inc\"\n\n");

	fprintf(stdout, "background { color Black }\n");
	fprintf(stdout, "global_settings { ambient_light <0.4, 0.4, 0.4> }\n");
	fprintf(stdout, "camera {\n");
	fprintf(stdout, "  location <2.0, 2.0, 2.0>\n");
	fprintf(stdout, "  look_at <0.5, 0.5, 0.5>\n");
	fprintf(stdout, "}\n");
	fprintf(stdout, "light_source { <6, 3.0, 2.0> color <3.0, 3.0, 3.0> }\n");

	fprintf(stdout, "union {\n");
	root->generatePOVCode(*this, VoxCoord());
	fprintf(stdout, "  texture {\n");
	fprintf(stdout, "    pigment { color Blue }\n");
	fprintf(stdout, "  }\n");
	//fprintf(stdout, "  interior {I_Glass_Dispersion1}\n");
	fprintf(stdout, "}\n");
}

long VoxTree::addCSGObject(const CSGObject &obj, int layer)
{
	return root->addCSGObject(*this, VoxCoord(), obj, layer);
}

long VoxTree::prune(void)
{
	return root->prune(*this, VoxCoord());
}

}
