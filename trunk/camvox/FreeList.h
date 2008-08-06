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

#include <vector>
#include <stdint.h>

namespace camvox {

typedef struct free_region_s free_region_t;

struct free_region_s {
	uint32_t	start;
	uint32_t	length;
};

class FreeList {
protected:
	uint32_t			nr_items;
	std::vector<free_region_s *>	regions;

	void merge(std::vector<free_region_t *>::iterator);

public:
	FreeList(uint32_t _nr_items);
	~FreeList();

	uint32_t alloc();
	void free(uint32_t item_nr);
	uint32_t maxItemNr();
};

}

#endif
