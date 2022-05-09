#ifndef _hk_test_utils_h_
#define _hk_test_utils_h_

#include "hk_app.h"
#include "hk_msg.h"
#include "hk_events.h"
#include "hk_version.h"
#include "hk_msgids.h"
#include "hk_tbldefs.h"
#include "utstubs.h"

/*
 * Global context structures
 */
typedef struct
{
    uint16      EventID;
    uint16      EventType;
    const char *Spec;
} __attribute__((packed)) CFE_EVS_SendEvent_context_t;

typedef struct
{
    const char *Spec;
} __attribute__((packed)) CFE_ES_WriteToSysLog_context_t;

int32 UT_Utils_stub_reporter_hook(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context);

void HK_Test_InitGoodCopyTable(hk_copy_table_entry_t *CpyTbl);

void HK_Test_InitEmptyCopyTable(hk_copy_table_entry_t *CpyTbl);

void HK_Test_InitGoodRuntimeTable(hk_runtime_tbl_entry_t *RtTbl);

void HK_Test_InitEmptyRuntimeTable(hk_runtime_tbl_entry_t *RtTbl);

#endif /* _hk_test_utils_h_ */
