import json
import networkinfotranslator
import alcuin_plugin

sbml_get_save_as_file_name = alcuin_plugin.CPlusPlusCommand("getSaveAsFileName")
sbml_get_save_as_file_name.add_parameter("xml")
sbml_export_network_info = alcuin_plugin.CPlusPlusCommand("exportNetworkInfo")
sbml_write_sbml = alcuin_plugin.PythonCommand("write_sbml")
sbml_write_sbml.add_parameter(sbml_export_network_info)
sbml_write_sbml.add_parameter(sbml_get_save_as_file_name)
sbml_save_as_sbml = alcuin_plugin.CommandPlugin(plugin_name="as SBML", plugin_type="save_as", commands=[sbml_write_sbml])
alcuin_plugin.add_plugin(sbml_save_as_sbml)

json_get_save_as_file_name = alcuin_plugin.CPlusPlusCommand("getSaveAsFileName")
json_get_save_as_file_name.add_parameter("json")
json_export_network_info = alcuin_plugin.CPlusPlusCommand("exportNetworkInfo")
json_write_json = alcuin_plugin.PythonCommand("write_json")
json_write_json.add_parameter(json_export_network_info)
json_write_json.add_parameter(json_get_save_as_file_name)
json_save_as_json = alcuin_plugin.CommandPlugin(plugin_name="as JSON", plugin_type="save_as", commands=[json_write_json])
alcuin_plugin.add_plugin(json_save_as_json)

save_get_save_file_name = alcuin_plugin.CPlusPlusCommand("getSaveFileName")
save_get_save_file_name.add_parameter("xml")
save_export_network_info = alcuin_plugin.CPlusPlusCommand("exportNetworkInfo")
save_save_file = alcuin_plugin.PythonCommand("save_file")
save_save_file.add_parameter(save_export_network_info)
save_save_file.add_parameter(save_get_save_file_name)
save = alcuin_plugin.CommandPlugin(plugin_name="Save", plugin_type="save", commands=[save_save_file])
alcuin_plugin.add_plugin(save)


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
        return networkinfotranslator.import_network_editor_export_sbml(network_info, file_name)

def save_file(input):
    if len(input) == 2 and input[1]:
        if input[1].split(".")[-1] == "xml":
            write_sbml(input)
        else:
            write_json(input)
