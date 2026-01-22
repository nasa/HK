###########################################################
#
# HK App mission build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# Add stand alone documentation
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/docs/dox_src ${MISSION_BINARY_DIR}/docs/hk-usersguide)

# The list of header files that control the HK configuration
set(HK_MISSION_CONFIG_FILE_LIST
  hk_fcncode_values.h
  hk_interface_cfg_values.h
  hk_mission_cfg.h
  hk_msgdefs.h
  hk_msg.h
  hk_msgstruct.h
  hk_tbldefs.h
  hk_tbl.h
  hk_tblstruct.h
  hk_topicid_values.h
)

generate_configfile_set(${HK_MISSION_CONFIG_FILE_LIST})
