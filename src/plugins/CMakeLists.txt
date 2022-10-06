## find python packages
find_package(Python COMPONENTS Development)
set(PYTHON_DEVS Python::Python)

### build plugins
file(GLOB PLUGINS ${CMAKE_SOURCE_DIR}/src/plugins/*.cpp)
foreach(PLUGIN ${PLUGINS})
	get_filename_component(PLUGIN_NAME ${PLUGIN} NAME_WE)

	## build the plugin
	add_library(${PLUGIN_NAME} SHARED ${PLUGIN} negui_pyhelper.hpp)

	## include directories
	target_include_directories(${PLUGIN_NAME} PUBLIC
		${CMAKE_SOURCE_DIR}/src
		${CMAKE_SOURCE_DIR}/src/plugins)

	## link libraries
	target_link_libraries(${PLUGIN_NAME}
		Qt5::Core
		Qt5::Gui 
		Qt5::Widgets
		Qt5::PrintSupport
		${PYTHON_DEVS})

	## set library dist
	install(TARGETS ${PLUGIN_NAME} RUNTIME DESTINATION bin/plugins COMPONENT networkeditorgui_lib LIBRARY DESTINATION lib/plugins COMPONENT networkeditorgui_lib ARCHIVE DESTINATION lib/plugins COMPONENT networkeditorgui_lib)
endforeach()

## install headers
file(GLOB plugin_headers ${CMAKE_SOURCE_DIR}/src/plugins/*.h ${CMAKE_SOURCE_DIR}/plugins/src/*.hpp)
install(FILES ${plugin_headers} DESTINATION include/plugins COMPONENT networkeditorgui_lib)