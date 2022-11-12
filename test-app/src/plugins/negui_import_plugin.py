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
    sbml_graph_info = sbmlplot.SBMLGraphInfoImportFromSBMLModel()
    sbml_graph_info.extract_info(filename)
    networ_editor_json = sbmlplot.SBMLGraphInfoExportToNetworkEditor()
    networ_editor_json.extract_graph_info(sbml_graph_info)
    graph_info = networ_editor_json.export()
    return json.dumps(graph_info)
