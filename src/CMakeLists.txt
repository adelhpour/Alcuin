## set the required sources and headers
set(gui_sources negui_mainwindow.cpp)
set(gui_headers negui_mainwindow.h)

set(CMAKE_AUTOMOC ON)

## wrap the ui files to a c++ header
#qt5_wrap_cpp(moc_sources ${gui_headers})

### shared library
## build
add_library(networkeditorgui SHARED 
	${gui_sources} 
	${gui_headers})

## link libraries
target_link_libraries(networkeditorgui
	Qt5::Core
	Qt5::Gui 
	Qt5::Widgets
	Qt5::PrintSupport)

## set library dist
install(TARGETS networkeditorgui RUNTIME DESTINATION bin COMPONENT networkeditorgui_lib LIBRARY DESTINATION lib COMPONENT networkeditorgui_lib ARCHIVE DESTINATION lib COMPONENT networkeditorgui_lib)

## install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src DESTINATION include COMPONENT networkeditorgui_lib
        FILES_MATCHING PATTERN "*.h")