import json
import sbmlplot
    
def dataExportInfo():
    # json
    json_format = {'name' : "as JSON", 'type': "dataexporttool", 'file-extension' : "json", 'default-save-file-name' : "network", 'element-types': {'node': ["Reaction", "Compartment"], 'edge': ["Reactant", "Product", "Modifier"]}}

    # sbml
    sbml_format = {'name' : "as SBML", 'type': "dataexporttool", 'file-extension' : "xml", 'default-save-file-name' : "model", 'element-types': {'node': ["Reaction"], 'edge': ["Reactant", "Product", "Modifier"]}}
    
    return json.dumps({'items': [json_format, sbml_format]})
    

def checkForGraphInfoCompatibiliy(graphInfoString, filetype):
    # json
    if filetype == "as JSON":
        return checkForJSONCompatibiliy(graphInfoString)
    # sbml
    elif filetype == "as SBML":
        return checkForSBMLCompatibiliy(graphInfoString)
        
    return json.dumps({'isInfoCompatible': False, 'messages': [{'message': "export format is not supported"}]})

def checkForJSONCompatibiliy(graphInfoString):
    return json.dumps({'isInfoCompatible': True})


def checkForSBMLCompatibiliy(graphInfoString):
    compartmentsAreCompatible = True
    compartmentEdgesAreCompatible = True
    
    # read graph info
    graphInfo = json.loads(graphInfoString)
    
    
    # get parents
    parentIds = []
    if 'nodes' in list(graphInfo.keys()):
        for n_index in range(len(graphInfo['nodes'])):
            if 'parent' in list(graphInfo['nodes'][n_index].keys()) and graphInfo['nodes'][n_index]['parent'] != "N/A" and graphInfo['nodes'][n_index]['parent'] not in parentIds:
                parentIds.append(graphInfo['nodes'][n_index]['parent'])
                
    
    compartments = []
    species = []
    for n_index in range(len(graphInfo['nodes'])):
        if 'id' in list(graphInfo['nodes'][n_index].keys()):
            if graphInfo['nodes'][n_index]['id'] in parentIds:
                if 'parent' in list(graphInfo['nodes'][n_index].keys()) and graphInfo['nodes'][n_index]['parent'] != "N/A":
                    compartmentsAreCompatible = False
                    break
                else:
                    compartments.append(graphInfo['nodes'][n_index])
            else:
                species.append(graphInfo['nodes'][n_index])
        
    for e_index in range(len(graphInfo['edges'])):
        if 'id' in list(graphInfo['edges'][e_index].keys()):
            if graphInfo['edges'][e_index]['source']['node'] in parentIds or graphInfo['edges'][e_index]['target']['node'] in parentIds:
                compartmentEdgesAreCompatible = False
                break;
            
        
    if compartmentsAreCompatible and compartmentEdgesAreCompatible:
        return json.dumps({'isInfoCompatible': True})
        
    messages = []
    if not compartmentsAreCompatible:
        messages.append({'message': "More than one level of node nesting is not supported in SBML models"})
    
    if not compartmentEdgesAreCompatible:
        messages.append({'message': "Edges starting/ending in a parent not is not supported in SBML models"})
    
    return json.dumps({'isInfoCompatible': False, 'messages': messages})
    
    
def annotateSBMLCompartments(graphInfo):
    parentIds = []
    if 'nodes' in list(graphInfo.keys()):
        for n_index in range(len(graphInfo['nodes'])):
            if 'parent' in list(graphInfo['nodes'][n_index].keys()) and graphInfo['nodes'][n_index]['parent'] != "N/A" and graphInfo['nodes'][n_index]['parent'] not in parentIds:
                parentIds.append(graphInfo['nodes'][n_index]['parent'])
       
    if 'nodes' in list(graphInfo.keys()):
        for n_index in range(len(graphInfo['nodes'])):
            if 'id' in list(graphInfo['nodes'][n_index].keys()) and graphInfo['nodes'][n_index]['id'] in parentIds:
                graphInfo['nodes'][n_index]['type'] = "Compartment"
            

def writeGraphInfoToFile(graphInfoString, filename, filetype):
    graphInfo = json.loads(graphInfoString)
    
    # json
    if filetype == "as JSON":
        writeJSON(graphInfo, filename)
    # sbml
    elif filetype == "as SBML":
        annotateSBMLCompartments(graphInfo)
        writeSBML(graphInfo, filename)


def writeJSON(graphInfo, filename):
    with open(filename, 'w', encoding='utf8') as js_file:
        json.dump(graphInfo, js_file, indent=1)


def writeSBML(network_info, filename):
    print("Bye Bye")
    network_info_import_from_network_editor = sbmlplot.NetworkInfoImportFromNetworkEditor()
    network_info_import_from_network_editor.extract_info(network_info)
    network_info_export_to_sbml_model = sbmlplot.NetworkInfoExportToSBMLModel()
    network_info_export_to_sbml_model.extract_graph_info(network_info_import_from_network_editor)
    network_info_export_to_sbml_model.export(filename)
