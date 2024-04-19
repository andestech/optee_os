/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2015, Linaro Limited
 * Copyright (c) 2024, Andes Technology Corporation
 */

#ifndef __KERNEL_FPU_H
#define __KERNEL_FPU_H

#include <types_ext.h>
#include <compiler.h>

#define FPU_NUM_REGS	U(32)

struct fpu_reg {
	uint64_t f;
};

struct fpu_state {
	struct fpu_reg reg[FPU_NUM_REGS];
	uint32_t status_fs;
	uint32_t fcsr;
};

#ifdef CFG_WITH_FPU
/* fpu_is_enabled() - Returns true if FPU is enabled */
bool fpu_is_enabled(void);
/* fpu_enable() - Enables fpu */
void fpu_enable(void);
/* fpu_disable() - Disables fpu */
void fpu_disable(void);
#else
static inline bool fpu_is_enabled(void) { return false; }
static inline void fpu_enable(void) { }
static inline void fpu_disable(void) { }
#endif

/*
 * fpu_lazy_save_state_init() - Saves FPU enable status and disables FPU
 * @state:	FPU state structure to initialize
 */
void fpu_lazy_save_state_init(struct fpu_state *state);

/*
 * fpu_lazy_save_state_final() - Saves rest of FPU state
 * @state:	FPU state to save to
 * @force_save:	Forces saving of state regardless of previous state if true.
 *
 * If FPU was enabled when fpu_lazy_save_state_init() was called or
 * @force_save is true: save rest of state and disable FPU. Otherwise, do
 * nothing.
 */
void fpu_lazy_save_state_final(struct fpu_state *state, bool force_save);

/*
 * fpu_lazy_restore_state() - Lazy restore FPU state
 * @state:		FPU state to restore
 *
 * Restores FPU enable status and also restores rest of FPU state if
 * fpu_lazy_save_state_final() was called on this state.
 */
void fpu_lazy_restore_state(struct fpu_state *state, bool full_state);

#endif /*__KERNEL_FPU_H*/
