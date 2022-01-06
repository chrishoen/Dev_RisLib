#*******************************************************************************
#*******************************************************************************
#*******************************************************************************
# copied from MyConfig_ubuntu.cmake

function(my_init_config_variables)

   set (MyTargetType ubuntu PARENT_SCOPE)

   set (MyTargetHostName "main@cpdev1" PARENT_SCOPE)
   set (MyTargetHostName "127.0.0.1" PARENT_SCOPE)

   set (MyCopyBinToDir "/opt/prime/bin" PARENT_SCOPE)
   set (MyCopyLibToDir "/opt/prime/lib" PARENT_SCOPE)

   set (MySudo "sudo " PARENT_SCOPE)

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************
