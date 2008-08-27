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
#include <exception>
#include <camvox/CSGObject.h>

namespace camvox {

class Voxel_err : public std::exception {
	const char *msg;
public:
	Voxel_err(const char *_msg) {
		msg = _msg;
	}

	virtual ~Voxel_err() throw () {
	}

	virtual const char* what() const throw () {
		return msg;
	}
};

/** Value of a voxel.
 * 00000000 00000000 00000000 00000000		Al child voxels are the same.
 * 1nnnnnnn nnnnnnnn nnnnnnnn nnnnnnnn		Node number.
 * 01cccccc cccccccc cccccccc llllllll		CSGobject + layer mask.
 */
class Voxel {
public:
	uint32_t	value;

	Voxel() : value(0) {}

	Voxel(uint8_t layers) : value(0) {
		setLayers(layers);
	}

	Voxel(const Voxel &other) : value(other.value) {}

	Voxel &operator=(const Voxel &other) {
		value = other.value;
		return *this;
	}

	bool operator!=(const Voxel &other) const {
		return value != other.value;
	}

	bool operator==(const Voxel &other) const {
		return value == other.value;
	}

	void orLayers(const Voxel &other) {
		setLayers(getLayers() | other.getLayers());
	}

	void andLayers(const Voxel &other) {
		setLayers(getLayers() & other.getLayers());
	}

	void xorLayers(const Voxel &other) {
		setLayers(getLayers() ^ other.getLayers());
	}

	void setNodeNr(uint32_t node_nr) {
		if (__builtin_expect(node_nr > 0x7fffffff, 0)) {
			throw Voxel_err("Node number may not be larger than 0x7fffffff.");
		}
		value = node_nr | 0x80000000;
	}

	void setLayers(uint8_t layers) {
		if (__builtin_expect(isLayers(), 1)) {
			value = (value & 0xffffff00) | layers;
		} else {
			value = layers | 0x40000000;
		}
	}

	void setCSGObject(const CSGObject *obj) {
		uint32_t id = obj->id;

		if (__builtin_expect(id > 0x3fffff, 0)) {
			throw Voxel_err("CSGObject is may not be larger than 0x3fffff.");
		}

		if (__builtin_expect(isLayers(), 1)) {
			value = (value & 0xc00000ff) | (id << 8);
		} else {
			value = (id << 8) | 0x40000000;
		}
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
		if (__builtin_expect(!isNodeNr(), 0)) {
			throw Voxel_err("Voxel is not a node number.");
		}

		return value & 0x7fffffff;
	}

	uint8_t getLayers(void) const {
		if (__builtin_expect(!isLayers(), 0)) {
			throw Voxel_err("Voxel is not a layer.");
		}

		return value & 0x000000ff;
	}

	CSGObject *getCSGObject(void) const {
		if (__builtin_expect(!isLayers(), 0)) {
			throw Voxel_err("Voxel is not a layer.");
		}

		uint32_t id = (value & 0x3fffff00) >> 8;
		return csgobject_list[id];
	}
};

}

#endif
