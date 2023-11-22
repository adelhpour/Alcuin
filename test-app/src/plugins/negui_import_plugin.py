import json
import sbmlplot
    
def items_info():
    # json
    jsonformat = {'name' : "JSON", 'type': "importtool", 'file-extension' : "json", 'name-of-call-functions': ["load_json"]}

    # sbml
    sbmlformat = {'name' : "SBML", 'type': "importtool", 'file-extension' : "xml", 'name-of-call-functions': ["load_sbml"]}
    
    return json.dumps({'items': [jsonformat, sbmlformat]})


def load_json(input):
    file_name = input[0]
    f = open(file_name)
    return json.dumps(json.load(f))


def load_sbml(input):
    file_name = input[0]
    sbml_graph_info = sbmlplot.SBMLGraphInfoImportFromSBMLModel()
    sbml_graph_info.extract_info(file_name)
    network_editor_json = sbmlplot.SBMLGraphInfoExportToNetworkEditor()
    network_editor_json.extract_graph_info(sbml_graph_info)
    graph_info = network_editor_json.export()
    return json.dumps(graph_info)

"""
# keep commented till using the new version of sbmlplot
def loadSBML(file_name):
    network_info_import_from_sbml_model = sbmlplot.NetworkInfoImportFromSBMLModel()
    network_info_import_from_sbml_model.extract_info(file_name)
    network_info_export_to_network_editor = sbmlplot.NetworkInfoExportToNetworkEditor()
    network_info_export_to_network_editor.extract_graph_info(network_info_import_from_sbml_model)
    network_info = network_info_export_to_network_editor.get_graph_info(file_name)
    return json.dumps(network_info)
"""