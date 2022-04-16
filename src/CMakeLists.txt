### find python packages
find_package(Python COMPONENTS Development)
set (PYTHON_DEVS Python::Python)

set(CMAKE_AUTOMOC ON)

### build the library
## set the required sources and headers
set(gui_sources negui_mainwindow.cpp negui_pyhelper.hpp)
set(gui_headers negui_mainwindow.h negui_interfaces.h)

## build
add_library(NetworkEditorGUI SHARED 
	${gui_sources} 
	${gui_headers})

## link libraries
target_link_libraries(NetworkEditorGUI
	Qt5::Core
	Qt5::Gui 
	Qt5::Widgets
	Qt5::PrintSupport
	${PYTHON_DEVS})

## set library dist
install(TARGETS NetworkEditorGUI RUNTIME DESTINATION bin COMPONENT networkeditorgui_lib LIBRARY DESTINATION lib COMPONENT networkeditorgui_lib ARCHIVE DESTINATION lib COMPONENT networkeditorgui_lib)

## install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src DESTINATION include COMPONENT networkeditorgui_lib
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")