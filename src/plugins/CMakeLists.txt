set(BUILD_WITH_EMBEDDABLE_PYTHON_DEVS OFF CACHE BOOL "Build the python plugins using the embeddable python devs")
if (BUILD_WITH_EMBEDDABLE_PYTHON_DEVS)

	## get python prefix
	set(Python_PREFIX "" CACHE PATH "Python root directory (contains lib, include, etc.)")

	## extract python version string
	string(REPLACE "/" ";" SPLITTED_Python_PREFIX "${Python_PREFIX}" )
	string(REPLACE "\\" ";" SPLITTED_Python_PREFIX "${SPLITTED_Python_PREFIX}" )
	list(LENGTH SPLITTED_Python_PREFIX SPLITTED_Python_PREFIX_LENGTH)
	MATH(EXPR LAST_FOLDER_INDEX "${SPLITTED_Python_PREFIX_LENGTH} - 1")
	list(GET SPLITTED_Python_PREFIX ${LAST_FOLDER_INDEX} PYTHON_VERSION_STRING)

	## extract python version major and minor
	string(REPLACE "." ";" PYTHON_VERSION_STRING "${PYTHON_VERSION_STRING}" )
	list(LENGTH PYTHON_VERSION_STRING PYTHON_VERSION_LENGTH)
	if (PYTHON_VERSION_LENGTH EQUAL 2)
		list(GET PYTHON_VERSION_STRING 0 PYTHON_VERSION_MAJOR)
		list(GET PYTHON_VERSION_STRING 1 PYTHON_VERSION_MINOR)
		set(PYTHON_VERSION ${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR})

		## set required variables
		set(Python_INCLUDE_DIRS ${Python_PREFIX}/include/python${PYTHON_VERSION})
		set(Python_LIBRARIES ${Python_PREFIX}/Python)
		add_definitions(-DEMBEDDABLE_PYTHON_DEVS_ARE_USED=true)
		add_definitions(-DEMBEDDABLE_PYTHON_DEVS_VERSION_MAJOR=${PYTHON_VERSION_MAJOR})
		add_definitions(-DEMBEDDABLE_PYTHON_DEVS_VERSION_MINOR=${PYTHON_VERSION_MINOR})
	else ()
		message(FATAL_ERROR "Python Devs are not found. Set Python_PREFIX to the Python root directory (contains lib, include, etc.)")
	endif ()
else ()
	find_package(Python COMPONENTS Development REQUIRED)
endif ()

### build plugins
file(GLOB PLUGINS ${CMAKE_SOURCE_DIR}/src/plugins/*.cpp)
foreach(PLUGIN ${PLUGINS})
	get_filename_component(PLUGIN_NAME ${PLUGIN} NAME_WE)

	## build the plugin
	add_library(${PLUGIN_NAME} SHARED ${PLUGIN} negui_pyhelper.hpp)

	## include directories
	target_include_directories(${PLUGIN_NAME} PUBLIC
		${CMAKE_SOURCE_DIR}/src
		${CMAKE_SOURCE_DIR}/src/plugins
		${Python_INCLUDE_DIRS})

	## link libraries
	target_link_libraries(${PLUGIN_NAME}
		Qt5::Core
		Qt5::Gui
		Qt5::Widgets
		Qt5::PrintSupport
		${Python_LIBRARIES})

	## set library dist
	install(TARGETS ${PLUGIN_NAME} RUNTIME DESTINATION bin/plugins COMPONENT networkeditorgui_lib LIBRARY DESTINATION lib/plugins COMPONENT networkeditorgui_lib ARCHIVE DESTINATION lib/plugins COMPONENT networkeditorgui_lib)
endforeach()

## install headers
file(GLOB plugin_headers ${CMAKE_SOURCE_DIR}/src/plugins/*.h ${CMAKE_SOURCE_DIR}/plugins/src/*.hpp)
install(FILES ${plugin_headers} DESTINATION include/plugins COMPONENT networkeditorgui_lib)