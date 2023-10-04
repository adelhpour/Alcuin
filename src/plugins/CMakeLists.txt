set(BUILD_WITH_EMBEDDABLE_PYTHON_DEVS OFF CACHE BOOL "Build the python plugins using the embeddable python devs")
if (BUILD_WITH_EMBEDDABLE_PYTHON_DEVS)
	set(Python_VERSION_MAJOR 0)
	set(Python_VERSION_MINOR 0)
	if(APPLE)
		## get python prefix
		set(Python_PREFIX "" CACHE PATH "Python root directory (contains lib, include, etc.)")

		## extract python version string
		string(REPLACE "/" ";" SPLITTED_Python_PREFIX "${Python_PREFIX}" )
		string(REPLACE "\\" ";" SPLITTED_Python_PREFIX "${SPLITTED_Python_PREFIX}" )
		list(LENGTH SPLITTED_Python_PREFIX SPLITTED_Python_PREFIX_LENGTH)
		math(EXPR LAST_FOLDER_INDEX "${SPLITTED_Python_PREFIX_LENGTH} - 1")
		list(GET SPLITTED_Python_PREFIX ${LAST_FOLDER_INDEX} Python_VERSION_STRING)

		## extract python version major and minor
		string(REPLACE "." ";" Python_VERSION_STRING "${Python_VERSION_STRING}" )
		list(LENGTH Python_VERSION_STRING Python_VERSION_LENGTH)
		if (Python_VERSION_LENGTH EQUAL 2)
			list(GET Python_VERSION_STRING 0 Python_VERSION_MAJOR)
			list(GET Python_VERSION_STRING 1 Python_VERSION_MINOR)
			set(Python_VERSION ${Python_VERSION_MAJOR}.${Python_VERSION_MINOR})

			## set required variables
			set(Python_INCLUDE_DIRS ${Python_PREFIX}/include/python${Python_VERSION})
			set(Python_LIBRARIES ${Python_PREFIX}/Python)
		endif ()
	elseif (WIN32)
		set(Python_PREFIX "" CACHE PATH "Python root directory (contains .dll)")

		## extract python version string
		file(GLOB Python_DLLS_FULL_PATHS ${Python_PREFIX}/*.dll)
		foreach(Python_DLL_FULL_PATH ${Python_DLLS_FULL_PATHS})
			string(REPLACE "/" ";" SPLITTED_Python_DLL_FULL_PATH "${Python_DLL_FULL_PATH}" )
			string(REPLACE "\\" ";" SPLITTED_Python_DLL_FULL_PATH "${SPLITTED_Python_DLL_FULL_PATH}" )
			list(LENGTH SPLITTED_Python_DLL_FULL_PATH SPLITTED_Python_DLL_FULL_PATH_LENGTH)
			math(EXPR LAST_FOLDER_INDEX "${SPLITTED_Python_DLL_FULL_PATH_LENGTH} - 1")
			list(GET SPLITTED_Python_DLL_FULL_PATH ${LAST_FOLDER_INDEX} Python_DLL)
			string(REPLACE "." ";" SPLITTED_Python_DLL "${Python_DLL}" )
			list(GET SPLITTED_Python_DLL 0 Python_DLL_NAME)
			if (Python_DLL_NAME MATCHES "python")
				string(REPLACE "python" "" Python_DLL_VERSION ${Python_DLL_NAME})
				string(REPLACE "_d" "" Python_DLL_VERSION ${Python_DLL_VERSION})
				set(BASE_VERSION 10)
				if (Python_DLL_VERSION GREATER ${BASE_VERSION})
					if (Python_DLL_VERSION GREATER 100)
						set(BASE_VERSION 100)
					endif ()
					math(EXPR Python_VERSION_MAJOR "${Python_DLL_VERSION} / ${BASE_VERSION}")
					math(EXPR Python_VERSION_MINOR "${Python_DLL_VERSION} % ${BASE_VERSION}")
					set(Python_INCLUDE_DIRS "${Python_PREFIX}/include")
					set(Python_LIBRARIES ${Python_PREFIX}/libs/python${Python_VERSION_MAJOR}${Python_VERSION_MINOR}.lib)
				endif ()
			endif ()
		endforeach ()
	endif ()

	if (${Python_VERSION_MAJOR} GREATER 0)
		add_definitions(-DEMBEDDABLE_PYTHON_DEVS_ARE_USED=true)
		add_definitions(-DEMBEDDABLE_PYTHON_DEVS_VERSION_MAJOR=${Python_VERSION_MAJOR})
		add_definitions(-DEMBEDDABLE_PYTHON_DEVS_VERSION_MINOR=${Python_VERSION_MINOR})
	else ()
		message(FATAL_ERROR "Python Devs are not found. Set Python_PREFIX to the Python root directory")
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
			Qt5::Svg
			${Python_LIBRARIES})

	## set library dist
	install(TARGETS ${PLUGIN_NAME} RUNTIME DESTINATION bin/plugins COMPONENT alcuin_lib LIBRARY DESTINATION lib/plugins COMPONENT alcuin_lib ARCHIVE DESTINATION lib/plugins COMPONENT alcuin_lib)
endforeach()

## install headers
file(GLOB plugin_headers ${CMAKE_SOURCE_DIR}/src/plugins/*.h ${CMAKE_SOURCE_DIR}/plugins/src/*.hpp)
install(FILES ${plugin_headers} DESTINATION include/plugins COMPONENT alcuin_lib)

## install embeddable python
if (${BUILD_WITH_EMBEDDABLE_PYTHON_DEVS})
	if (APPLE)
		install(DIRECTORY ${Python_PREFIX} DESTINATION ${CMAKE_INSTALL_PREFIX}/Python.framework/Versions)
	elseif (WIN32)
		install(DIRECTORY ${Python_PREFIX} DESTINATION ${CMAKE_INSTALL_PREFIX})
	endif ()
endif ()