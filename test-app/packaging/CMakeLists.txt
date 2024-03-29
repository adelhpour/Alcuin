# set CPack variables
set(CPACK_PACKAGE_VERSION_MAJOR ${Alcuin_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${Alcuin_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${Alcuin_VERSION_PATCH})

set(CPACK_PACKAGE_NAME "Alcuin Test App")
set(CPACK_PACKAGE_FILE_NAME "AlcuinTestApp${Alcuin_VERSION_MAJOR}.${Alcuin_VERSION_MINOR}.${Alcuin_VERSION_PATCH}Setup")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A Test Application for the Graphical User Interface of the Alcuin Project")
set(CPACK_PACKAGE_VENDOR "Herbert Sauro's Sys-Bio Lab")

set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN TRUE)
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED TRUE)
set(CPACK_COMPONENTS_ALL alcuin_test_app)
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_VERBATIM_VARIABLES TRUE)
#set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
#set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_GENERATOR IFW)
set(CPACK_IFW_VERBOSE ON)

if (WIN32)
    set(CPACK_IFW_TARGET_DIRECTORY "C:/Program Files/${CPACK_PACKAGE_FILE_NAME}")
    set(CPACK_IFW_PACKAGE_START_MENU_DIRECTORY CPACK_PACKAGE_NAME)
elseif (APPLE)
    set(CPACK_IFW_TARGET_DIRECTORY "/usr/bin")
endif ()

find_program(CPACK_IFW_BINARYCREATOR_EXECUTABLE
        NAMES binarycreator binarycreator.exe
        REQUIRED
        COMMENT "absolute path to the binarycreator executable. If you do not have binarycreator you will need to install Qt Installer Framework")

include(CPack REQUIRED)
include(CPackIFW REQUIRED)

cpack_add_component(
        alcuin_test_app
        DISPLAY_NAME "Application"
        DESCRIPTION "A Test Application for the Graphical User Interface of the Alcuin Project"
)

cpack_ifw_configure_component(
        alcuin_test_app
        DEFAULT TRUE)

if (WIN32)
    configure_file(inno_setup.iss.in inno_setup.iss)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/inno_setup.iss DESTINATION ${CMAKE_INSTALL_PREFIX}/packaging COMPONENT alcuin_lib)
endif ()