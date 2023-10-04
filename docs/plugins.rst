*******
Plugins
*******

Introduction
############

Alcuin allows the developers to extend and costumize its features through plugins written in Python. Through this capability, with minimum effort, a developer can turn Alcuin, providing the basic requirements for a network editor tool, into their desired network editor software tool.

How to write a plugin
#####################



Default Plugins
###############

Alcuin is shipped with some default plugins which are considered to be applicable to many network editor software tools. Here is a list of the default plugins shipped alongside Alcuin:

Import Plugin
=============

The Import Plugin is used to import an already exisiting network into Alcuin. A developer can define a new import file format through a Python script. In addition to the information like file-format and file-extension, the developer's code must include functions converting the network information from its original format into the JSON fromat, readable by Alcuin.
    Here is an example of a Python script defining a new import format:

    .. code-block:: Python

        import json
        
        def importInfo():
            format = {'name' : "JSON", 'file-extension' : "json"}
    
            return json.dumps({'items': [format]})
            
        def readGraphInfoFromFile(filename, filetype):
            if filetype == "JSON":
                f = open(filename)
                
            return json.dumps(json.load(f))

Data Export Plugin
==================

The Data Export Plugin is used to export the information of the network displayed on the scene into a file with a specific file format. A developer can define a new data export file format through a Python script. In addition to the information like file-format and file-extension, default-save-file-name, and element-types, the developer's code must include functions converting the network information from the JSON fromat, readable by Alcuin, into the format in which they want it to be stored. Here is an example of a Python script defining a new data export format:

    .. code-block:: Python

        import json
    
        def dataExportInfo():
            format = {'name' : "as JSON", 'file-extension' : "json",\
            'default-save-file-name' : "Graph", 'element-types':\
            {'node': ["NodeType1", "NodeType2"],\
            'edge': ["EdgeType1", "EdgeType2", "EdgeType3"]}}
    
            return json.dumps({'items': [format]})
            
        def writeGraphInfoToFile(graphInfoString, filename, filetype):
            graphInfo = json.loads(graphInfoString)
            if filetype == "as JSON":
                with open(filename, 'w', encoding='utf8') as js_file:
                json.dump(graphInfo, js_file, indent=1)
            
        

Print Export Plugin
===================

The Print Export Plugin is used to save a figure of the network displayed on the scene into a specific figure format. A developer can define a new print export file format through a Python script. The developer's code must include information on figure-format, file-extension, and default-save -file-name. Here is an example of a Python script defining a new print export format:

    .. code-block:: Python

        import json

        def printExportInfo():
            pdfformat = {'name' : "as PDF", 'file-extension' : "pdf",\
            'default-save-file-name' : "Drawing"}
            
            return json.dumps({'items': [pdfformat]})

Node Style Plugin
=================

The Node Style Plugin is used to define a collection of node styles to be used by a user to create their desired network. A preview of each node style is shown on the menu of the Node action in the toolbar.A developer can define a new node style through a Python script. The developer's code must include information on node style name and shapes, defining the features of the shape, such as fill, stroke, stroke-width and other exclusive features. Here is an example of a Python script defining a new node style:

    .. code-block:: Python
    
        import json

        def nodeStylesInfo():
            style = {'name' : "Simple-Rectangle", 'shapes' :\
                [{'shape' : "rect", 'fill' : "white", 'stroke' : "black", 'stroke-width' : 2,\
                'x' : 0.0, 'y' : 0.0, 'width' : 35.0, 'height' : 25.0}]}
            
            return json.dumps({'items': [style]})

Edge Style Plugin
=================

The Edge Style Plugin is used to define a collection of edge and arrow head styles to be used by a user to create their desired network. A preview of each edge and arrow head style is shown on the menu of the Edge action in the toolbar.A developer can define a new edge style through a Python script. The developer's code must include information on edge and arrow head style name and shapes, defining the features of the shape, such as fill, stroke, stroke-width and other exclusive features. Here is an example of a Python script defining a new edge style:

    .. code-block:: Python

        import json

        def edgeStylesInfo():
            style = {'name' : "Line-Triangular-Head",\
            'shapes' : [{'shape' : "line", 'stroke' : "black", 'stroke-width' : 2}],\
            'arrow-head' : {'shapes' : [{'shape' : "polygon", 'fill' : "green",\
            'stroke' : "black", 'stroke-width' : 2}]}}
            
            return json.dumps({'items': [style]})

AutoLayout Plugin
=================

The AutoLayout Plugin is used to autmatically set the position of the network elements. A developer can define a new autolayout engine through a Python script. In addition to the information like engine name and required parameters, needed to be asked from the user, the developer's code must include functions that read the information of the network, enter it into the autolayout engine, apply the autolayout algorithm of the engine, and read the updated network information from the autolayout engine. Here is an example of a Python script defining a new autolayout engine and how it is used to set the position of the network elements automatically:

    .. code-block:: Python

        import json
    
        def autoLayoutInfo():
            engine = {'name' : "Spiral",\
            'parameters' : [{'parameter' : "resolution", 'type' : "double",\
            'min' : 0.01, 'max' : 1.0, 'step' : 0.01, 'default' : 0.35},\
            {'parameter' : "equidistant", 'type' : "boolean", 'default' : False}]}
            
            return json.dumps({'items': [engine]})
            
        def autoLayout(graphInfoString, autoLayoutInfoString):
            # here, the developer must define how using the spiral autolayout engine,
            # the network elements must be positioned.
            
            return json.dumps(graphInfo)
