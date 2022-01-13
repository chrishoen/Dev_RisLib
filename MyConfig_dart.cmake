#*******************************************************************************
#*******************************************************************************
#*******************************************************************************
# copied from MyConfig_dart.cmake

function(my_init_config_variables)

   set (MyTargetType dart PARENT_SCOPE)

   set (MyTargetHostName "root@dart" PARENT_SCOPE)

   set (MyCopyBinToDir "/home/root/stenograph/bin" PARENT_SCOPE)
   set (MyCopyLibToDir "/home/root/stenograph/lib" PARENT_SCOPE)
   set (SRC_LINUX_DIR "/home/main/work/nextgen/src_linux"  PARENT_SCOPE)

   set (MySudo "" PARENT_SCOPE)

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************
