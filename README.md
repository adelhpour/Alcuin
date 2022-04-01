# NetworkEditorGUI

NetwrokEditorGUI is a portable GUI library, based on Qt/C++, that provides developers with the basic features required for a reaction network editor software tool. These features are:

* Add/Remove Nodes to the scene,
* Add/Remove Edges between two nodes on the scene,
* Drag and Move a node and its connected edges on the scene,
* Undo/Redo actions,
* Zoom and Pan on the scene,
* Export a .pdf figure of the scene.

## How to Use

The steps to build NetwrokEditorGUI from its source code are as follows:
* Create a directory for your entire NetwrokEditorGUI project. This directory has to be placed in somewhere not admin-blocked and is recommended to have the following file structure:

```
<root directory>
├── build
├── install
```

* Clone or download <a href="https://github.com/adelhpour/NetworkEditorGUI/">NetwrokEditorGUI source</a> to the `<root directory>`.

* <a href="https://cmake.org/download/">Download</a> and Install CMake and then open CMake-gui.

    + Select `<root directory>/NetworkEditorGUI-main` (where the top-level `CMakeLists.txt` is located) as the source directory.
    
    + Select `<root directory>/build` as the build (binary) directory.
    
    + Set `CMAKE_INSTALL_PREFIX` to `<root directory>/install`.

    + Click "Configure" button and choose your compiler.
    
    + At this stage, CMake requires you to set the value of `Qt5_DIR` to the directory where `Qt5Config.cmake` and other Qt dependencies are located.
    
        - ******NOTE******: To build NetworkEditorGUI, Qt5 development tools are needed to be already installed on your machine.
        
    + Click "Configure" button time and again until all red variable rows turn into white.

    + Click "Generate" button.
    
* Now that you have figured out all the configuration settings, all you need is building the project as follows:

    + On Windows, you can either click on "Open Project" button on CMake-gui or open the generated `.sln` file in  `<root directory>/build` in Visual Studio.  Once the Visual Studio is launched, change the configuration to "Release", and build `ALL-BUILD` and `INSTALL`.

    + On Linux and macOS, simply go to `<root directory>/build` and  run `make` and then `make install` on terminal.

## How to Use
To use NetwrokEditorGUI, you first need to:

* [Build](#how-to-build) NetwrokEditorGUI.

* Add NetwrokEditorGUI include directory (`<root directory>/install/include/src`) to the "include directory" of your project.

* Add NetwrokEditorGUI library (in `<root directory>/install/lib` ) to the "dependencies" of your project.

Then, include the following header in your script:
```
    #include "negui_mainwindow.h"
```

### A Simple Example
A simple example of using NetwrokEditorGUI to create a GUI is in /example directory of the project. The steps to build it are all the same as the ones for [building NetwrokEditorGUI](#how-to-build) except that you need to set `NETWORKEDITORGUI_PREFIX` to directory where NetwrokEditorGUI is installed (`<root directory>/install`).

Here is a preview of the example GUI:

<img src="https://github.com/adelhpout/NetworkEditorGUI/blob/main/docs/Figures/GUIpreview.gif" width="350" height="450">

## License
NetwrokEditorGUI is licensed under <a href="https://github.com/adelhpour/NetworkEditorGUI/blob/main/LICENSE">The MIT License</a>.
