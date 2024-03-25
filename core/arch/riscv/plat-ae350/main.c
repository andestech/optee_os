// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright 2022-2023 NXP
 */

#include <console.h>
#include <drivers/plic.h>
#include <drivers/ns16550.h>
#include <kernel/boot.h>
#include <kernel/tee_common_otp.h>
#include <platform_config.h>

static struct ns16550_data console_data __nex_bss;

register_ddr(DRAM_BASE, DRAM_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC, UART2_BASE, UART2_REG_SIZE);

void plat_console_init(void)
{
	ns16550_init(&console_data, UART2_BASE + UART2_REG_OFFSET,
		     IO_WIDTH_U8, UART2_REG_SHIFT);
	register_serial_console(&console_data.chip);
}

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
