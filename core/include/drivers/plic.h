/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright 2022-2023 NXP
 */

#ifndef __DRIVERS_PLIC_H
#define __DRIVERS_PLIC_H

#include <kernel/interrupt.h>
#include <kernel/misc.h>
#include <platform_config.h>

struct plic_data {
	vaddr_t plic_base;
	size_t max_it;
	struct itr_chip chip;
	bool got_foreign_it;
};

void plic_init(paddr_t plic_base_pa);
void plic_hart_init(void);
void plic_it_handle(void);
void plic_dump_state(void);

#endif /*__DRIVERS_PLIC_H*/
