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
  hk_fcncodes.h
  hk_interface_cfg.h
  hk_mission_cfg.h
  hk_msg.h
  hk_msgdefs.h
  hk_msgstruct.h
  hk_tbl.h
  hk_tbldefs.h
  hk_tblstruct.h
)

# Create wrappers around the all the config header files
# This makes them individually overridable by the missions, without modifying
# the distribution default copies
foreach(HK_CFGFILE ${HK_MISSION_CONFIG_FILE_LIST})
  get_filename_component(CFGKEY "${HK_CFGFILE}" NAME_WE)
  if (DEFINED HK_CFGFILE_SRC_${CFGKEY})
    set(DEFAULT_SOURCE "${HK_CFGFILE_SRC_${CFGKEY}}")
  else()
    set(DEFAULT_SOURCE "${CMAKE_CURRENT_LIST_DIR}/config/default_${HK_CFGFILE}")
  endif()

  generate_config_includefile(
    FILE_NAME           "${HK_CFGFILE}"
    FALLBACK_FILE       "${DEFAULT_SOURCE}"
  )
endforeach()
