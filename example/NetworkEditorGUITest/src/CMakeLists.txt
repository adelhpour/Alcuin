## set the required sources and headers
set(gui_sources ne_main.cpp)

## create the executable
add_executable(NetworkEditor-GUI WIN32 MACOSX_BUNDLE 
	${gui_sources})

## include directories
target_include_directories(NetworkEditor-GUI PUBLIC
	${NETWORKEDITORGUI_INCLUDEDIR})

## link libraries
target_link_libraries(NetworkEditor-GUI
	Qt5::Core
	Qt5::Gui 
	Qt5::Widgets
	Qt5::PrintSupport
	${NETWORKEDITORGUI_LIBS})

if (APPLE)
	## set the info plist for macOS	
	set_target_properties(NetworkEditor-GUI PROPERTIES MACOSX_BUNDLE_INFO_PLIST 			${CMAKE_SOURCE_DIR}/src/Info.Plist)
	install(TARGETS NetworkEditor-GUI BUNDLE DESTINATION bin COMPONENT networkeditor_gui_test)
elseif (WIN32)
	install(FILES ${NETWORKEDITORGUI_DLLS} DESTINATION bin COMPONENT networkeditor_gui)
	install(FILES $<TARGET_FILE:Qt5::Core> $<TARGET_FILE:Qt5::Gui>          $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE:Qt5::PrintSupport> DESTINATION bin COMPONENT networkeditor_gui_test)
	install(FILES $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> DESTINATION bin/platforms   COMPONENT networkeditor_gui_test)
	install(FILES $<TARGET_FILE:Qt5::QWindowsVistaStylePlugin> DESTINATION bin/styles       COMPONENT networkeditor_gui_test)
	install(TARGETS NetworkEditor-GUI DESTINATION bin COMPONENT networkeditor_gui_test)
else ()
	install(TARGETS NetworkEditor-GUI DESTINATION bin COMPONENT networkeditor_gui_test)
endif ()