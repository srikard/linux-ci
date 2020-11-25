/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Common timebase prototypes and such for all ppc machines.
 *
 * Written by Cort Dougan (cort@cs.nmt.edu) to merge
 * Paul Mackerras' version and mine for PReP and Pmac.
 */

#ifndef __POWERPC_TIMEBASE_H
#define __POWERPC_TIMEBASE_H

#include <asm/reg.h>

/* For compatibility, get_tbl() is defined as get_tb() on ppc64 */
static inline unsigned long get_tbl(void)
{
	return mftb();
}

static inline u64 get_tb(void)
{
	unsigned int tbhi, tblo, tbhi2;

	/*
	 * We use __powerpc64__ here not CONFIG_PPC64 because we want the compat
	 * VDSO to use the 32-bit compatible version in the while loop below.
	 */
	if (IS_BUILTIN(__powerpc64__))
		return mftb();

	do {
		tbhi = mftbu();
		tblo = mftb();
		tbhi2 = mftbu();
	} while (tbhi != tbhi2);

	return ((u64)tbhi << 32) | tblo;
}

static inline void set_tb(unsigned int upper, unsigned int lower)
{
	mtspr(SPRN_TBWL, 0);
	mtspr(SPRN_TBWU, upper);
	mtspr(SPRN_TBWL, lower);
}
#endif /* __POWERPC_TIMEBASE_H */
