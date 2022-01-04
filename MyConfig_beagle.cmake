#*******************************************************************************
#*******************************************************************************
#*******************************************************************************
# copied from MyConfig_beagle.cmake

function(my_init_config_variables)

   set (MyTargetType beagle PARENT_SCOPE)

   set (MyTargetHostName "debian@bbx" PARENT_SCOPE)

   set (MyCopyBinToDir "/opt/prime/bin" PARENT_SCOPE)
   set (MyCopyLibToDir "/opt/prime/lib" PARENT_SCOPE)

   set (MySudo "sudo " PARENT_SCOPE)

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************
