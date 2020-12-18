// SPDX-License-Identifier: GPL-2.0

#ifndef __TI_PRUSS_H
#define __TI_PRUSS_H

/**
 * enum pruss_mem - PRUSS memory range identifiers
 */
enum pruss_mem {
	PRUSS_MEM_DRAM0 = 0,
	PRUSS_MEM_DRAM1,
	PRUSS_MEM_SHRD_RAM2,
	PRUSS_MEM_MAX,
};

/**
 * struct pruss_mem_region - PRUSS memory region structure
 * @va: kernel virtual address of the PRUSS memory region
 * @pa: physical (bus) address of the PRUSS memory region
 * @size: size of the PRUSS memory region
 */
struct pruss_mem_region {
	void __iomem *va;
	phys_addr_t pa;
	size_t size;
};

/**
 * struct pruss - PRUSS parent structure
 * @dev: pruss device pointer
 * @mem_regions: data for each of the PRUSS memory regions
 * @mem_in_use: to indicate if memory resource is in use
 */
struct pruss {
	struct udevice *dev;
	struct pruss_mem_region mem_regions[PRUSS_MEM_MAX];
	struct pruss_mem_region *mem_in_use[PRUSS_MEM_MAX];
};

int pruss_request_shrmem_region(struct udevice *dev, phys_addr_t *loc);
int pruss_request_tm_region(struct udevice *dev, phys_addr_t *loc);
int pruss_request_mem_region(struct udevice *dev, enum pruss_mem mem_id,
			     struct pruss_mem_region *region);
int pruss_release_mem_region(struct udevice *dev, struct pruss_mem_region *region);

#endif /* __TI_PRUSS_H */
