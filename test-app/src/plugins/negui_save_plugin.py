import json
import networkinfotranslator

def items_info():
    save_as_sbml = {'name' : " as SBML",
                    'type': "save_as",
                    'call-functions': [{'name': "write_sbml", 'api': "python", 'inputs': [{'name': "exportNetworkInfo", 'inputs':[]},
                                                                                          {'name': "getSaveAsFileName", 'inputs':["xml"]}]}]}

    save_as_json = {'name' : " as JSON",
                    'type': "save_as",
                    'call-functions': [{'name': "write_json", 'api': "python", 'inputs': [{'name': "exportNetworkInfo", 'inputs':[]},
                                                                                                {'name': "getSaveAsFileName", 'inputs':["json"]}]}]}

    save = {'name' : "Save",
                    'type': "save",
                    'call-functions': [{'name': "save_file", 'api': "python", 'inputs':[{'name': "exportNetworkInfo", 'inputs':[]}]}]}

    return (json.dumps(save_as_sbml), json.dumps(save_as_json), json.dumps(save))

def write_json(input):
    if len(input) == 2 and input[1]:
        graph_info = json.loads(input[0])
        file_name = input[1]
        with open(file_name, 'w', encoding='utf8') as js_file:
            json.dump(graph_info, js_file, indent=1)


def write_sbml(input):
    if len(input) == 2 and input[1]:
        network_info = json.loads(input[0])
        file_name = input[1]
        network_info_import_from_network_editor = networkinfotranslator.NetworkInfoImportFromNetworkEditor()
        network_info_import_from_network_editor.extract_info(network_info)
        network_info_export_to_sbml_model = networkinfotranslator.NetworkInfoExportToSBMLModel()
        network_info_export_to_sbml_model.extract_graph_info(network_info_import_from_network_editor)
        network_info_export_to_sbml_model.export(file_name)

def save_file(input):
    if len(input) == 2:
        if input[1].split(".")[-1] == "xml":
            write_sbml(input)
        else:
            write_json(input)
