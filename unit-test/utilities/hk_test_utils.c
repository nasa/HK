#include "cfe.h"
#include "cfe_msgids.h"
#include "hk_app.h"
#include "hk_msg.h"
#include "hk_events.h"
#include "hk_version.h"
#include "hk_msgids.h"
#include "hk_test_utils.h"

int32 UT_Utils_stub_reporter_hook(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context)
{
    uint8 i            = 0;    /* i is index */
    uint8 size_used    = 0;    /* determines size of argument to be saved */
    void *val_location = NULL; /* pointer to arg value to be saved */
    void *obj_ptr;             /* tracker indicates where to push data into UserObj */

    /* Determine where in the UserObj we should be located dependent upon CallCount */
    if (CallCount == 0)
    {
        obj_ptr = UserObj;
    }
    else
    {
        uint8 context_size = 0;

        for (i = 0; i < Context->ArgCount; ++i)
        {
            /* A UT_STUBCONTEXT_ARG_TYPE_DIRECT type indicates the arg itself is the ptr argument, add a (void*) size */
            if (Context->Meta[i].Type == UT_STUBCONTEXT_ARG_TYPE_DIRECT)
            {
                context_size += sizeof(void *);
            }
            else /* UT_STUBCONTEXT_ARG_TYPE_INDIRECT indicates the arg is pointing to the value to be saved, add its
                    size */
            {
                context_size += Context->Meta[i].Size;
            }
        }

        /* obj_ptr moves a full context_size for every call (initial value is 0) -- user object for calls > 1 must be an
         * array of contexts */
        obj_ptr = UserObj + (context_size * CallCount);
    }

    for (i = 0; i < Context->ArgCount; ++i)
    {
        /* UT_STUBCONTEXT_ARG_TYPE_DIRECT indicates the arg is the ptr that is to be saved */
        if (Context->Meta[i].Type == UT_STUBCONTEXT_ARG_TYPE_DIRECT)
        {
            val_location = &Context->ArgPtr[i];
            size_used    = sizeof(void *);
        }
        else /* UT_STUBCONTEXT_ARG_TYPE_INDIRECT indicates the arg is pointing to the value to be saved */
        {
            val_location = Context->ArgPtr[i];
            size_used    = Context->Meta[i].Size;
        }
        /* put the argument value into the user object */
        memcpy(obj_ptr, val_location, size_used);
        /* move to end of this size item in the user object */
        obj_ptr += size_used;
    }

    return StubRetcode;
}

void HK_Test_InitGoodCopyTable(hk_copy_table_entry_t *CpyTbl)
{
    int32 i = 0;

    /* this setup matches the default HK table */
    CpyTbl[0].InputMid     = CFE_EVS_HK_TLM_MID;
    CpyTbl[0].InputOffset  = 12;
    CpyTbl[0].OutputMid    = HK_COMBINED_PKT1_MID;
    CpyTbl[0].OutputOffset = 12;
    CpyTbl[0].NumBytes     = 4;

    CpyTbl[1].InputMid     = CFE_TIME_HK_TLM_MID;
    CpyTbl[1].InputOffset  = 12;
    CpyTbl[1].OutputMid    = HK_COMBINED_PKT1_MID;
    CpyTbl[1].OutputOffset = 16;
    CpyTbl[1].NumBytes     = 4;

    CpyTbl[2].InputMid     = CFE_SB_HK_TLM_MID;
    CpyTbl[2].InputOffset  = 12;
    CpyTbl[2].OutputMid    = HK_COMBINED_PKT1_MID;
    CpyTbl[2].OutputOffset = 20;
    CpyTbl[2].NumBytes     = 4;

    CpyTbl[3].InputMid     = CFE_ES_HK_TLM_MID;
    CpyTbl[3].InputOffset  = 12;
    CpyTbl[3].OutputMid    = HK_COMBINED_PKT1_MID;
    CpyTbl[3].OutputOffset = 24;
    CpyTbl[3].NumBytes     = 4;

    CpyTbl[4].InputMid     = CFE_EVS_HK_TLM_MID;
    CpyTbl[4].InputOffset  = 12;
    CpyTbl[4].OutputMid    = HK_COMBINED_PKT1_MID;
    CpyTbl[4].OutputOffset = 28;
    CpyTbl[4].NumBytes     = 4;

    for (i = 5; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        CpyTbl[i].InputMid     = HK_UNDEFINED_ENTRY;
        CpyTbl[i].InputOffset  = 0;
        CpyTbl[i].OutputMid    = HK_UNDEFINED_ENTRY;
        CpyTbl[i].OutputOffset = 0;
        CpyTbl[i].NumBytes     = 0;
    }
}

void HK_Test_InitEmptyCopyTable(hk_copy_table_entry_t *CpyTbl)
{
    int32 i = 0;

    for (i = 0; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        CpyTbl[i].InputMid     = HK_UNDEFINED_ENTRY;
        CpyTbl[i].InputOffset  = 0;
        CpyTbl[i].OutputMid    = HK_UNDEFINED_ENTRY;
        CpyTbl[i].OutputOffset = 0;
        CpyTbl[i].NumBytes     = 0;
    }
}

void HK_Test_InitGoodRuntimeTable(hk_runtime_tbl_entry_t *RtTbl)
{
    int32 i = 0;

    hk_runtime_tbl_entry_t *RtEntry = NULL;

    /* Loop thru the RunTime table initializing the fields */
    for (i = 0; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        RtEntry = &RtTbl[i];

        if (i < 5)
        {
            RtEntry->OutputPktAddr      = 1; /* just needs to be non-null */
            RtEntry->InputMidSubscribed = HK_INPUTMID_SUBSCRIBED;
            RtEntry->DataPresent        = HK_DATA_NOT_PRESENT;
        }
        else
        {
            RtEntry->OutputPktAddr      = NULL;
            RtEntry->InputMidSubscribed = HK_INPUTMID_NOT_SUBSCRIBED;
            RtEntry->DataPresent        = HK_DATA_NOT_PRESENT;
        }
    }
}

void HK_Test_InitEmptyRuntimeTable(hk_runtime_tbl_entry_t *RtTbl)
{
    int32 i = 0;

    hk_runtime_tbl_entry_t *RtEntry = NULL;

    /* Loop thru the RunTime table initializing the fields */
    for (i = 0; i < HK_COPY_TABLE_ENTRIES; i++)
    {
        RtEntry = &RtTbl[i];

        RtEntry->OutputPktAddr      = NULL;
        RtEntry->InputMidSubscribed = HK_INPUTMID_NOT_SUBSCRIBED;
        RtEntry->DataPresent        = HK_DATA_NOT_PRESENT;
    }
}
