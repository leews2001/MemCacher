#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "MemCacherLib" for configuration "Debug"
set_property(TARGET MemCacherLib APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(MemCacherLib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/MemCacherLib.lib"
  )

list(APPEND _cmake_import_check_targets MemCacherLib )
list(APPEND _cmake_import_check_files_for_MemCacherLib "${_IMPORT_PREFIX}/lib/MemCacherLib.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
