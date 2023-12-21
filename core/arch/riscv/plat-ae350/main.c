// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright 2022-2023 NXP
 * Copyright 2024 Andes Technology
 */

#include <console.h>
#include <drivers/ns16550.h>
#include <drivers/plic.h>
#include <kernel/boot.h>
#include <kernel/tee_common_otp.h>
#include <platform_config.h>

static struct ns16550_data console_data __nex_bss;

register_ddr(DRAM_BASE, DRAM_SIZE);

register_phys_mem_pgdir(MEM_AREA_IO_NSEC, UART2_BASE,
			CORE_MMU_PGDIR_SIZE);

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

void console_init(void)
{
	ns16550_init(&console_data, UART2_BASE + UART2_REG_OFFSET,
		     IO_WIDTH_U8, UART2_REG_SHIFT);
	register_serial_console(&console_data.chip);
}

static enum itr_return foreign_itr_cb(struct itr_handler *h)
{
	struct plic_data *pd = container_of(h->chip, struct plic_data, chip);

	pd->got_foreign_it = true;

	EMSG("got foreign interrupt");
	return ITRR_HANDLED;
}

static struct itr_handler foreign_itr = {
	.it = FOREIGN_IRQ,
	.flags = ITRF_TRIGGER_LEVEL,
	.handler = foreign_itr_cb,
};
DECLARE_KEEP_PAGER(foreign_itr);

static TEE_Result init_foreign_itr(void)
{
	TEE_Result res = TEE_ERROR_GENERIC;

	foreign_itr.chip = interrupt_get_main_chip();
	res = interrupt_add_configure_handler(&foreign_itr, IRQ_TYPE_LEVEL_HIGH,
					      1);
	if (res)
		return res;

	interrupt_enable(foreign_itr.chip, foreign_itr.it);

	return TEE_SUCCESS;
}
driver_init(init_foreign_itr);

void interrupt_main_handler(void)
{
	if (IS_ENABLED(CFG_RISCV_PLIC))
		plic_it_handle();
}
