import json
import sbmlplot
    
def importInfo():
    # json
    jsonformat = {'name' : "JSON", 'type': "importtool", 'file-extension' : "json"}

    # sbml
    sbmlformat = {'name' : "SBML", 'type': "importtool", 'file-extension' : "xml"}
    
    return json.dumps({'items': [jsonformat, sbmlformat]})
    

def readGraphInfoFromFile(filename, filetype):
    # json
    if filetype == "JSON":
        return loadJson(filename)
    # sbml
    elif filetype == "SBML":
        return loadSBML(filename)
    
        
    return json.dumps({'items': [{'name' : "SBML", 'file-extension' : "xml"}]})
        
    
def loadJson(filename):
    f = open(filename)
    return json.dumps(json.load(f))
    

def loadSBML(filename):
    network_info_import_from_sbml_model = sbmlplot.NetworkInfoImportFromSBMLModel()
    print(filename)
    network_info_import_from_sbml_model.extract_info(filename)
    network_info_export_to_network_editor = sbmlplot.NetworkInfoExportToNetworkEditor()
    network_info_export_to_network_editor.extract_graph_info(sbml_graph_info)
    network_info = network_info_export_to_network_editor.export()
    return json.dumps(network_info)
