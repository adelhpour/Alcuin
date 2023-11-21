import json
import sbmlplot

def items_info():
    # json
    json_format = {'name' : "as JSON", 'type': "dataexporttool", 'file-extension' : "json", 'default-save-file-name' : "network", 'element-types': {'node': ["Reaction", "Compartment"], 'edge': ["Reactant", "Product", "Modifier"]}}

    # sbml
    sbml_format = {'name' : "as SBML", 'type': "dataexporttool", 'file-extension' : "xml", 'default-save-file-name' : "model", 'element-types': {'node': ["Reaction"], 'edge': ["Reactant", "Product", "Modifier"]}}

    return json.dumps({'items': [json_format, sbml_format]})


def checkForGraphInfoCompatibility(input):
    graph_info = json.loads(input[0])
    file_type = input[1]
    # json
    if file_type == "as JSON":
        return checkForJSONCompatibility()
    # sbml
    elif file_type == "as SBML":
        return checkForSBMLCompatibility(graph_info)

    return ({'isInfoCompatible': False, 'messages': [{'message': "export format is not supported"}]},)


def checkForJSONCompatibility():
    return (json.dumps({'isInfoCompatible': True}),)


def checkForSBMLCompatibility(graph_info):
    all_species_have_parents = True
    for n_index in range(len(graph_info['nodes'])):
        if graph_info['nodes'][n_index]['style']['category'] == "Species":
            if 'parent' not in list(graph_info['nodes'][n_index].keys()) \
                    or graph_info['nodes'][n_index]['parent'] == "N/A":
                all_species_have_parents = False

    messages = []
    if not all_species_have_parents:
        messages.append({'message': "Some Species in the network do not belong to any Compartments."})

    is_info_compatible = all_species_have_parents
    return (json.dumps({'isInfoCompatible': is_info_compatible, 'messages': messages}),)

def writeGraphInfoToFile(input):
    graph_info = json.loads(input[0])
    file_type = input[1]
    file_name = input[2]

    # json
    if file_type == "as JSON":
        return writeJSON(graph_info, file_name)
    # sbml
    elif file_type == "as SBML":
        return writeSBML(graph_info, file_name)


def writeJSON(graph_info, file_name):
    with open(file_name, 'w', encoding='utf8') as js_file:
        json.dump(graph_info, js_file, indent=1)


def writeSBML(graph_info, file_name):
    sbml_graph_info = sbmlplot.SBMLGraphInfoImportFromNetworkEditor()
    sbml_graph_info.extract_info(graph_info)
    sbml_export = sbmlplot.SBMLGraphInfoExportToSBMLModel()
    sbml_export.extract_graph_info(sbml_graph_info)
    sbml_export.export(file_name)

"""
# keep commented till using the new version of sbmlplot
def writeSBML(network_info, file_name):
    network_info_import_from_network_editor = sbmlplot.NetworkInfoImportFromNetworkEditor()
    network_info_import_from_network_editor.extract_info(network_info)
    network_info_export_to_sbml_model = sbmlplot.NetworkInfoExportToSBMLModel()
    network_info_export_to_sbml_model.extract_graph_info(network_info_import_from_network_editor)
    network_info_export_to_sbml_model.export(file_name)
    
"""
