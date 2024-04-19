// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2015, Linaro Limited
 * Copyright (c) 2024, Andes Technology Corporation
 */

#include <riscv.h>
#include <assert.h>
#include <kernel/fpu.h>
#include "fpu_private.h"

bool fpu_is_enabled(void)
{
	unsigned long status = read_csr(CSR_SSTATUS);

	return !!(status & STATUS_FS_MASK);
}

void fpu_enable(void)
{
	unsigned long status = read_csr(CSR_SSTATUS);

	assert(!fpu_is_enabled());
	status |= STATUS_FS_INITIAL;
	write_csr(CSR_SSTATUS, status);
}

void fpu_disable(void)
{
	unsigned long status = read_csr(CSR_SSTATUS);

	status &= ~STATUS_FS_MASK;
	status |= STATUS_FS_OFF;
	write_csr(CSR_SSTATUS, status);
}

static int fpu_fs_is_dirty(void)
{
	unsigned long status = read_csr(CSR_SSTATUS);

	return (status & STATUS_FS_MASK) == STATUS_FS_DIRTY;
}

void fpu_lazy_save_state_init(struct fpu_state *state)
{
	unsigned long status = read_csr(CSR_SSTATUS);

	state->status_fs = (uint32_t)status & STATUS_FS_MASK;
	fpu_disable();
}

void fpu_lazy_save_state_final(struct fpu_state *state, bool force_save)
{
	if (fpu_fs_is_dirty() || force_save) {
		assert(!fpu_is_enabled());
		fpu_enable();
		state->fcsr = read_csr(CSR_FCSR);
		fpu_save_extension_regs(state->reg);
		fpu_disable();
	}
}

void fpu_lazy_restore_state(struct fpu_state *state, bool full_state)
{
	unsigned long status = read_csr(CSR_SSTATUS);

	if (full_state) {
		fpu_enable();
		write_csr(CSR_FCSR, state->fcsr);
		fpu_restore_extension_regs(state->reg);
	}
	status &= ~STATUS_FS_MASK;
	status |= state->status_fs;
	write_csr(CSR_SSTATUS, status);
}
