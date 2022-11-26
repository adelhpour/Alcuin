cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(networkeditorguitestapp)

include(GNUInstallDirs)

if(WIN32)
	## on windows Ws2_32.lib is needed to be included for the dll
    	list(APPEND EXTRA_LIBS "Ws2_32.lib")

	## get networkeditorgui directories
	set(NETWORKEDITORGUI_PREFIX "C:/Program Files/libnetworkeditorgui" CACHE PATH "networkeditorGUI root directory (contains lib, include, etc.)")
	file(GLOB NETWORKEDITORGUI_LIBS ${NETWORKEDITORGUI_PREFIX}/lib/*.lib)
	file(GLOB NETWORKEDITORGUI_DLLS "${NETWORKEDITORGUI_PREFIX}/bin/*.dll")
	file(GLOB NETWORKEDITORGUI_PLUGINS_LIBS ${NETWORKEDITORGUI_PREFIX}/lib/plugins/*.lib)
	file(GLOB NETWORKEDITORGUI_PLUGINS_DLLS "${NETWORKEDITORGUI_PREFIX}/bin/plugins/*.dll")
elseif(UNIX)
	## get networkeditorgui directories
	set(NETWORKEDITORGUI_PREFIX "/usr/local" CACHE PATH "libnetworkeditorGUI root directory (contains lib, include, etc.)")
	
	## set use of RPATH for Mac
	if (APPLE)
		file(GLOB NETWORKEDITORGUI_LIBS ${NETWORKEDITORGUI_PREFIX}/lib/*.dylib)
		file(GLOB NETWORKEDITORGUI_PLUGINS_LIBS ${NETWORKEDITORGUI_PREFIX}/lib/plugins/*.dylib)
		set(CMAKE_MACOSX_RPATH true)
		set(CMAKE_BUILD_RPATH "${CMAKE_BINARY_DIR}/src;${CMAKE_BINARY_DIR}/src/dependencies")
		set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH} "@loader_path;@loader_path/../Frameworks;@loader_path/../Frameworks/dependencies")

		## set C/C++ compiler flags
    		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -Wno-inline")
    		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fPIC -std=gnu++11")
	else ()
		file(GLOB NETWORKEDITORGUI_LIBS ${NETWORKEDITORGUI_PREFIX}/lib/*.so)
		file(GLOB NETWORKEDITORGUI_PLUGINS_LIBS ${NETWORKEDITORGUI_PREFIX}/lib/plugins/*.so)
		set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH} "$ORIGIN;;$ORIGIN/../lib/dependencies")
		## set C/C++ compiler flags
    		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -Wno-inline")
    		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fPIC -std=gnu++11 -Wl,--disable-new-dtags")
	endif()

	## set RPATH features
	set(CMAKE_SKIP_BUILD_RPATH FALSE)
	set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
	set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif ()

set(NETWORKEDITORGUI_INCLUDEDIR ${NETWORKEDITORGUI_PREFIX}/include)
set(NETWORKEDITORGUI_PLUGINS_INCLUDEDIR ${NETWORKEDITORGUI_PREFIX}/include/plugins)

# find Qt5 and required components
find_package(Qt5 COMPONENTS Core Gui Widgets DBus PrintSupport REQUIRED)

## Set build type default.
set(CMAKE_BUILD_TYPE "Release" CACHE STRING
  "Choose the type of build to perform. The options are: Debug, Release, RelWithDebInfo, MinSizeRel.")
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" FORCE)
endif()

add_subdirectory(src)