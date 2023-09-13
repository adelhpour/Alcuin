import json
import sbmlplot

def dataExportInfo():
    # json
    json_format = {'name' : "as JSON", 'type': "dataexporttool", 'file-extension' : "json", 'default-save-file-name' : "network", 'element-types': {'node': ["Reaction", "Compartment"], 'edge': ["Reactant", "Product", "Modifier"]}}

    # sbml
    sbml_format = {'name' : "as SBML", 'type': "dataexporttool", 'file-extension' : "xml", 'default-save-file-name' : "model", 'element-types': {'node': ["Reaction"], 'edge': ["Reactant", "Product", "Modifier"]}}

    return json.dumps({'items': [json_format, sbml_format]})


def checkForGraphInfoCompatibility(graphInfoString, filetype):
    # json
    if filetype == "as JSON":
        return checkForJSONCompatibility(graphInfoString)
    # sbml
    elif filetype == "as SBML":
        return checkForSBMLCompatibility(graphInfoString)

    return json.dumps({'isInfoCompatible': False, 'messages': [{'message': "export format is not supported"}]})

def checkForJSONCompatibility(graphInfoString):
    return json.dumps({'isInfoCompatible': True})

def checkForSBMLCompatibility(graphInfoString):
    all_species_have_parents = True
    # read graph info
    graphInfo = json.loads(graphInfoString)

    for n_index in range(len(graphInfo['nodes'])):
        if graphInfo['nodes'][n_index]['style']['category'] == "Species":
            if 'parent' not in list(graphInfo['nodes'][n_index].keys()) \
                    or graphInfo['nodes'][n_index]['parent'] == "N/A":
                all_species_have_parents = False

    messages = []
    if not all_species_have_parents:
        messages.append({'message': "Some Species in the network do not belong to any Compartments."})

    is_info_compatible = all_species_have_parents
    return json.dumps({'isInfoCompatible': is_info_compatible, 'messages': messages})

def writeGraphInfoToFile(graphInfoString, filename, filetype):
    graphInfo = json.loads(graphInfoString)

    # json
    if filetype == "as JSON":
        writeJSON(graphInfo, filename)
    # sbml
    elif filetype == "as SBML":
        writeSBML(graphInfo, filename)


def writeJSON(graphInfo, filename):
    with open(filename, 'w', encoding='utf8') as js_file:
        json.dump(graphInfo, js_file, indent=1)

def writeSBML(graphInfo, filename):
    sbml_graph_info = sbmlplot.SBMLGraphInfoImportFromNetworkEditor()
    sbml_graph_info.extract_info(graphInfo)
    sbml_export = sbmlplot.SBMLGraphInfoExportToSBMLModel()
    sbml_export.extract_graph_info(sbml_graph_info)
    sbml_export.export(filename)

"""
# keep commented till using the new version of sbmlplot
def writeSBML(network_info, file_name):
    network_info_import_from_network_editor = sbmlplot.NetworkInfoImportFromNetworkEditor()
    network_info_import_from_network_editor.extract_info(network_info)
    network_info_export_to_sbml_model = sbmlplot.NetworkInfoExportToSBMLModel()
    network_info_export_to_sbml_model.extract_graph_info(network_info_import_from_network_editor)
    network_info_export_to_sbml_model.export(file_name)
    
"""
