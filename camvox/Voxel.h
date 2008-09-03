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
private:
	uint32_t	value;
public:
	/** Create a dontPrune voxel.
	 */
	Voxel();

	/** Set voxel to be a layer.
	 * @param layers	Bit mask of layers.
	 */
	Voxel(uint8_t layers);

	/** Copy constructor.
	 * @param other		The voxel to copy.
	 */
	Voxel(const Voxel &other);

	/** Copy operator.
	 * @param other		The voxel to copy.
	 */
	Voxel &operator=(const Voxel &other);

	/** Unequal operator.
	 * @param other		The other voxel.
	 * @returns true if not equal.
	 */
	bool operator!=(const Voxel &other) const;

	/** Equal operator.
	 * @param other		The other voxel.
	 * @returns true if both voxels are equal.
	 */
	bool operator==(const Voxel &other) const;

	/** OR the layers of the other voxel to this one.
	 * @param other		The other voxel.
	 */
	void orLayers(const Voxel &other);

	/** AND the layers of the other voxel to this one.
	 * @param other		The other voxel.
	 */
	void andLayers(const Voxel &other);

	/** XOR the layers of the other voxel to this one.
	 * @param other		The other voxel.
	 */
	void xorLayers(const Voxel &other);

	/** Voxel points to a node.
	 * @param node_nr	Number of the node.
	 */
	void setNodeNr(uint32_t node_nr);

	/** Set the layers on this voxel.
	 * @param layers	Layer mask.
	 */
	void setLayers(uint8_t layers);

	/** Set the CSGObject which caused this voxel.
	 * @param obj		simplified CSGObject.
	 */
	void setCSGObject(const CSGObject *obj);

	/** Set the voxel to don't prune.
	 * This means that childs voxels are different from each other.
	 */
	void setDontPrune(void);

	/** Check if this voxel points to a child node.
	 * @returns true if this voxel points to a node.
	 */
	bool isNodeNr(void) const;

	/** Check if this voxel is a leaf node.
	 * @returns true if this voxel is a leaf node.
	 */
	bool isLayers(void) const;

	/** Check if the voxel node can be pruned.
	 * @returns true if this voxel can not be pruned.
	 */
	bool isDontPrune(void) const;

	/** Get the voxel value.
	 * @returns voxel value.
	 */
	uint32_t getValue(void) const;

	/** get the node nr.
	 * @returns node nr.
	 */
	uint32_t getNodeNr(void) const;

	/** Get the layer mask.
	 * @returns layer mask.
	 */
	uint8_t getLayers(void) const;

	/** Get the object that caused this leaf.
	 * @returns simplified CSGObject.
	 */
	CSGObject *getCSGObject(void) const;
};

inline Voxel::Voxel() :
	value(0)
{
}

inline Voxel::Voxel(uint8_t layers) :
	value(0)
{
	setLayers(layers);
}

inline Voxel::Voxel(const Voxel &other) :
	value(other.value)
{
}

inline Voxel &Voxel::operator=(const Voxel &other)
{
	value = other.value;
	return *this;
}

inline bool Voxel::operator!=(const Voxel &other) const
{
	return value != other.value;
}

inline bool Voxel::operator==(const Voxel &other) const
{
	return value == other.value;
}

inline void Voxel::orLayers(const Voxel &other)
{
	setLayers(getLayers() | other.getLayers());
}

inline void Voxel::andLayers(const Voxel &other)
{
	setLayers(getLayers() & other.getLayers());
}

inline void Voxel::xorLayers(const Voxel &other)
{
	setLayers(getLayers() ^ other.getLayers());
}

inline void Voxel::setNodeNr(uint32_t node_nr)
{
	if (__builtin_expect(node_nr > 0x7fffffff, 0)) {
		throw Voxel_err("Node number may not be larger than 0x7fffffff.");
	}
	value = node_nr | 0x80000000;
}

inline void Voxel::setLayers(uint8_t layers)
{
	if (__builtin_expect(isLayers(), 1)) {
		value = (value & 0xffffff00) | layers;
	} else {
		value = layers | 0x40000000;
	}
}

inline void Voxel::setCSGObject(const CSGObject *obj)
{
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

inline void Voxel::setDontPrune(void)
{
	value = 0;
}

inline bool Voxel::isNodeNr(void) const
{
	return (value & 0x80000000) == 0x80000000;
}

inline bool Voxel::isLayers(void) const
{
	return (value & 0xc0000000) == 0x40000000;
}

inline bool Voxel::isDontPrune(void) const
{
	return value == 0;
}

inline uint32_t Voxel::getValue(void) const
{
	return value;
}

inline uint32_t Voxel::getNodeNr(void) const
{
	if (__builtin_expect(!isNodeNr(), 0)) {
		throw Voxel_err("Voxel is not a node number.");
	}

	return value & 0x7fffffff;
}

inline uint8_t Voxel::getLayers(void) const
{
	if (__builtin_expect(!isLayers(), 0)) {
		throw Voxel_err("Voxel is not a layer.");
	}

	return value & 0x000000ff;
}

inline CSGObject *Voxel::getCSGObject(void) const
{
	if (__builtin_expect(!isLayers(), 0)) {
		throw Voxel_err("Voxel is not a layer.");
	}

	uint32_t id = (value & 0x3fffff00) >> 8;
	return csgobject_list[id];
}

}

#endif
