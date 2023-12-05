import networkx as nx
import json

def items_info():
    # circular
    circular = {'name' : "AutoLayout (Circular)", 'call-functions':[
        {'name': "createNetwork", 'inputs': [{'name': "auto_layout_circular", 'api': "python", 'inputs': [{'name': "exportNetworkInfo", 'inputs': []}]}]},
        {'name': "askForAdjustExtentsOfNodes", 'inputs': []},
        {'name': "askForAdjustConnectedEdgesOfNodes", 'inputs': []},
        {'name': "createChangeStageCommand", 'inputs': []},
        {'name': "enableNormalMode", 'inputs': []}]}

    planar = {'name' : "AutoLayout (Planar)", 'call-functions':[
        {'name': "createNetwork", 'inputs': [{'name': "auto_layout_planar", 'api': "python", 'inputs': [{'name': "exportNetworkInfo", 'inputs': []}]}]},
        {'name': "askForAdjustExtentsOfNodes", 'inputs': []},
        {'name': "askForAdjustConnectedEdgesOfNodes", 'inputs': []},
        {'name': "createChangeStageCommand", 'inputs': []},
        {'name': "enableNormalMode", 'inputs': []}]}

    spiral = {'name' : "AutoLayout (Spiral)", 'call-functions':[
        {'name': "createNetwork", 'inputs': [{'name': "auto_layout_spiral", 'api': "python", 'inputs': [{'name': "exportNetworkInfo", 'inputs': []}, 0.35, False]}]},
        {'name': "askForAdjustExtentsOfNodes", 'inputs': []},
        {'name': "askForAdjustConnectedEdgesOfNodes", 'inputs': []},
        {'name': "createChangeStageCommand", 'inputs': []},
        {'name': "enableNormalMode", 'inputs': []}]}

    return json.dumps({'items': [circular, planar, spiral]})

def auto_layout_circular(input):
    graph_info = json.loads(input[0])
    graph = nx.Graph()
    if 'nodes' in list(graph_info.keys()):
        for n_index in range(len(graph_info['nodes'])):
            if 'id' in list(graph_info['nodes'][n_index].keys()) and 'position' in list(graph_info['nodes'][n_index].keys()) and not is_parent(graph_info['nodes'][n_index]) and not is_centroid_node(graph_info['nodes'][n_index]):
                graph.add_node(graph_info['nodes'][n_index]['id'], pos = '{},{}'.format(graph_info['nodes'][n_index]['position']['x'], graph_info['nodes'][n_index]['position']['y']))

    if 'edges' in list(graph_info.keys()):
        for e_index in range(len(graph_info['edges'])):
            if 'id' in list(graph_info['edges'][e_index].keys()) and 'start' in list(graph_info['edges'][e_index].keys()) and 'end' in list(graph_info['edges'][e_index].keys()):
                if 'node' in list(graph_info['edges'][e_index]['start']) and 'node' in list(graph_info['edges'][e_index]['end']):
                    graph.add_edge(graph_info['edges'][e_index]['start']['node'], graph_info['edges'][e_index]['end']['node'])
    pos_center = [0, 0]
    dim_scale = 1.0
    if 'dimensions' in list(graph_info.keys()) and 'width' in list(graph_info['dimensions'].keys()) and 'height' in list(graph_info['dimensions'].keys()):
        dim_scale = 0.5 * min(graph_info['dimensions']['width'], graph_info['dimensions']['height'])
        if 'position' in list(graph_info.keys()) and 'x' in list(graph_info['position'].keys()) and 'y' in list(graph_info['position'].keys()):
            pos_center = [graph_info['position']['x'], graph_info['position']['y']]

    # apply autolayout algorithm
    node_pos = nx.circular_layout(graph, center = pos_center, scale = dim_scale)

    # write graph info
    if 'nodes' in list(graph_info.keys()):
        for n_index in range(len(graph_info['nodes'])):
            if 'id' in list(graph_info['nodes'][n_index].keys()) and 'position' in list(graph_info['nodes'][n_index].keys()):
                if graph_info['nodes'][n_index]['id'] in list(node_pos.keys()):
                    graph_info['nodes'][n_index]['position']['x'] = node_pos[graph_info['nodes'][n_index]['id']][0]
                    graph_info['nodes'][n_index]['position']['y'] = node_pos[graph_info['nodes'][n_index]['id']][1]

    return json.dumps(graph_info)

