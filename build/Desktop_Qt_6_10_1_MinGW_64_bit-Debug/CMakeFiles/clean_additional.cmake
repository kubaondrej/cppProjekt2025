# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appCPPProjekt_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appCPPProjekt_autogen.dir\\ParseCache.txt"
  "appCPPProjekt_autogen"
  )
endif()
