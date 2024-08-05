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
add_executable(${PROJECT_NAME_TITLE} WIN32 MACOSX_BUNDLE
	${source_files} ${header_files})

## include directories
target_include_directories(${PROJECT_NAME_TITLE} PUBLIC
		${ALCUIN_INCLUDEDIR})

## link libraries
target_link_libraries(${PROJECT_NAME_TITLE}
	Qt6::Core
	Qt6::Gui
	Qt6::Widgets
	Qt6::PrintSupport
	Qt6::Svg
	${ALCUIN_LIBS})

file(GLOB ALCUIN_TEST_APP_PLUGINS_SCRIPTS ${CMAKE_SOURCE_DIR}/src/plugins/*.py)
list(APPEND ALCUIN_PLUGINS_SCRIPTS ${ALCUIN_TEST_APP_PLUGINS_SCRIPTS})
set(DEPENDENCIES_BUILD_DIR ${CMAKE_BINARY_DIR}/src/dependencies)
set(PLUGINS_BUILD_DIR ${CMAKE_BINARY_DIR}/src/plugins)

add_custom_command(TARGET ${PROJECT_NAME_TITLE} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${DEPENDENCIES_BUILD_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ALCUIN_LIBS} ${DEPENDENCIES_BUILD_DIR}
	COMMAND ${CMAKE_COMMAND} -E make_directory ${PLUGINS_BUILD_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ALCUIN_PLUGINS_LIBS} ${PLUGINS_BUILD_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ALCUIN_PLUGINS_SCRIPTS} ${PLUGINS_BUILD_DIR})

if (APPLE)
	## set the info plist
	set_target_properties(${PROJECT_NAME_TITLE} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/src/Info.Plist)
	
	## copy and bundle the dependencies and plugins
	set(FRAMEWORKS_DIR $<TARGET_FILE_DIR:${PROJECT_NAME_TITLE}>/../Frameworks)
	set(PLUGINS_DIR $<TARGET_FILE_DIR:${PROJECT_NAME_TITLE}>/../PlugIns)
	set(RESOURCES_DIR $<TARGET_FILE_DIR:${PROJECT_NAME_TITLE}>/../Resources)
	add_custom_command(TARGET ${PROJECT_NAME_TITLE} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${FRAMEWORKS_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${DEPENDENCIES_BUILD_DIR} ${FRAMEWORKS_DIR}/dependencies
	COMMAND ${CMAKE_COMMAND} -E make_directory ${FRAMEWORKS_DIR}/QtCore.framework/Versions/A
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt6::Core> ${FRAMEWORKS_DIR}/QtCore.framework/Versions/A
	COMMAND ${CMAKE_COMMAND} -E make_directory ${FRAMEWORKS_DIR}/QtGui.framework/Versions/A
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt6::Gui> ${FRAMEWORKS_DIR}/QtGui.framework/Versions/A
	COMMAND ${CMAKE_COMMAND} -E make_directory ${FRAMEWORKS_DIR}/QtWidgets.framework/Versions/A
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt6::Widgets> ${FRAMEWORKS_DIR}/QtWidgets.framework/Versions/A
	COMMAND ${CMAKE_COMMAND} -E make_directory ${FRAMEWORKS_DIR}/QtDBus.framework/Versions/A
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt6::DBus>  ${FRAMEWORKS_DIR}/QtDBus.framework/Versions/A
	COMMAND ${CMAKE_COMMAND} -E make_directory ${FRAMEWORKS_DIR}/QtPrintSupport.framework/Versions/A
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt6::PrintSupport> ${FRAMEWORKS_DIR}/QtPrintSupport.framework/Versions/A
	COMMAND ${CMAKE_COMMAND} -E make_directory ${FRAMEWORKS_DIR}/QtSvg.framework/Versions/A
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt6::Svg> ${FRAMEWORKS_DIR}/QtSvg.framework/Versions/A
	COMMAND ${CMAKE_COMMAND} -E make_directory ${PLUGINS_DIR}/platforms
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt6::QCocoaIntegrationPlugin> ${PLUGINS_DIR}/platforms
	COMMAND ${CMAKE_COMMAND} -E make_directory ${PLUGINS_DIR}/styles
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt6::QMacStylePlugin> ${PLUGINS_DIR}/styles
	COMMAND ${CMAKE_COMMAND} -E make_directory ${PLUGINS_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ALCUIN_PLUGINS_LIBS} ${PLUGINS_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ALCUIN_PLUGINS_SCRIPTS} ${PLUGINS_DIR}
	COMMAND ${CMAKE_COMMAND} -E make_directory ${RESOURCES_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${ALCUIN_PREFIX}/resources ${RESOURCES_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/qt.conf ${RESOURCES_DIR})

	## install the bundle
	install(TARGETS ${PROJECT_NAME_TITLE} BUNDLE DESTINATION bin COMPONENT alcuin_test_app)

	## install embeddable python
	if (EXISTS "${ALCUIN_PREFIX}/Python.framework")
		install(DIRECTORY "${ALCUIN_PREFIX}/Python.framework" DESTINATION bin/${PROJECT_NAME_TITLE}.app/Contents/Frameworks COMPONENT alcuin_test_app)
	endif ()

elseif (UNIX)
	## install the dependencies
	install(FILES
	$<TARGET_FILE:Qt6::Core> "$<TARGET_FILE_DIR:Qt6::Core>/libQt6Core.so.6"
	$<TARGET_FILE:Qt6::Gui> "$<TARGET_FILE_DIR:Qt6::Gui>/libQt6Gui.so.6"
	$<TARGET_FILE:Qt6::Widgets> "$<TARGET_FILE_DIR:Qt6::Widgets>/libQt6Widgets.so.6"
	$<TARGET_FILE:Qt6::DBus> "$<TARGET_FILE_DIR:Qt6::DBus>/libQt6DBus.so.6"
	$<TARGET_FILE:Qt6::PrintSupport> "$<TARGET_FILE_DIR:Qt6::PrintSupport>/libQt6PrintSupport.so.6"
	$<TARGET_FILE:Qt6::Svg> "$<TARGET_FILE_DIR:Qt6::Svg>/libQt6Svg.so.6"

	DESTINATION lib/dependencies COMPONENT alcuin_test_app)

	## install the executable
	install(TARGETS ${PROJECT_NAME_TITLE} DESTINATION bin COMPONENT alcuin_test_app)

elseif (WIN32)
	## install the dependencies
	install(FILES ${ALCUIN_DLLS} DESTINATION bin COMPONENT alcuin_test_app)
	install(FILES ${ALCUIN_PLUGINS_DLLS} DESTINATION bin/plugins COMPONENT alcuin_test_app)
	install(FILES ${PY_SCRIPTS} DESTINATION bin/plugins COMPONENT alcuin_test_app)
	install(FILES $<TARGET_FILE:Qt6::Core> $<TARGET_FILE:Qt6::Gui> $<TARGET_FILE:Qt6::Widgets> $<TARGET_FILE:Qt6::DBus> $<TARGET_FILE:Qt6::PrintSupport> $<TARGET_FILE:Qt6::Svg> DESTINATION bin COMPONENT alcuin_test_app)
	install(FILES $<TARGET_FILE:Qt6::QWindowsIntegrationPlugin> DESTINATION bin/platforms   COMPONENT alcuin_test_app)
	install(FILES $<TARGET_FILE:Qt6::QWindowsVistaStylePlugin> DESTINATION bin/styles       COMPONENT alcuin_test_app)
	install(FILES ${CMAKE_SOURCE_DIR}/qt.conf DESTINATION bin COMPONENT alcuin_test_app)
	install(DIRECTORY ${ALCUIN_PREFIX}/resources DESTINATION bin COMPONENT alcuin_test_app)
	## install embeddable python
	if (EXISTS "${ALCUIN_PREFIX}/Python")
		install(DIRECTORY "${ALCUIN_PREFIX}/Python/" DESTINATION bin COMPONENT alcuin_test_app)
	endif ()

	## install the executable
	install(TARGETS ${PROJECT_NAME_TITLE} DESTINATION bin COMPONENT alcuin_test_app)
	
endif ()