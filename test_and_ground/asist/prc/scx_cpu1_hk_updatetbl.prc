PROC scx_cpu1_hk_updatetbl

#include "ut_statusdefs.h"
#include "ut_cfe_info.h"
#include "cfe_platform_cfg.h"
#include "cfe_evs_events.h"
#include "cfe_es_events.h"
#include "to_events.h"
#include "hk_platform_cfg.h"
#include "hk_events.h"
#include "tst_hk_events.h"
#include "cfs_hk_requirements.h"

write ";*********************************************************************"
write ";  Step 1.0:  Update Copy Table."
write ";*********************************************************************"
write ";  Step 1.1: Generate and upload updated copy table"
write ";*********************************************************************"

s SCX_CPU1_hk_tabletst
start load_table("hk_cpy_tbl.tbl", "CPU1")
wait 20
ut_sendcmd "SCX_CPU1_TBL_VALIDATE INACTIVE VTABLENAME=""HK.CopyTable"""
wait 20
ut_sendcmd "SCX_CPU1_TBL_ACTIVATE ATABLENAME=""HK.CopyTable"""
wait 10


write ";*********************************************************************"
write ";  End procedure SCX_CPU1_k_updatetbl                         "
write ";*********************************************************************"
ENDPROC
