// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright 2022-2023 NXP
 */

#include <console.h>
#include <drivers/plic.h>
#include <kernel/boot.h>
#include <kernel/tee_common_otp.h>
#include <platform_config.h>

register_ddr(DRAM_BASE, DRAM_SIZE);

#ifdef CFG_RISCV_PLIC
void boot_primary_init_intc(void)
{
	plic_init(PLIC_BASE);
}

void boot_secondary_init_intc(void)
{
	plic_hart_init();
}
#endif /* CFG_RISCV_PLIC */

void interrupt_main_handler(void)
{
	if (IS_ENABLED(CFG_RISCV_PLIC))
		plic_it_handle();
}
