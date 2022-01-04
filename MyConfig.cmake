#*******************************************************************************
#*******************************************************************************
#*******************************************************************************
# copied from MyConfig_win.cmake

function(my_init_config_variables)

   set (MyTargetType windows PARENT_SCOPE)

   set (MyTargetHostName "127.0.0.1" PARENT_SCOPE)

   set (MyCopyBinToDir "C:/aaa_prime/CMake" PARENT_SCOPE)
   set (MyCopyLibToDir "C:/MyTools/MyLib" PARENT_SCOPE)

   set (MySudo "" PARENT_SCOPE)

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************
