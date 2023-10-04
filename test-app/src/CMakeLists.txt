set(CMAKE_AUTOMOC ON)

### build the main gui
## set the required sources and headers
set(source_files
		negui_main.cpp
		negui_main_window.cpp
		negui_menu_bar.cpp
		negui_about_message_box.cpp)

set(header_files
		negui_main_window.h
		negui_menu_bar.h
		negui_about_message_box.h)

## build the bundle/executable
add_executable(AlcuinTestApp WIN32 MACOSX_BUNDLE 
	${source_files} ${header_files})

## include directories
target_include_directories(AlcuinTestApp PUBLIC
		${ALCUIN_INCLUDEDIR})

## link libraries
target_link_libraries(AlcuinTestApp
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
	Qt5::PrintSupport
	Qt5::Svg
	${ALCUIN_LIBS})

file(GLOB PY_SCRIPTS ${CMAKE_SOURCE_DIR}/src/plugins/*.py)
set(DEPENDENCIES_BUILD_DIR ${CMAKE_BINARY_DIR}/src/dependencies)
set(PLUGINS_BUILD_DIR ${CMAKE_BINARY_DIR}/src/plugins)

add_custom_command(TARGET AlcuinTestApp POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${DEPENDENCIES_BUILD_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ALCUIN_LIBS} ${DEPENDENCIES_BUILD_DIR}
	COMMAND ${CMAKE_COMMAND} -E make_directory ${PLUGINS_BUILD_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ALCUIN_PLUGINS_LIBS} ${PLUGINS_BUILD_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PY_SCRIPTS} ${PLUGINS_BUILD_DIR})

if (APPLE)
	## set the info plist
	set_target_properties(AlcuinTestApp PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/src/Info.Plist)
	
	## copy and bundle the dependencies and plugins
	set(FRAMEWORKS_DIR $<TARGET_FILE_DIR:AlcuinTestApp>/../Frameworks)
	set(PLUGINS_DIR $<TARGET_FILE_DIR:AlcuinTestApp>/../PlugIns)
	set(RESOURCES_DIR $<TARGET_FILE_DIR:AlcuinTestApp>/../Resources)
	add_custom_command(TARGET AlcuinTestApp POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${FRAMEWORKS_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${DEPENDENCIES_BUILD_DIR} ${FRAMEWORKS_DIR}/dependencies
	COMMAND ${CMAKE_COMMAND} -E make_directory ${FRAMEWORKS_DIR}/QtCore.framework/Versions/5
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> ${FRAMEWORKS_DIR}/QtCore.framework/Versions/5
	COMMAND ${CMAKE_COMMAND} -E make_directory ${FRAMEWORKS_DIR}/QtGui.framework/Versions/5
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> ${FRAMEWORKS_DIR}/QtGui.framework/Versions/5
	COMMAND ${CMAKE_COMMAND} -E make_directory ${FRAMEWORKS_DIR}/QtWidgets.framework/Versions/5
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets> ${FRAMEWORKS_DIR}/QtWidgets.framework/Versions/5
	COMMAND ${CMAKE_COMMAND} -E make_directory ${FRAMEWORKS_DIR}/QtDBus.framework/Versions/5
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::DBus>  ${FRAMEWORKS_DIR}/QtDBus.framework/Versions/5
	COMMAND ${CMAKE_COMMAND} -E make_directory ${FRAMEWORKS_DIR}/QtPrintSupport.framework/Versions/5
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::PrintSupport> ${FRAMEWORKS_DIR}/QtPrintSupport.framework/Versions/5
	COMMAND ${CMAKE_COMMAND} -E make_directory ${FRAMEWORKS_DIR}/QtSvg.framework/Versions/5
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Svg> ${FRAMEWORKS_DIR}/QtSvg.framework/Versions/5
	COMMAND ${CMAKE_COMMAND} -E make_directory ${PLUGINS_DIR}/platforms
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QCocoaIntegrationPlugin> ${PLUGINS_DIR}/platforms
	COMMAND ${CMAKE_COMMAND} -E make_directory ${PLUGINS_DIR}/styles
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QMacStylePlugin> ${PLUGINS_DIR}/styles
	COMMAND ${CMAKE_COMMAND} -E make_directory ${PLUGINS_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ALCUIN_PLUGINS_LIBS} ${PLUGINS_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PY_SCRIPTS} ${PLUGINS_DIR}
	COMMAND ${CMAKE_COMMAND} -E make_directory ${RESOURCES_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${ALCUIN_PREFIX}/resources ${RESOURCES_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/qt.conf ${RESOURCES_DIR})

	## install embeddable python
	if (EXISTS "${ALCUIN_PREFIX}/Python.framework")
		add_custom_command(TARGET AlcuinTestApp POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E make_directory ${FRAMEWORKS_DIR}/Python.framework
				COMMAND ${CMAKE_COMMAND} -E copy_directory "${ALCUIN_PREFIX}/Python.framework" ${FRAMEWORKS_DIR}/Python.framework)
	endif ()

	## install the bundle
	install(TARGETS AlcuinTestApp BUNDLE DESTINATION bin COMPONENT alcuin_test_app)

elseif (UNIX)
	## install the dependencies
	install(FILES
	$<TARGET_FILE:Qt5::Core> "$<TARGET_FILE_DIR:Qt5::Core>/libQt5Core.so.5"
	$<TARGET_FILE:Qt5::Gui> "$<TARGET_FILE_DIR:Qt5::Gui>/libQt5Gui.so.5"
	$<TARGET_FILE:Qt5::Widgets> "$<TARGET_FILE_DIR:Qt5::Widgets>/libQt5Widgets.so.5"
	$<TARGET_FILE:Qt5::PrintSupport> "$<TARGET_FILE_DIR:Qt5::PrintSupport>/libQt5PrintSupport.so.5"
	$<TARGET_FILE:Qt5::Svg> "$<TARGET_FILE_DIR:Qt5::Svg>/libQt5Svg.so.5"
	DESTINATION lib/dependencies COMPONENT alcuin_test_app)

	## install the executable
	install(TARGETS AlcuinTestApp DESTINATION bin COMPONENT alcuin_test_app)

elseif (WIN32)
	## install the dependencies
	install(FILES ${ALCUIN_DLLS} DESTINATION bin COMPONENT alcuin_test_app)
	install(FILES ${ALCUIN_PLUGINS_DLLS} DESTINATION bin/plugins COMPONENT alcuin_test_app)
	install(FILES ${PY_SCRIPTS} DESTINATION bin/plugins COMPONENT alcuin_test_app)
	install(FILES $<TARGET_FILE:Qt5::Core> $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE:Qt5::PrintSupport> $<TARGET_FILE:Qt5::Svg> DESTINATION bin COMPONENT alcuin_test_app)
	install(FILES $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> DESTINATION bin/platforms   COMPONENT alcuin_test_app)
	install(FILES $<TARGET_FILE:Qt5::QWindowsVistaStylePlugin> DESTINATION bin/styles       COMPONENT alcuin_test_app)
	install(FILES ${CMAKE_SOURCE_DIR}/qt.conf DESTINATION bin COMPONENT alcuin_test_app)
	install(DIRECTORY ${ALCUIN_PREFIX}/resources DESTINATION bin COMPONENT alcuin_test_app)
	## install embeddable python
	if (EXISTS "${ALCUIN_PREFIX}/Python")
		install(DIRECTORY "${ALCUIN_PREFIX}/Python/" DESTINATION bin COMPONENT alcuin_test_app)
	endif ()

	## install the executable
	install(TARGETS AlcuinTestApp DESTINATION bin COMPONENT alcuin_test_app)
	
endif ()