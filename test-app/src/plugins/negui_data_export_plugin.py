import json
import networkinfotranslator

def items_info():
    # json
    json_format = {'name' : "as JSON",
                   'type': "dataexporttool",
                   'file-extension' : "json",
                   'default-save-file-name' : "network",
                   'call-functions': [
                       {'name': "check_for_json_compatibility", 'input-api-functions': []},
                       {'name': "write_json", 'input-api-functions': []}]}

    # sbml
    sbml_format = {'name' : "as SBML",
                   'type': "dataexporttool",
                   'file-extension' : "xml",
                   'default-save-file-name' : "model",
                   'call-functions': [
                       {'name': "check_for_sbml_compatibility", 'input-api-functions': []},
                       {'name': "write_sbml", 'input-api-functions': []}]}

    return json.dumps({'items': [json_format, sbml_format]})


def check_for_json_compatibility(input):
    return json.dumps({'isInfoCompatible': True})


def check_for_sbml_compatibility(input):
    graph_info = json.loads(input[0])
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
    return json.dumps({'isInfoCompatible': is_info_compatible, 'messages': messages})


def write_json(input):
    graph_info = json.loads(input[0])
    file_name = input[1]
    with open(file_name, 'w', encoding='utf8') as js_file:
        json.dump(graph_info, js_file, indent=1)


def write_sbml(input):
    network_info = json.loads(input[0])
    file_name = input[1]
    network_info_import_from_network_editor = networkinfotranslator.NetworkInfoImportFromNetworkEditor()
    network_info_import_from_network_editor.extract_info(network_info)
    network_info_export_to_sbml_model = networkinfotranslator.NetworkInfoExportToSBMLModel()
    network_info_export_to_sbml_model.extract_graph_info(network_info_import_from_network_editor)
    network_info_export_to_sbml_model.export(file_name)
