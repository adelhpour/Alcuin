import networkx as nx
import json

def items_info():
    # circular
    circular = {'name' : "Circular", 'type': "autolayoutengine", 'name-of-call-functions': ["auto_layout"]}
    # planar
    planar = {'name' : "Planar", 'type': "autolayoutengine", 'name-of-call-functions': ["auto_layout"]}
    # kamad_kawai
    kamad_kawai = {'name' : "Kamda-Kawai", 'type': "autolayoutengine", 'name-of-call-functions': ["auto_layout"]}
    # spiral
    spiral = {'name' : "Spiral", 'type': "autolayoutengine", 'name-of-call-functions': ["auto_layout"], 'parameters' : [{'parameter' : "resolution", 'type' : "double", 'min' : 0.01, 'max' : 1.0, 'step' : 0.01, 'default' : 0.35}, {'parameter' : "equidistant", 'type' : "boolean", 'default' : False}]}

    return json.dumps({'items': [circular, planar, kamad_kawai, spiral]})


def auto_layout(input):
    graph_info = json.loads(input[0])
    autolayout_info = json.loads(input[1])
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
    node_pos = {}
    if 'name' in list(autolayout_info.keys()):
        if autolayout_info['name'] == "Circular":
            node_pos = nx.circular_layout(graph, center = pos_center, scale = dim_scale)
        elif autolayout_info['name'] == "Planar":
            node_pos = nx.planar_layout(graph, center = pos_center, scale = dim_scale)
        elif autolayout_info['name'] == "Kamda-Kawai":
            node_pos = nx.kamada_kawai_layout(graph, center = pos_center, scale = dim_scale)
        elif autolayout_info['name'] == "Spiral":
            _resolution = 0.35
            _equidistant = False
            if 'parameters' in list(autolayout_info.keys()):
                for p_index in range(len(autolayout_info['parameters'])):
                    if 'parameter' in list(autolayout_info['parameters'][p_index].keys()) and 'value' in list(autolayout_info['parameters'][p_index].keys()):
                        if autolayout_info['parameters'][p_index]['parameter'] == "resolution" :
                            _resolution = autolayout_info['parameters'][p_index]['value']
                        if autolayout_info['parameters'][p_index]['parameter'] == "equidistant" :
                            _equidistant = autolayout_info['parameters'][p_index]['value']
            node_pos = nx.spiral_layout(graph, center = pos_center, scale = dim_scale, resolution = _resolution, equidistant = _equidistant)
        else:
            node_pos = nx.circular_layout(graph, center = pos_center, scale = dim_scale)

    # write graph info
    if 'nodes' in list(graph_info.keys()):
        for n_index in range(len(graph_info['nodes'])):
            if 'id' in list(graph_info['nodes'][n_index].keys()) and 'position' in list(graph_info['nodes'][n_index].keys()):
                if graph_info['nodes'][n_index]['id'] in list(node_pos.keys()):
                    graph_info['nodes'][n_index]['position']['x'] = node_pos[graph_info['nodes'][n_index]['id']][0]
                    graph_info['nodes'][n_index]['position']['y'] = node_pos[graph_info['nodes'][n_index]['id']][1]

    return (json.dumps(graph_info), )


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