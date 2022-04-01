cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(networkeditorgui)
add_definitions(-DMAKE_NEGUI_LIB=true)

include(GNUInstallDirs)

## Determine the version
set(NetworkEditorGUI_VERSION_MAJOR)
set(NetworkEditorGUI_VERSION_MINOR)
set(NetworkEditorGUI_VERSION_PATCH)
set(NetworkEditorGUI_VERSION_RELEASE)
set(NetworkEditorGUI_DOTTED_VERSION)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION.txt")

    file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION.txt" VersionString NEWLINE_CONSUME)

    string(STRIP "${VersionString}" VersionString)

    set(NetworkEditorGUI_DOTTED_VERSION ${VersionString})

    string(REPLACE "." ";" VersionString "${VersionString}" )
    string(REPLACE "-" ";" VersionString "${VersionString}" )

    list(LENGTH VersionString versionLength)

    list(GET VersionString 0 NetworkEditorGUI_VERSION_MAJOR )
    list(GET VersionString 1 NetworkEditorGUI_VERSION_MINOR )
    list(GET VersionString 2 NetworkEditorGUI_VERSION_PATCH )

    if(${versionLength} GREATER 3)
        list(GET VersionString 3 NetworkEditorGUI_VERSION_RELEASE )
    endif()

endif()	

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

# find Qt5 and required components
find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport REQUIRED)

## Set build type default.
set(CMAKE_BUILD_TYPE "Release" CACHE STRING
  "Choose the type of build to perform. The options are: Debug, Release, RelWithDebInfo, MinSizeRel.")
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" FORCE)
endif()

add_subdirectory(src)