def auto_layout_circular(input):
    graph_info = json.loads(input[0])
    return json.dumps(update_graph_info_node_positions(graph_info, nx.circular_layout(get_graph(graph_info),
                                                                                      center = get_pos_center(graph_info),
                                                                                      scale = get_dim_scale(graph_info))))

def auto_layout_planar(input):
    graph_info = json.loads(input[0])
    return json.dumps(update_graph_info_node_positions(graph_info, nx.planar_layout(get_graph(graph_info),
                                                                                      center = get_pos_center(graph_info),
                                                                                      scale = get_dim_scale(graph_info))))

def auto_layout_spiral(input):
    print("in the spiral")
    print("size is: ", len(input))
    graph_info = json.loads(input[0])
    resolution = input[1]
    equidistant = input[2]
    print("size is: ", resolution)
    print("size is: ", equidistant)
    return json.dumps(update_graph_info_node_positions(graph_info, nx.spiral_layout(get_graph(graph_info),
                                                                                    center = get_pos_center(graph_info),
                                                                                    scale = get_dim_scale(graph_info),
                                                                                    resolution = resolution,
                                                                                    equidistant = equidistant)))

def get_graph(graph_info):
    graph = nx.Graph()
    if 'nodes' in list(graph_info.keys()):
        for n_index in range(len(graph_info['nodes'])):
            if 'id' in list(graph_info['nodes'][n_index].keys()) and 'position' in list(graph_info['nodes'][n_index].keys()) and not is_parent(graph_info['nodes'][n_index]) and not is_centroid_node(graph_info['nodes'][n_index]):
                graph.add_node(graph_info['nodes'][n_index]['id'], pos = '{},{}'.format(graph_info['nodes'][n_index]['position']['x'], graph_info['nodes'][n_index]['position']['y']))
    if 'edges' in list(graph_info.keys()):
        for e_index in range(len(graph_info['edges'])):
            if 'id' in list(graph_info['edges'][e_index].keys()) and 'start' in list(graph_info['edges'][e_index].keys()) and 'end' in list(graph_info['edges'][e_index].keys()):
                if 'node' in list(graph_info['edges'][e_index]['start']) and 'node' in list(graph_info['edges'][e_index]['end']):
                    graph.add_edge(graph_info['edges'][e_index]['start']['node'], graph_info['edges'][e_index]['end']['node'])

    return graph

def get_pos_center(graph_info):
    if 'position' in list(graph_info.keys()) and 'x' in list(graph_info['position'].keys()) and 'y' in list(graph_info['position'].keys()):
        return [graph_info['position']['x'], graph_info['position']['y']]

    return [0, 0]

def get_dim_scale(graph_info):
    if 'dimensions' in list(graph_info.keys()) and 'width' in list(graph_info['dimensions'].keys()) and 'height' in list(graph_info['dimensions'].keys()):
        return 0.5 * min(graph_info['dimensions']['width'], graph_info['dimensions']['height'])

    return 1.0

def update_graph_info_node_positions(graph_info, node_pos):
    if 'nodes' in list(graph_info.keys()):
        for n_index in range(len(graph_info['nodes'])):
            if 'id' in list(graph_info['nodes'][n_index].keys()) and 'position' in list(graph_info['nodes'][n_index].keys()):
                if graph_info['nodes'][n_index]['id'] in list(node_pos.keys()):
                    graph_info['nodes'][n_index]['position']['x'] = node_pos[graph_info['nodes'][n_index]['id']][0]
                    graph_info['nodes'][n_index]['position']['y'] = node_pos[graph_info['nodes'][n_index]['id']][1]

    return graph_info

def is_parent(node):
    if "style" in list(node.keys()) and "convertible-parent-category" in list(node['style'].keys()) and node['style']['convertible-parent-category']:
        return True

    return False


def is_centroid_node(node):
    if 'style' in list(node.keys()) and 'shapes' in list(node['style']):
        for shape in node['style']['shapes']:
            if 'shape' in list(shape.keys()) and shape['shape'] == "centroid":
                return True

    return False