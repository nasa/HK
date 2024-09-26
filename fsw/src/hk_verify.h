/************************************************************************
 * NASA Docket No. GSC-18,919-1, and identified as “Core Flight
 * System (cFS) Housekeeping (HK) Application version 2.5.1”
 *
 * Copyright (c) 2021 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * @file
 *  Define the CFS Housekeeping (HK) Application compile-time checks
 */
#ifndef HK_VERIFY_H
#define HK_VERIFY_H

#include "cfe.h"

#include "hk_platform_cfg.h"

#include "hk_app.h"

#if HK_DISCARD_INCOMPLETE_COMBO < 0
#error HK_DISCARD_INCOMPLETE_COMBO cannot be less than 0!
#elif HK_DISCARD_INCOMPLETE_COMBO > 1
#error HK_DISCARD_INCOMPLETE_COMBO cannot be greater than 1!
#endif

#ifndef HK_PIPE_DEPTH
#error HK_PIPE_DEPTH must be defined!
#elif (HK_PIPE_DEPTH < 1)
#error HK_PIPE_DEPTH cannot be less than 1!
#endif
/*
 * JPH 2015-06-29 - Removed check of:
 *  HK_PIPE_DEPTH  >  OS_QUEUE_MAX_DEPTH
 *
 * This is not a valid check anymore, as the HK app does not have knowledge
 * of OS_QUEUE_MAX_DEPTH.  But if the configuration violates this rule it will
 * show up as an obvious run-time error so the compile-time check is redundant.
 */

#ifndef HK_COPY_TABLE_ENTRIES
#error HK_COPY_TABLE_ENTRIES must be defined!
#elif (HK_COPY_TABLE_ENTRIES < 1)
#error HK_COPY_TABLE_ENTRIES cannot be less than 1!
#elif (HK_COPY_TABLE_ENTRIES > 8192)
#error HK_COPY_TABLE_ENTRIES cannot be greater than 8192!
#endif

#ifndef HK_NUM_BYTES_IN_MEM_POOL
#error HK_NUM_BYTES_IN_MEM_POOL must be defined!
#endif

#endif
