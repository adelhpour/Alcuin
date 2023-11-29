import json
import networkinfotranslator
    
def items_info():
    # json
    jsonformat = {'name' : "JSON",
                  'type': "importtool",
                  'file-extension' : "json",
                  'call-functions': [{'name': "load_json", 'input-api-functions': []}]}

    # sbml
    sbmlformat = {'name' : "SBML",
                  'type': "importtool",
                  'file-extension' : "xml",
                  'call-functions': [{'name': "load_sbml", 'input-api-functions': []}]}
    
    return json.dumps({'items': [jsonformat, sbmlformat]})


def load_json(input):
    file_name = input[0]
    f = open(file_name)
    return json.dumps(json.load(f))


def load_sbml(input):
    file_name = input[0]
    network_info_import_from_sbml_model = networkinfotranslator.NetworkInfoImportFromSBMLModel()
    network_info_import_from_sbml_model.extract_info(file_name)
    network_info_export_to_network_editor = networkinfotranslator.NetworkInfoExportToNetworkEditor()
    network_info_export_to_network_editor.extract_graph_info(network_info_import_from_sbml_model)
    network_info = network_info_export_to_network_editor.export(file_name)
    return json.dumps(network_info)