*********
Tutorials
*********

Introduction
############

NetworkEditorGUI is both a viewer and editor of graphical networks with nodes, edges, and arrowheads. The network can be either created on a blank canvas, or imported and edited there. The user interface of the NetworkEditorGUI consists of a Toolbar and a GraphicsView with a GraphicsScene. Additionaly, a pop-up feature Menu exists for each network element shown on the GraphicsScene.

Items
#####

Toolbar
=======

The toolbar of the user interface is at the top of the NetworkEditorGUI window. All the actions required to view or edit the network are incoporated into the toolbar. In its simplest form and when no plugin is loaded, the toolbar contain the following actions:

    * The Node action to choose the default node,
    * The Edge action to add the default edge,
    * The Remove action to remove the desired node or edge from the GraphicsScene,
    * The Undo and Redo actions,
    * The Reset action to remove all the elements from the GraphicsScene.

.. figure:: images/toolbar-no-plugin.png
    :width: 700
    :alt: Toolbar
    :align: center

    Screenshot of the toolbar with no plugin
    
By loading plugins into NetworkEditorGUI, it is possible to add actions associated with those plugins into the toolbar. Currently, the following actions related to the plugins are added:

    * The Import action to load an existing network,
    * The Export action to save the network data or figure,
    * The actions to add nodes with different styels,
    * The actions to add edges with different styles and arrowhead styles,
    * The Autolayout action to automatically position the nodes on the scene.
    
.. figure:: images/toolbar-with-plugin.png
    :width: 700
    :alt: Toolbar
    :align: center

    Screenshot of the toolbar with plugins


GraphicsView
============

The graphics view of the user interface is placed under the toolbar. It contains an interactive graphics scene which enables the user to have access to the following features:

    * Display the nodes, edges, and arrowheads on the graphics scene,
    * Add and remove nodes to/from the graphics scene ,
    * Add and remove edges and arrowheads between the nodes to/from the graphics scene,
    * Nest a node inside another node,
    * Modify node, edge, and arrowhead features,
    * Drag and move a node and its connected edges and arrowheads on the graphics scene,
    * Zoom and pan on the graphics scene.

Actions
#######

In the following, some simple examples of how a user can use NetworkEditorGUI to view and edit their network are shown.

Add Node
========

To add a node to the graphcis scene, a user must first click on the Node action in the toolbar and choose their desired node style. Once the node style is chosen, the user must click on the position on the scene to which they want to add the node. The user can click on multiple positions on the scene consecutively to add nodes with the same style.

.. figure:: images/adding_nodes.gif
    :width: 700
    :alt: Toolbar
    :align: center

    Adding nodes

Add Edge
========

Remove Element
==============

Unde/Redo
=========

Reset
=====

Import
======

Export
======

AutoLayout
==========

Zoom and Pan
============

Nest
====
