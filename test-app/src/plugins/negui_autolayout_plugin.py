import networkx as nx
import json
import alcuin_plugin

# circular
circular_export_network_info = alcuin_plugin.CPlusPlusCommand("exportNetworkInfo")
circular_auto_layout_function = alcuin_plugin.PythonCommand("auto_layout_circular")
circular_auto_layout_function.add_parameter(circular_export_network_info)
circular_create_network = alcuin_plugin.CPlusPlusCommand("createNetwork")
circular_create_network.add_parameter(circular_auto_layout_function)
circular_ask_for_adjust_extents_of_nodes = alcuin_plugin.CPlusPlusCommand("askForAdjustExtentsOfNodes")
circular_ask_for_adjust_connected_edges_of_nodes = alcuin_plugin.CPlusPlusCommand("askForAdjustConnectedEdgesOfNodes")
circular_create_change_stage_command = alcuin_plugin.CPlusPlusCommand("createChangeStageCommand")
circular_enable_normal_mode = alcuin_plugin.CPlusPlusCommand("enableNormalMode")
circular_autolayout_plugin = alcuin_plugin.CommandPlugin(plugin_name="AutoLayout (Circular)",
                                                         commands=[circular_create_network,
                                                                   circular_ask_for_adjust_extents_of_nodes,
                                                                   circular_ask_for_adjust_connected_edges_of_nodes,
                                                                   circular_create_change_stage_command,
                                                                   circular_enable_normal_mode])
alcuin_plugin.add_plugin(circular_autolayout_plugin)

# planar
planar_export_network_info = alcuin_plugin.CPlusPlusCommand("exportNetworkInfo")
planar_auto_layout_function = alcuin_plugin.PythonCommand("auto_layout_planar")
planar_auto_layout_function.add_parameter(planar_export_network_info)
planar_create_network = alcuin_plugin.CPlusPlusCommand("createNetwork")
planar_create_network.add_parameter(planar_auto_layout_function)
planar_ask_for_adjust_extents_of_nodes = alcuin_plugin.CPlusPlusCommand("askForAdjustExtentsOfNodes")
planar_ask_for_adjust_connected_edges_of_nodes = alcuin_plugin.CPlusPlusCommand("askForAdjustConnectedEdgesOfNodes")
planar_create_change_stage_command = alcuin_plugin.CPlusPlusCommand("createChangeStageCommand")
planar_enable_normal_mode = alcuin_plugin.CPlusPlusCommand("enableNormalMode")
planar_autolayout_plugin = alcuin_plugin.CommandPlugin(plugin_name="AutoLayout (Planar)",
                                                       commands=[planar_create_network,
                                                                 planar_ask_for_adjust_extents_of_nodes,
                                                                 planar_ask_for_adjust_connected_edges_of_nodes,
                                                                 planar_create_change_stage_command,
                                                                 planar_enable_normal_mode])
alcuin_plugin.add_plugin(planar_autolayout_plugin)

# spiral
spiral_export_network_info = alcuin_plugin.CPlusPlusCommand("exportNetworkInfo")
spiral_auto_layout_function = alcuin_plugin.PythonCommand("auto_layout_spiral")
spiral_auto_layout_function.add_parameter(spiral_export_network_info)
spiral_create_network = alcuin_plugin.CPlusPlusCommand("createNetwork")
spiral_create_network.add_parameter(spiral_auto_layout_function)
spiral_ask_for_adjust_extents_of_nodes = alcuin_plugin.CPlusPlusCommand("askForAdjustExtentsOfNodes")
spiral_ask_for_adjust_connected_edges_of_nodes = alcuin_plugin.CPlusPlusCommand("askForAdjustConnectedEdgesOfNodes")
spiral_create_change_stage_command = alcuin_plugin.CPlusPlusCommand("createChangeStageCommand")
spiral_enable_normal_mode = alcuin_plugin.CPlusPlusCommand("enableNormalMode")
spiral_autolayout_plugin = alcuin_plugin.CommandPlugin(plugin_name="AutoLayout (Spiral)",
                                                       commands=[spiral_create_network,
                                                                 spiral_ask_for_adjust_extents_of_nodes,
                                                                 spiral_ask_for_adjust_connected_edges_of_nodes,
                                                                 spiral_create_change_stage_command,
                                                                 spiral_enable_normal_mode])
alcuin_plugin.add_plugin(spiral_autolayout_plugin)


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
    graph_info = json.loads(input[0])
    resolution = input[1]
    equidistant = input[2]
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
