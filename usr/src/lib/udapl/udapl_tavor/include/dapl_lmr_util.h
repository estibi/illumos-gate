/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright (c) 2002-2003, Network Appliance, Inc. All rights reserved.
 */

/*
 * Copyright 2003 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 *
 * HEADER: dapl_lmr_util.h
 *
 * PURPOSE: Utility defs & routines for the LMR data structure
 *
 */

#ifndef _DAPL_LMR_UTIL_H_
#define	_DAPL_LMR_UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dapl_mr_util.h"

/*
 *
 * Function Prototypes
 *
 */

extern DAPL_LMR *dapl_lmr_alloc(IN DAPL_IA *ia,
				IN DAT_MEM_TYPE mem_type,
				IN DAT_REGION_DESCRIPTION region_desc,
				IN DAT_VLEN length,
				IN DAT_PZ_HANDLE pz_handle,
				IN DAT_MEM_PRIV_FLAGS mem_priv);

extern void dapl_lmr_dealloc(IN DAPL_LMR *lmr);

extern int32_t
dapl_lmr_convert_privileges(IN DAT_MEM_PRIV_FLAGS privileges);

#ifdef __cplusplus
}
#endif

#endif /* _DAPL_LMR_UTIL_H_ */
