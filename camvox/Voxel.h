/* Voxel.h - A voxel.
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
#ifndef VOXEL_H
#define VOXEL_H

#include <stdint.h>
#include <camvox/CSGObject.h>

namespace camvox {

/** Value of a voxel.
 * 00000000 00000000 00000000 00000000		Al child voxels are the same.
 * 1nnnnnnn nnnnnnnn nnnnnnnn nnnnnnnn		Node number.
 * 01cccccc cccccccc cccccccc llllllll		CSGobject + layer mask.
 */
class Voxel {
public:
	uint32_t	value;

	Voxel() {
		value = 0;
	}

	Voxel(uint32_t _value) {
		value = _value;
	}

	Voxel(const Voxel &other) {
		value = other.value;
	}

	Voxel &operator=(const Voxel &other) {
		value = other.value;
		return *this;
	}

	bool operator!=(const Voxel &other) const {
		return value != other.value;
	}

	void orLayers(const Voxel &other) {
		value|= other.value;
	}

	void andLayers(const Voxel &other) {
		value&= other.value;
	}

	void xorLayers(const Voxel &other) {
		value^= other.value;
	}

	void setNodeNr(uint32_t node_nr) {
		value = node_nr | 0x80000000;
	}

	void setLayers(uint32_t layers) {
		value = (layers & 0x3fffffff) | 0x40000000;
	}

	void setCSGObject(const CSGObject *obj) {
		uint32_t id = obj->id;

		value = (value & 0xffffff) | ((id << 8) & 0x3fffff00);
	}

	void setDontPrune(void) {
		value = 0;
	}

	bool isNodeNr(void) const {
		return (value & 0x80000000) == 0x80000000;
	}

	bool isLayers(void) const {
		return (value & 0xc0000000) == 0x40000000;
	}

	bool isDontPrune(void) const {
		return value == 0;
	}

	uint32_t getValue(void) const {
		return value;
	}

	uint32_t getNodeNr(void) const {
		return value & 0x7fffffff;
	}

	uint32_t getLayers(void) const {
		return value & 0x3fffffff;
	}

	CSGObject *getCSGObject(void) const {
		uint32_t id = (value & 0x3fffff00) >> 8;
		return csgobject_list[id];
	}
};

}

#endif
