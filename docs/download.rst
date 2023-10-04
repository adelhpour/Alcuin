********************
Download and Install
********************

Build from Source
#################

Portable Library
================

Alcuin can be built from its source code using the following steps:

* Create a directory for your entire Alcuin project. This directory should be placed in somewhere not admin-blocked and is recommended to have the following file structure:

    .. code-block::

        <root directory>
        ├── build
        ├── install
        ├── source
        
*  Clone or download `Alcuin source <https://github.com/adelhpour/Alcuin>`_ to `<root directory>/source`.

*  `Download <https://cmake.org/download/>`_ and Install CMake and then open CMake-gui.

    *  Select ``<root directory>/source/alcuin`` (where the top-level ``CMakeLists.txt`` is located) as the source directory.

    *  Select ``<root directory>/build`` as the build (binary) directory.
    
    *  Set ``CMAKE_INSTALL_PREFIX`` to ``<root directory>/install``.

    *  Click "Configure" button and choose your compiler.

    *  At this step, CMake requires you to set the value of ``Qt_DIR`` to the directory where ``QtConfig.cmake`` and other Qt dependencies are located.
    
        .. note::

            To build Alcuin from source, Qt development tools are needed to be already installed on your machine.
    
    *  Click "Configure" button time and again until no red variable row remains.
    
    *  Click "Generate" button.
    
*  Now that you have figured out all the configuration settings, all you need is building the project as follows:

    *  On Windows, you can either click on "Open Project" button on CMake-gui or open the generated ``.sln`` file in ``<root directory>/build`` in Visual Studio. Once Visual Studio is launched, change the configuration to "Release", and build ``ALL-BUILD`` and ``INSTALL``.
    
    *  On macOS and Linux, simply go to ``<root directory>/build`` and run ``make`` and then ``make install`` on terminal.
    
Application
===========

An application consisting of a window of Alcuin widget can be built from its source code. To build it, a developer should follow similar steps described above for building the portable library except that ``<root directory>/source/alcuin/test-app`` must be selected as the source directory in CMake-gui.


Download Binary Installers
##########################

Alcuin runs on:

* Microsoft Windows,
* macOS,
* Linux.

Installation with precompiled binaries is provided for users who intend to use Alcuin without changing its source code. Using Qt Installer Framework, we have created installers for Microsoft Windows (.exe), macOS (.dmg), and Linux (.run) platforms, which are available in the `Releases page <https://github.com/adelhpour/Alcuin/releases>`_ of the repository on GitHub.
