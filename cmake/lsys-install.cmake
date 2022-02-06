install(TARGETS lsys
	EXPORT lsysTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(FILES lsys.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
	EXPORT lsysTargets
	FILE lsysTargets.cmake
	NAMESPACE lsys::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lsys
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/lsysConfig.cmake"
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lsys
)
install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/lsysConfig.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/lsysConfigVersion.cmake"
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lsys
)

set(version 3.4.1)

set_property(TARGET lsys PROPERTY VERSION ${version})
set_property(TARGET lsys PROPERTY SOVERSION 3)
set_property(TARGET lsys PROPERTY
  INTERFACE_lsys_MAJOR_VERSION 3)
set_property(TARGET lsys APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING lsys_MAJOR_VERSION
)

# generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/lsysConfigVersion.cmake"
  VERSION "${version}"
  COMPATIBILITY AnyNewerVersion
)