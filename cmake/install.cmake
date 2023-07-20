install(
  TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/include
)

	#copy headers to install dir
	install(
	  DIRECTORY incl/
	  DESTINATION include/${PROJECT_NAME})

	#install *Targets.cmake
	install(
	    EXPORT ${PROJECT_NAME}Targets
	    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/cmake)

	#install *Config.cmake
	install(FILES
	    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake
	    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/cmake)

	#Specifying & exporting the version of package
	include(CMakePackageConfigHelpers)
	write_basic_package_version_file(
	    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake
	    VERSION ${PROJECT_VERSION} #as specified at project(...) on top
	    COMPATIBILITY SameMajorVersion)

	install(FILES
	    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake
	    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/cmake)

