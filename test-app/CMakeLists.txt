cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(alcuintestapp)
set(PROJECT_NAME_TITLE "AlcuinTestApp")

include(GNUInstallDirs)

if(WIN32)
	## on windows Ws2_32.lib is needed to be included for the dll
    	list(APPEND EXTRA_LIBS "Ws2_32.lib")

	## get alcuin directories
	set(ALCUIN_PREFIX "C:/Program Files/libalcuin" CACHE PATH "alcuin root directory (contains lib, include, etc.)")
	file(GLOB ALCUIN_LIBS ${ALCUIN_PREFIX}/lib/*.lib)
	file(GLOB ALCUIN_DLLS "${ALCUIN_PREFIX}/bin/*.dll")
	file(GLOB ALCUIN_PLUGINS_LIBS ${ALCUIN_PREFIX}/lib/plugins/*.lib)
	file(GLOB ALCUIN_PLUGINS_DLLS "${ALCUIN_PREFIX}/bin/plugins/*.dll")
elseif(UNIX)
	## get alcuin directories
	set(ALCUIN_PREFIX "/usr/local" CACHE PATH "libalcuin root directory (contains lib, include, etc.)")
	
	## set use of RPATH for Mac
	if (APPLE)
		file(GLOB ALCUIN_LIBS ${ALCUIN_PREFIX}/lib/*.dylib)
		file(GLOB ALCUIN_PLUGINS_LIBS ${ALCUIN_PREFIX}/lib/plugins/*.dylib)
		set(CMAKE_MACOSX_RPATH true)
		set(CMAKE_BUILD_RPATH "${CMAKE_BINARY_DIR}/src;${CMAKE_BINARY_DIR}/src/dependencies")
		set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH} "@loader_path;@loader_path/../Frameworks;@loader_path/../Frameworks/dependencies")

		## set C/C++ compiler flags
    		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -Wno-inline")
    		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fPIC -std=gnu++11")
	else ()
		file(GLOB ALCUIN_LIBS ${ALCUIN_PREFIX}/lib/*.so)
		file(GLOB ALCUIN_PLUGINS_LIBS ${ALCUIN_PREFIX}/lib/plugins/*.so)
		set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH} "$ORIGIN;$ORIGIN/../lib/dependencies")
		## set C/C++ compiler flags
    		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -Wno-inline")
    		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fPIC -std=gnu++11 -Wl,--disable-new-dtags")
	endif()

	## set RPATH features
	set(CMAKE_SKIP_BUILD_RPATH FALSE)
	set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
	set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif ()

set(ALCUIN_INCLUDEDIR ${ALCUIN_PREFIX}/include)
set(ALCUIN_PLUGINS_INCLUDEDIR ${ALCUIN_PREFIX}/include/plugins)

# find Qt5 and required components
find_package(Qt5 COMPONENTS Core Gui Widgets DBus PrintSupport Svg REQUIRED)

## Set build type default.
set(CMAKE_BUILD_TYPE "Release" CACHE STRING
  "Choose the type of build to perform. The options are: Debug, Release, RelWithDebInfo, MinSizeRel.")
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" FORCE)
endif()

## Determine the version
set(Alcuin_VERSION_MAJOR 0)
set(Alcuin_VERSION_MINOR 0)
set(Alcuin_VERSION_PATCH 0)
set(Alcuin_VERSION_RELEASE 0)
set(Alcuin_DOTTED_VERSION 0.0.0)
if(EXISTS "${ALCUIN_PREFIX}/share/doc/alcuin/VERSION.txt")

	file(STRINGS "${ALCUIN_PREFIX}/share/doc/alcuin/VERSION.txt" VersionString NEWLINE_CONSUME)

	string(STRIP "${VersionString}" VersionString)

	set(Alcuin_DOTTED_VERSION ${VersionString})

	string(REPLACE "." ";" VersionString "${VersionString}" )
	string(REPLACE "-" ";" VersionString "${VersionString}" )

	list(LENGTH VersionString versionLength)

	list(GET VersionString 0 Alcuin_VERSION_MAJOR)
	list(GET VersionString 1 Alcuin_VERSION_MINOR)
	list(GET VersionString 2 Alcuin_VERSION_PATCH)

	if(${versionLength} GREATER 3)
		list(GET VersionString 3 Alcuin_VERSION_RELEASE )
	endif()

endif()

add_subdirectory(src)

## Package the project
option(BUILD_PACKAGING "Package AlcuinTestApp into binary distributions" OFF)
if (${BUILD_PACKAGING})
	add_subdirectory(packaging)
endif()


