import alcuin_plugin

def items_info():
    get_save_figure_file_name = alcuin_plugin.CPlusPlusCommand("getSaveFigureFileName")
    get_save_figure_file_name.add_parameter("pdf")
    save_figure = alcuin_plugin.CPlusPlusCommand("saveFigure")
    save_figure.add_parameter(get_save_figure_file_name)
    export_as_pdf = alcuin_plugin.CommandPlugin(plugin_name="as PDF", plugin_type="export", commands=[save_figure])

    get_save_figure_file_name = alcuin_plugin.CPlusPlusCommand("getSaveFigureFileName")
    get_save_figure_file_name.add_parameter("svg")
    save_figure = alcuin_plugin.CPlusPlusCommand("saveFigure")
    save_figure.add_parameter(get_save_figure_file_name)
    export_as_svg = alcuin_plugin.CommandPlugin(plugin_name="as SVG", plugin_type="export", commands=[save_figure])

    return (export_as_pdf.__repr__(), export_as_svg.__repr__())
