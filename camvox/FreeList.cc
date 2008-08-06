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

#include <camvox/FreeList.h>

namespace camvox {

FreeList::FreeList(uint32_t _nr_items) : nr_items(_nr_items), regions()
{
	free_region_t	*a;

	a = new free_region_t;
	a->start = 0;
	a->length = nr_items;

	regions.push_back(a);
}

FreeList::~FreeList()
{
	// Delete all the regions, don't bother removing them from the list.
	for (unsigned int i = 0; i < regions.size(); i++) {
		delete regions[i];
	}
}

uint32_t FreeList::alloc()
{
	free_region_t	*a;
	uint32_t	item_nr;

	// Get the item from the back of the list.
	a = regions.back();
	item_nr = a->start + (a->length - 1);

	// Remove the item from the region.
	a->length--;
	if (!a->length) {
		// Remove the region from the list.
		regions.pop_back();
		delete a;
	}

	// Return the item in reverse.
	return (nr_items - 1) - item_nr;
}

void FreeList::merge(std::vector<free_region_t *>::iterator i)
{
	free_region_t	*a = *i;
	free_region_t	*b = *(i + 1);

	// Check if the two regions are connected.
	if ((a->start + a->length) == b->start) {
		// Add the lengths together.
		a->length+= b->length;

		// Remove the second region.
		regions.erase(i + 1);
		delete b;
	}
}

void FreeList::free(uint32_t item_nr)
{
	free_region_t	*a;

	// Reverse the item.
	item_nr = (nr_items - 1) - item_nr;

	// Find the region where the item will fit in.
	std::vector<free_region_t *>::reverse_iterator i;
	for (i = regions.rbegin(); i != regions.rend(); ++i) {
		a = *i;

		if (item_nr == (a->start - 1)) {
			// Our item is exactly before this region.
			// Return the item.
			a->start--;
			a->length++;

			// Only merge if it is not the 0th entry.
			if ((i + 1) != regions.rend()) {
				merge(i.base() - 1);
			}
			return;
		}

		if (item_nr < a->start) {
			// Our item need to be found earlier.
			continue;
		}

		if (item_nr == (a->start + a->length)) {
			// Our item is exactly behind this region.
			// Return the item.
			a->length++;

			// Only merge if it is not the nth entry.
			if (i != regions.rbegin()) {
				merge(i.base());
			}
			return;
		}

		// Create a new region and insert it after the current region.
		a = new free_region_t;
		a->start = item_nr;
		a->length = 1;
		regions.insert(i.base() + 1, a);
		return;
	}
}

uint32_t FreeList::maxItemNr(void)
{
	// Check how much is removed from the first region.
	free_region_t *a = regions[0];

	// Return the number of items that should be on the right side of this region.
	return nr_items - a->length;
}

}

