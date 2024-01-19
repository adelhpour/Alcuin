import json
import networkinfotranslator
import alcuin_plugin

sbml_save_current_network_with_user_permission = alcuin_plugin.CPlusPlusCommand("saveCurrentNetworkWithUserPermission")
sbml_get_open_file_name = alcuin_plugin.CPlusPlusCommand("getOpenFileName")
sbml_get_open_file_name.add_parameter("xml")
sbml_load_sbml = alcuin_plugin.PythonCommand("load_sbml")
sbml_load_sbml.add_parameter(sbml_get_open_file_name)
sbml_create_network = alcuin_plugin.CPlusPlusCommand("createNetwork")
sbml_create_network.add_parameter(sbml_load_sbml)
sbml_create_change_stage_command = alcuin_plugin.CPlusPlusCommand("createChangeStageCommand")
sbml_open_sbml_format = alcuin_plugin.CommandPlugin(plugin_name="SBML", plugin_type="open",
                                               commands=[sbml_save_current_network_with_user_permission,
                                                         sbml_create_network,
                                                         sbml_create_change_stage_command])
alcuin_plugin.add_plugin(sbml_open_sbml_format)

json_save_current_network_with_user_permission = alcuin_plugin.CPlusPlusCommand("saveCurrentNetworkWithUserPermission")
json_get_open_file_name = alcuin_plugin.CPlusPlusCommand("getOpenFileName")
json_get_open_file_name.add_parameter("json")
json_load_json = alcuin_plugin.PythonCommand("load_json")
json_load_json.add_parameter(json_get_open_file_name)
json_create_network = alcuin_plugin.CPlusPlusCommand("createNetwork")
json_create_network.add_parameter(json_load_json)
json_create_change_stage_command = alcuin_plugin.CPlusPlusCommand("createChangeStageCommand")
json_open_json_format = alcuin_plugin.CommandPlugin(plugin_name="JSON", plugin_type="open",
                                                    commands=[json_save_current_network_with_user_permission,
                                                              json_create_network,
                                                              json_create_change_stage_command])

alcuin_plugin.add_plugin(json_open_json_format)

def load_json(input):
    file_name = input[0]
    if file_name:
        f = open(file_name)
        return json.dumps(json.load(f))

    return ""


def load_sbml(input):
    file_name = input[0]
    if file_name:
        network_info_import_from_sbml_model = networkinfotranslator.NetworkInfoImportFromSBMLModel()
        network_info_import_from_sbml_model.extract_info(file_name)
        network_info_export_to_network_editor = networkinfotranslator.NetworkInfoExportToNetworkEditor()
        network_info_export_to_network_editor.extract_graph_info(network_info_import_from_sbml_model)
        network_info = network_info_export_to_network_editor.export(file_name)
        return json.dumps(network_info)

    return ""