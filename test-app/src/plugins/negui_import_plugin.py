import json
import networkinfotranslator
    
def items_info():
    import_json_format = {'name' : "JSON",
                  'type': "importtool",
                  'call-functions': [
                      {'name': "saveCurrentNetwork", 'inputs':[]},
                      {'name': "createNetwork", 'inputs': [{'name': "load_json", 'api': "python", 'inputs': [
                          {'name': "getOpenFileName", 'inputs': ["json"]}]}]},
                      {'name': "createChangeStageCommand", 'inputs': []}]}

    import_sbml_format = {'name' : "SBML",
                  'type': "importtool",
                  'call-functions': [
                      {'name': "saveCurrentNetwork", 'inputs':[]},
                      {'name': "createNetwork", 'inputs': [{'name': "load_sbml", 'api': "python", 'inputs': [
                          {'name': "getOpenFileName", 'inputs': ["xml"]}]}]},
                      {'name': "createChangeStageCommand", 'inputs': []}]}
    
    return json.dumps({'items': [import_json_format, import_sbml_format]})

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