# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appcppProjekt2025Qt_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appcppProjekt2025Qt_autogen.dir\\ParseCache.txt"
  "appcppProjekt2025Qt_autogen"
  )
endif()
