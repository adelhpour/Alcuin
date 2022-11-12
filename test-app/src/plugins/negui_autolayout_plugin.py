import networkx as nx
import json
    
def autoLayoutInfo():
    # circular
    circular = {'name' : "Circular", 'type': "autolayoutengine"}
    # planar
    planar = {'name' : "Planar", 'type': "autolayoutengine"}
    # kamad_kawai
    kamad_kawai = {'name' : "Kamda-Kawai", 'type': "autolayoutengine"}
    # spiral
    spiral = {'name' : "Spiral", 'type': "autolayoutengine", 'parameters' : [{'parameter' : "resolution", 'type' : "double", 'min' : 0.01, 'max' : 1.0, 'step' : 0.01, 'default' : 0.35}, {'parameter' : "equidistant", 'type' : "boolean", 'default' : False}]}
    
    return json.dumps({'items': [circular, planar, kamad_kawai, spiral]})
        
    
def autoLayout(graphInfoString, autoLayoutInfoString):
    # read graph info
    graphInfo = json.loads(graphInfoString)
    autoLayoutInfo = json.loads(autoLayoutInfoString)
    graph = nx.Graph()
    if 'nodes' in list(graphInfo.keys()):
        for n_index in range(len(graphInfo['nodes'])):
            if 'id' in list(graphInfo['nodes'][n_index].keys()) and 'position' in list(graphInfo['nodes'][n_index].keys()) and ('parent' not in list(graphInfo['nodes'][n_index].keys()) or ('parent' in list(graphInfo['nodes'][n_index].keys()) and graphInfo['nodes'][n_index]['parent'] != "N/A")):
                graph.add_node(graphInfo['nodes'][n_index]['id'], pos = '{},{}'.format(graphInfo['nodes'][n_index]['position']['x'],     graphInfo['nodes'][n_index]['position']['y']))
    
    if 'edges' in list(graphInfo.keys()):
        for e_index in range(len(graphInfo['edges'])):
            if 'id' in list(graphInfo['edges'][e_index].keys()) and 'start' in list(graphInfo['edges'][e_index].keys()) and 'end' in list(graphInfo['edges'][e_index].keys()):
                if 'node' in list(graphInfo['edges'][e_index]['start']) and 'node' in list(graphInfo['edges'][e_index]['end']):
                    graph.add_edge(graphInfo['edges'][e_index]['start']['node'], graphInfo['edges'][e_index]['end']['node'])
    pos_center = [0, 0]
    dim_scale = 1.0
    if 'dimensions' in list(graphInfo.keys()) and 'width' in list(graphInfo['dimensions'].keys()) and 'height' in list(graphInfo['dimensions'].keys()):
        dim_scale = 0.5 * min(graphInfo['dimensions']['width'], graphInfo['dimensions']['height'])
        if 'position' in list(graphInfo.keys()) and 'x' in list(graphInfo['position'].keys()) and 'y' in list(graphInfo['position'].keys()):
            pos_center = [graphInfo['position']['x'] + 0.5 * graphInfo['dimensions']['width'], graphInfo['position']['y'] + 0.5 * graphInfo['dimensions']['height']]
    
    # apply autolayout algorithm
    node_pos = {}
    if 'name' in list(autoLayoutInfo.keys()):
        if autoLayoutInfo['name'] == "Circular":
            node_pos = nx.circular_layout(graph, center = pos_center, scale = dim_scale)
        elif autoLayoutInfo['name'] == "Planar":
            node_pos = nx.planar_layout(graph, center = pos_center, scale = dim_scale)
        elif autoLayoutInfo['name'] == "Kamda-Kawai":
            node_pos = nx.kamada_kawai_layout(graph, center = pos_center, scale = dim_scale)
        elif autoLayoutInfo['name'] == "Spiral":
            _resolution = 0.35
            _equidistant = False
            if 'parameters' in list(autoLayoutInfo.keys()):
                for p_index in range(len(autoLayoutInfo['parameters'])):
                    if 'parameter' in list(autoLayoutInfo['parameters'][p_index].keys()) and 'value' in list(autoLayoutInfo['parameters'][p_index].keys()):
                        if autoLayoutInfo['parameters'][p_index]['parameter'] == "resolution" :
                            _resolution = autoLayoutInfo['parameters'][p_index]['value']
                        if autoLayoutInfo['parameters'][p_index]['parameter'] == "equidistant" :
                            _equidistant = autoLayoutInfo['parameters'][p_index]['value']
            node_pos = nx.spiral_layout(graph, center = pos_center, scale = dim_scale, resolution = _resolution, equidistant = _equidistant)
        else:
            node_pos = nx.circular_layout(graph, center = pos_center, scale = dim_scale)
    
    # write graph info
    if 'nodes' in list(graphInfo.keys()):
        for n_index in range(len(graphInfo['nodes'])):
            if 'id' in list(graphInfo['nodes'][n_index].keys()) and 'position' in list(graphInfo['nodes'][n_index].keys()):
                if graphInfo['nodes'][n_index]['id'] in list(node_pos.keys()):
                    graphInfo['nodes'][n_index]['position']['x'] = node_pos[graphInfo['nodes'][n_index]['id']][0]
                    graphInfo['nodes'][n_index]['position']['y'] = node_pos[graphInfo['nodes'][n_index]['id']][1]
    
    return json.dumps(graphInfo)
