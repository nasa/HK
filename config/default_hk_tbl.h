/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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
 *  The CFS Housekeeping (HK) Application Table Structure Definition
 *
 * Provides default definitions for HK table structures
 *
 * @note This file may be overridden/superceded by mission-provided defintions
 * either by overriding this header or by generating definitions from a command/data
 * dictionary tool.
 */
#ifndef DEFAULT_HK_TBL_H
#define DEFAULT_HK_TBL_H

/************************************************************************
** Includes
*************************************************************************/
#include "common_types.h"

#include "hk_tbldefs.h"
#include "hk_tblstruct.h"

/* Provide backward compatibility with existing table definitions */
typedef HK_CopyTableEntry_t    hk_copy_table_entry_t;
typedef HK_RuntimeTableEntry_t hk_runtime_tbl_entry_t;

#endif
