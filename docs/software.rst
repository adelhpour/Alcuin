********
Software
********

NetworkEditorGUI is an open-source portable GUI library that provides developers with an embeddable Qt widget.

How to use
##########

To the Developers
=================

To use NetworkEditorGUI, a developer should follow the following steps:

    * Link their code against NetworkEditorGUI library, located in the ``lib`` or ``bin`` folder (depending on the operating system) under install directory
    * include NetworkEditorGUI header files, located in the ``include`` folder under install directory, in their project
    * create an object of ``MyNetworkEditorWidget`` in their code
    
To the Users
============

To use NetworkEditorGUI, a user should build a test-app from source or install it using the binary installers

Architecture
############

We have tried to write NetworkEditorGUI code in the simplest form possible. In its design, MyNetworkEditorWidget, a customized QFrame class, holds all the other objects and is composed of two main parts, the front-end and the back-end.

Front-end
=========

On the front-end, NetworkEditorGUI consists of the following items:
    
    * An object of MyToolbar, a customized QToolbar class, contating the action buttons,
    * An object of MyGraphicsView, a customized QGraphicsView class, holding the scene,
    * An object of MyGraphicsScene, a customized QGraphicsScene class, displaying the network,
    
Back-end
========
    
On the back-end, NetworkEditorGUI consists of the following items:
    
    * MyInteractor class, a class to control all the use cases,
    
    * Plugins class, an interface that is used by MyInteractor and is inherited by the plugins, including ImportPlugin, DataExportPlugin, PrintExportPlugin, NodeStylePlugin, EdgeStylePlugin, and AutoLayoutPlugin,
    
    * ElementBase class, an interface that is used by MyInteractor and is inherited by the netwrok elements, including Node, Edge, and ArrowHead,
    
    * ElementStyleBase class, an interface that is used by ElementBase derived classes and is inherited by the network element styles, including NodeStyle, EdgeStyle, and ArrowHeadStyle,
    
    * ElementGraphicsItemBase class, an interface that is used by ElementBase derived classes and is inherited by the network element graphics items, including NodeGraphicsItem, EdgeGraphicsItem, and ArrowHeadGraphicsItem,
    
    * ShapeStyleBase class, an interface that is used by ElementStyleBase derived classes and is inherited by the shape styles, including EllipseStyle, RectStyle, PolygonStyle, LineStyle, and TextStyle,
    
    * ShapeGraphicsItemBase class, an interface that is used by ElementGraphicsItemBase derived classes and is inherited by the shape graphics items, including EllipseGraphicsItem, RectGraphicsItem, PolygonGraphicsItem, LineGraphicsItem, and TextGraphicsItem.
    
Communication between front-end and back-end
============================================

In NetworkEditorGUI, all the request and responses are passed on to and from the front-end and back-end through Qt Signals and Slots.


Here, you can see the design elaborating the architecture of the project.
    
    
.. figure:: images/design.pdf
    :width: 700
    :alt: NetworkEditorGUI design
    :align: center

    NetworkEditorGUI design
