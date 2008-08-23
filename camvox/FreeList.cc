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

#include <camvox/Exceptions.h>
#include <camvox/FreeList.h>

namespace camvox {

FreeList::FreeList(uint32_t _nr_items) : nr_items(_nr_items), regions()
{
	free_region_t	*a;

	a = new free_region_t;
	a->start = 0;
	a->length = nr_items;

	regions.push_front(a);
}

FreeList::~FreeList()
{
	// Delete all the regions, don't bother removing them from the list.
	std::list<free_region_t *>::iterator i;
	for (i = regions.begin(); i != regions.end(); ++i) {
		delete *i;
	}
}

uint32_t FreeList::alloc()
{
	free_region_t	*a;
	uint32_t	item_nr;

	if (regions.size() == 0) {
		throw new FreeList_err("No more items in the free list left.");
	}

	// Get the item from the start of the list.
	a = *(regions.begin());

	// Get the first item.
	item_nr = a->start;

	// Remove the item from the region.
	a->start++;
	a->length--;
	if (!a->length) {
		// Remove the region from the list.
		regions.pop_front();
		delete a;
	}

	// Return the item.
	return item_nr;
}

void FreeList::merge(std::list<free_region_t *>::iterator i)
{
	free_region_t	*a = *i;
	i++;
	free_region_t	*b = *i;

	// Check if the two regions are connected.
	if ((a->start + a->length) == b->start) {
		// Add the lengths together.
		a->length+= b->length;

		// Remove the second region.
		regions.erase(i);
		delete b;
	}
}

void FreeList::free(uint32_t item_nr)
{
	free_region_t	*a;

	// Find the region where the item will fit in.
	std::list<free_region_t *>::iterator i;
	for (i = regions.begin(); i != regions.end(); ++i) {
		a = *i;

		if (item_nr < (a->start - 1)) {
			// Our item is somewhere before this region.
			a = new free_region_t;
			a->start = item_nr;
			a->length = 1;

			// This region needs to be inserted before this.
			regions.insert(i, a);
			return;
		}

		if (item_nr == (a->start - 1)) {
			// Our item is imediatly in front of this region.
			a->start--;
			a->length++;

			// Only try to merge if this is not the first entry.
			if (i != regions.begin()) {
				i--;
				merge(i);
			}
			return;
		}

		if (item_nr < (a->start + a->length)) {
			// Our item is inside this region.
			throw FreeList_err("Item to be freed is already in free list.");
		}

		if (item_nr == (a->start + a->length)) {
			// Our item is imediatly after this region.
			a->length++;

			// Only try to merge if this is not the last entry.
			std::list<free_region_t *>::iterator j = i;
			j++;
			if (j != regions.end()) {
				merge(i);
			}
			return;
		}
	}

	// Create new region with only our item.
	a = new free_region_t;
	a->start = item_nr;
	a->length = 1;

	// This region needs to be appended after the last region.
	regions.push_back(a);
	return;
}

uint32_t FreeList::arraySize(void)
{
	// Check where the last region starts.
	std::list<free_region_t *>::iterator i = regions.end();
	i--;
	free_region_t *a = *i;

	// Return the last item number that was allocated
	return a->start;
}

}

