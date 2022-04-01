cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(networkeditorguitest)

include(GNUInstallDirs)

if(WIN32)
	## on windows Ws2_32.lib is needed to be included for the dll
    	list(APPEND EXTRA_LIBS "Ws2_32.lib")
elseif(UNIX)
	## set use of RPATH for Mac
	if (APPLE)
		set(CMAKE_MACOSX_RPATH true)
		## set C/C++ compiler flags
    		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -Wno-inline")
    		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fPIC -std=gnu++11")
	else ()
		## set C/C++ compiler flags
    		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -Wno-inline")
    		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fPIC -std=gnu++11 -Wl,--disable-new-dtags")
	endif()

	## set RPATH features
	set(CMAKE_SKIP_BUILD_RPATH FALSE)
	set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
	set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif ()

## get networkeditorgui directories
if (WIN32)
	set(NETWORKEDITORGUI_PREFIX "C:/Program Files/libnetworkeditorgui" CACHE PATH "networkeditorGUI root directory (contains lib, include, etc.)")
	file(GLOB NETWORKEDITORGUI_LIBS ${NETWORKEDITORGUI_PREFIX}/lib/*.lib)
	file(GLOB NETWORKEDITORGUI_DLLS "${NETWORKEDITORGUI_PREFIX}/bin/*.dll")
elseif(UNIX)
	set(NETWORKEDITORGUI_PREFIX "/usr/local" CACHE PATH "libnetworkeditorGUI root directory (contains lib, include, etc.)")
	file(GLOB NETWORKEDITORGUI_LIBS ${NETWORKEDITORGUI_PREFIX}/lib/libnetworkeditorgui.*)
endif()

set(NETWORKEDITORGUI_INCLUDEDIR ${NETWORKEDITORGUI_PREFIX}/include/src)

# find Qt5 and required components
find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport REQUIRED)

## Set build type default.
set(CMAKE_BUILD_TYPE "Release" CACHE STRING
  "Choose the type of build to perform. The options are: Debug, Release, RelWithDebInfo, MinSizeRel.")
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" FORCE)
endif()

add_subdirectory(src)