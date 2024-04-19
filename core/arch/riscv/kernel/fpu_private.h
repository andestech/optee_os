/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2015, Linaro Limited
 * Copyright (c) 2024, Andes Technology Corporation
 */

#ifndef FPU_PRIVATE
#define FPU_PRIVATE

#include <kernel/fpu.h>

void fpu_save_extension_regs(struct fpu_reg regs[FPU_NUM_REGS]);
void fpu_restore_extension_regs(struct fpu_reg regs[FPU_NUM_REGS]);

#endif /*FPU_PRIVATE*/
