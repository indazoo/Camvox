/* FreeList.h - Management of regions of memory.
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
#ifndef FREELIST_H
#define FREELIST_H

#include <list>
#include <stdint.h>

namespace camvox {

typedef struct free_region_s free_region_t;

/** A region of free items.
 */
struct free_region_s {
	uint32_t	start;		///< The first item of this region.
	uint32_t	length;		///< The number of items in this region.
};

/** This class manages free items.
 * If one would have an array fixed sized items, one would like
 * to know which of these items are still free.
 *
 * With this class we have a list of all the items that are not
 * yet used by the application.
 */
class FreeList {
private:
	uint32_t			nr_items;	///< The number of items this class manages.
	std::list<free_region_t *>	regions;	///< A set of regions, see @see struct free_region_s.

	/** Merges two regions together, leaving one.
	 */
	void merge(std::list<free_region_t *>::iterator);

public:
	/** Construct a new free list.
	 * @param _nr_items maximum number of items to manage
	 */
	FreeList(uint32_t _nr_items);

	/** Destruct the free list.
	 */
	~FreeList();

	/** Get a free item.
	 * @returns a free item.
	 */
	uint32_t alloc();

	/** Give back an item.
	 * @param item_nr the item to bring back to the free list.
	 */
	void free(uint32_t item_nr);

	/** Find out how large the array is.
	 * This is used to figure out how large the item array needs
	 * to be as a mimimum.
	 * @returns The minimum array size to hold all items.
	 */
	uint32_t arraySize();
};

}

#endif
