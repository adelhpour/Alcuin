import json


class PluginBase:
    def __init__(self, plugin_name, plugin_type, plugin_short_description,
                 plugin_long_description, plugin_icon, plugin_version, plugin_author):
        self._check_plugin_name(plugin_name)
        self.plugin_name = plugin_name
        self._check_plugin_type(plugin_type)
        self.plugin_type = plugin_type
        self._check_plugin_short_description(plugin_short_description)
        self.plugin_short_description = plugin_short_description
        self._check_plugin_long_description(plugin_long_description)
        self.plugin_long_description = plugin_long_description
        self._check_plugin_icon(plugin_icon)
        self.plugin_icon = plugin_icon
        self._check_plugin_version(plugin_version)
        self.plugin_version = plugin_version
        self._check_plugin_author(plugin_author)
        self.plugin_author = plugin_author

    def run(self):
        raise NotImplementedError

    def get_name(self):
        return self.plugin_name

    def set_name(self, plugin_name):
        self.plugin_name = plugin_name

    def get_type(self):
        return self.plugin_type

    def set_type(self, plugin_type):
        self.plugin_type = plugin_type

    def get_short_description(self):
        return self.plugin_short_description

    def set_short_description(self, plugin_short_description):
        self.plugin_short_description = plugin_short_description

    def get_long_description(self):
        return self.plugin_long_description

    def set_long_description(self, plugin_long_description):
        self.pluign_long_description = plugin_long_description

    def get_icon(self):
        return self.plugin_icon

    def set_icon(self, plugin_icon):
        self.plugin_icon = plugin_icon

    def get_version(self):
        return self.plugin_version

    def set_version(self, plugin_version):
        self.plugin_version = plugin_version

    def get_author(self):
        return self.plugin_author

    def set_author(self, plugin_author):
        self.plugin_author = plugin_author

    @staticmethod
    def _check_plugin_name(plugin_name):
        if not isinstance(plugin_name, str):
            raise TypeError("plugin_name must be a string")

    @staticmethod
    def _check_plugin_type(plugin_type):
        if not isinstance(plugin_type, str):
            raise TypeError("plugin_type must be a string")

    @staticmethod
    def _check_plugin_short_description(plugin_short_description):
        if not isinstance(plugin_short_description, str):
            raise TypeError("plugin_short_description must be a string")

    @staticmethod
    def _check_plugin_long_description(plugin_long_description):
        if not isinstance(plugin_long_description, str):
            raise TypeError("plugin_long_description must be a string")

    @staticmethod
    def _check_plugin_icon(plugin_icon):
        if not isinstance(plugin_icon, str):
            raise TypeError("plugin_icon must be a string")

    @staticmethod
    def _check_plugin_version(plugin_version):
        if not isinstance(plugin_version, str):
            raise TypeError("plugin_version must be a string")

    @staticmethod
    def _check_plugin_author(plugin_author):
        if not isinstance(plugin_author, str):
            raise TypeError("plugin_author must be a string")

    def __str__(self):
        return json.dumps({'name': self.get_name(),
                            'type': self.get_type(),
                            'short_description': self.get_short_description(),
                            'long_description': self.get_long_description(),
                            'icon': self.get_icon(),
                            'version': self.get_version(),
                            'author': self.get_author()})

    def __repr__(self):
        return self.__str__()


class CommandPlugin(PluginBase):
    def __init__(self, plugin_name, commands=None, plugin_type="",
                 plugin_short_description="", plugin_long_description="",
                 plugin_icon="", plugin_version="", plugin_author=""):
        super().__init__(plugin_name, plugin_type,
                         plugin_short_description, plugin_long_description,
                         plugin_icon, plugin_version, plugin_author)
        self.commands = [] if commands is None else commands

    def get_commands(self):
        return self.commands

    def set_commands(self, commands):
        self.commands = commands

    def add_command(self, command):
        self.commands.append(command)

    def remove_command(self, command):
        self.commands.remove(command)

    def remove_command_by_name(self, command_name):
        for command in self.commands:
            if command.get_function_name() == command_name:
                self.commands.remove(command)
                break

    def remove_command_by_index(self, command_index):
        self.commands.pop(command_index)

    def __str__(self):
        return json.dumps({'name': self.get_name(),
                            'type': self.get_type(),
                            'short_description': self.get_short_description(),
                            'long_description': self.get_long_description(),
                            'icon': self.get_icon(),
                            'version': self.get_version(),
                            'author': self.get_author(),
                            'call-functions': json.loads(self.get_commands().__repr__())})


class CommandBase:
    def __init__(self, function_name, parameters=None):
        self._check_function_name(function_name)
        self.function_name = function_name
        if parameters is not None:
            self._check_parameters(parameters)
            self.parameters = parameters
        else:
            self.parameters = []

    def get_api(self):
        return ""

    def get_function_name(self):
        return self.function_name

    def set_function_name(self, function_name):
        self._check_function_name(function_name)
        self.function_name = function_name

    def get_parameters(self):
        return self.parameters

    def set_parameters(self, parameters):
        self._check_parameters(parameters)
        self.parameters = parameters

    def add_parameter(self, parameter):
        self._check_parameter(parameter)
        self.parameters.append(parameter)

    def remove_parameter(self, parameter):
        self._check_parameter(parameter)
        self.parameters.remove(parameter)

    def remove_parameter_by_name(self, parameter_name):
        self._check_parameter_name(parameter_name)
        for parameter in self.parameters:
            if parameter.get_name() == parameter_name:
                self.parameters.remove(parameter)
                break

    def remove_parameter_by_index(self, parameter_index):
        self._check_parameter_index(parameter_index)
        self.parameters.pop(parameter_index)

    @staticmethod
    def _check_function_name(function_name):
        if not isinstance(function_name, str):
            raise TypeError("function_name must be a string")

    @staticmethod
    def _check_parameter_name(parameter_name):
        if not isinstance(parameter_name, str):
            raise TypeError("parameter_name must be a string")

    def _check_parameter_index(self, parameter_index):
        if not isinstance(parameter_index, int):
            raise TypeError("parameter_index must be an integer")
        if not parameter_index < len(self.parameters):
            raise IndexError("parameter_index must be less than the number of parameters")

    def _check_parameters(self, parameters):
        if not hasattr(parameters, "__len__"):
            raise TypeError("parameters must be a list, tuple, or other iterable")
        for parameter in parameters:
            self._check_parameter(parameter)

    @staticmethod
    def _check_parameter(parameter):
        if not isinstance(parameter, (str, int, float, bool)) and not isinstance(parameter, CommandBase):
            raise TypeError("parameter must be a string, integer, float, boolean, or an instance of CommandBase")

    def __str__(self):
        parameter_array = []
        for parameter in self.get_parameters():
            if isinstance(parameter, CommandBase):
                parameter_array.append(json.loads(parameter.__repr__()))
            else:
                parameter_array.append(parameter)
        return json.dumps({'name': self.get_function_name(),
                            'api': self.get_api(),
                            'inputs': parameter_array})

    def __repr__(self):
        return self.__str__()


class CPlusPlusCommand(CommandBase):
    def __init__(self, function_name, parameters=None):
        super().__init__(function_name, parameters)

    def get_api(self):
        return "c++"


class PythonCommand(CommandBase):
    def __init__(self, function_name, parameters=None):
        super().__init__(function_name, parameters)

    def get_api(self):
        return "python"


class ElementStylePluginBase(PluginBase):
    def __init__(self, element_style_name, element_style_type,
                 element_style_short_description, element_style_long_description,
                 element_style_icon, element_style_version, element_style_author,
                 is_frequently_used, name_title, is_name_editable,
                 element_style_category, element_style_sub_category, element_style_shapes=None):
        super().__init__(plugin_name=element_style_name, plugin_type=element_style_type,
                         plugin_short_description=element_style_short_description,
                         plugin_long_description=element_style_long_description,
                         plugin_icon=element_style_icon, plugin_version=element_style_version,
                         plugin_author=element_style_author)
        self._check_is_frequently_used(is_frequently_used)
        self.is_frequently_used = is_frequently_used
        self._check_name_title(name_title)
        self.name_title = name_title
        self._check_is_name_editable(is_name_editable)
        self.is_name_editable = is_name_editable
        self._check_element_style_category(element_style_category)
        self.element_style_category = element_style_category
        self._check_element_style_sub_category(element_style_sub_category)
        self.element_style_sub_category = element_style_sub_category
        if element_style_shapes is not None:
            self._check_element_style_shapes(element_style_shapes)
            self.element_style_shapes = element_style_shapes
        else:
            self.element_style_shapes = []

    def get_is_frequently_used(self):
        return self.is_frequently_used

    def set_is_frequently_used(self, is_frequently_used):
        self._check_is_frequently_used(is_frequently_used)
        self.is_frequently_used = is_frequently_used

    def get_name_title(self):
        return self.name_title

    def set_name_title(self, name_title):
        self._check_name_title(name_title)
        self.name_title = name_title

    def get_is_name_editable(self):
        return self.is_name_editable

    def set_is_name_editable(self, is_name_editable):
        self._check_is_name_editable(is_name_editable)
        self.is_name_editable = is_name_editable

    def get_category(self):
        return self.element_style_category

    def set_category(self, element_style_category):
        self._check_element_style_category(element_style_category)
        self.element_style_category = element_style_category

    def get_sub_category(self):
        return self.element_style_sub_category

    def set_sub_category(self, element_style_sub_category):
        self._check_element_style_sub_category(element_style_sub_category)
        self.element_style_sub_category = element_style_sub_category

    def get_shapes(self):
        return self.element_style_shapes

    def set_shapes(self, element_style_shapes):
        self._check_element_style_shapes(element_style_shapes)
        self.element_style_shapes = element_style_shapes

    def add_shape(self, element_style_shape):
        self._check_element_style_shape(element_style_shape)
        self.element_style_shapes.append(element_style_shape)

    def remove_shape(self, element_style_shape):
        self._check_element_style_shape(element_style_shape)
        self.element_style_shapes.remove(element_style_shape)

    def remove_shape_by_index(self, element_style_shape_index):
        self._check_element_style_shape_index(element_style_shape_index)
        self.element_style_shapes.pop(element_style_shape_index)

    @staticmethod
    def _check_is_frequently_used(is_frequently_used):
        if not isinstance(is_frequently_used, bool):
            raise TypeError("is_frequently_used must be a boolean")

    @staticmethod
    def _check_name_title(name_title):
        if not isinstance(name_title, str):
            raise TypeError("name_title must be a string")

    @staticmethod
    def _check_is_name_editable(is_name_editable):
        if not isinstance(is_name_editable, bool):
            raise TypeError("is_name_editable must be a boolean")

    @staticmethod
    def _check_element_style_category(element_style_category):
        if not isinstance(element_style_category, str):
            raise TypeError("element_style_category must be a string")

    @staticmethod
    def _check_element_style_sub_category(element_style_sub_category):
        if not isinstance(element_style_sub_category, str):
            raise TypeError("element_style_sub_category must be a string")

    def _check_element_style_shapes(self, element_style_shapes):
        if not hasattr(element_style_shapes, "__len__"):
            raise TypeError("element_style_shapes must be a list, tuple, or other iterable")
        for element_style_shape in element_style_shapes:
            self._check_element_style_shape(element_style_shape)

    @staticmethod
    def _check_element_style_shape(element_style_shape):
        if not isinstance(element_style_shape, ShapeBase):
            raise TypeError("element_style_shape must be an instance of ShapeBase")

    def _check_element_style_shape_index(self, element_style_shape_index):
        if not isinstance(element_style_shape_index, int):
            raise TypeError("element_style_shape_index must be an integer")
        if not element_style_shape_index < len(self.element_style_shapes):
            raise IndexError("element_style_shape_index must be less than the number of element_style_shapes")

    def __str__(self):
        return json.dumps({'name': self.get_name(),
                            'type': self.get_type(),
                            'short_description': self.get_short_description(),
                            'long_description': self.get_long_description(),
                            'icon': self.get_icon(),
                            'version': self.get_version(),
                            'author': self.get_author(),
                            'is-frequently-used': self.get_is_frequently_used(),
                            'name-title': self.get_name_title(),
                            'is-name-editable': self.get_is_name_editable(),
                            'category': self.get_category(),
                            'sub-category': self.get_sub_category(),
                            'shapes': json.loads(self.get_shapes().__repr__())})


class NodeStylePlugin(ElementStylePluginBase):
    def __init__(self, node_style_name, node_style_short_description="", node_style_long_description="",
                 node_style_icon="", node_style_version="", node_style_author="",
                 is_frequently_used=False, name_title="", is_name_editable=False,
                 node_style_category="", node_style_sub_category="", node_style_shapes=None,
                 parent_title="", parent_categories=None, convertible_parent_category=""):
        super().__init__(element_style_name=node_style_name, element_style_type="nodestyle",
                         element_style_short_description=node_style_short_description,
                         element_style_long_description=node_style_long_description,
                         element_style_icon=node_style_icon, element_style_version=node_style_version,
                         element_style_author=node_style_author,
                         is_frequently_used=is_frequently_used, name_title=name_title,
                         is_name_editable=is_name_editable, element_style_category=node_style_category,
                         element_style_sub_category=node_style_sub_category, element_style_shapes=node_style_shapes)
        self._check_parent_title(parent_title)
        self.parent_title = parent_title
        if parent_categories is not None:
            self._check_parent_categories(parent_categories)
            self.parent_categories = parent_categories
        else:
            self.parent_categories = []
        self._check_convertible_parent_category(convertible_parent_category)
        self.convertible_parent_category = convertible_parent_category

    def get_parent_title(self):
        return self.parent_title

    def set_parent_title(self, parent_title):
        self._check_parent_title(parent_title)
        self.parent_title = parent_title

    def get_parent_categories(self):
        return self.parent_categories

    def set_parent_categories(self, parent_categories):
        self._check_parent_categories(parent_categories)
        self.parent_categories = parent_categories

    def add_parent_category(self, parent_category):
        self._check_parent_category(parent_category)
        self.parent_categories.append(parent_category)

    def remove_parent_category(self, parent_category):
        self._check_parent_category(parent_category)
        self.parent_categories.remove(parent_category)

    def remove_parent_category_by_index(self, parent_category_index):
        self._check_parent_category_index(parent_category_index)
        self.parent_categories.pop(parent_category_index)

    def get_convertible_parent_category(self):
        return self.convertible_parent_category

    def set_convertible_parent_category(self, convertible_parent_category):
        self._check_convertible_parent_category(convertible_parent_category)
        self.convertible_parent_category = convertible_parent_category

    @staticmethod
    def _check_parent_title(parent_title):
        if not isinstance(parent_title, str):
            raise TypeError("parent_title must be a string")

    @staticmethod
    def _check_parent_categories(parent_categories):
        if not hasattr(parent_categories, "__len__"):
            raise TypeError("parent_categories must be a list, tuple, or other iterable")
        for parent_category in parent_categories:
            NodeStylePlugin._check_parent_category(parent_category)

    @staticmethod
    def _check_parent_category(parent_category):
        if not isinstance(parent_category, str):
            raise TypeError("parent_category must be a string")

    def _check_parent_category_index(self, parent_category_index):
        if not isinstance(parent_category_index, int):
            raise TypeError("parent_category_index must be an integer")
        if not parent_category_index < len(self.parent_categories):
            raise IndexError("parent_category_index must be less than the number of parent_categories")

    @staticmethod
    def _check_convertible_parent_category(convertible_parent_category):
        if not isinstance(convertible_parent_category, str):
            raise TypeError("convertible_parent_category must be a string")

    def __str__(self):
        shape_array = []
        for shape in self.get_shapes():
            shape_array.append(json.loads(shape.__repr__()))
        return json.dumps({'name': self.get_name(),
                           'type': self.get_type(),
                           'short_description': self.get_short_description(),
                           'long_description': self.get_long_description(),
                           'icon': self.get_icon(),
                           'version': self.get_version(),
                           'author': self.get_author(),
                           'is-frequently-used': self.get_is_frequently_used(),
                           'name-title': self.get_name_title(),
                           'is-name-editable': self.get_is_name_editable(),
                           'category': self.get_category(),
                           'sub-category': self.get_sub_category(),
                           'shapes': shape_array,
                           'parent-title': self.get_parent_title(),
                           'parent-categories': self.get_parent_categories(),
                           'convertible-parent-category': self.get_convertible_parent_category()})


class EdgeStylePlugin(ElementStylePluginBase):
    def __init__(self, edge_style_name, edge_style_short_description="",
                 edge_style_long_description="", edge_style_icon="",
                 edge_style_version="", edge_style_author="",
                 is_frequently_used=False, name_title="", is_name_editable=False,
                 edge_style_category="", edge_style_sub_category="", edge_style_shapes=None,
                 connectible_source_node_title="", connectible_source_node_categories=None,
                 connectible_target_node_title="", connectible_target_node_categories=None, arrow_head_style=None):
        super().__init__(element_style_name=edge_style_name, element_style_type="edgestyle",
                         element_style_short_description=edge_style_short_description,
                         element_style_long_description=edge_style_long_description,
                         element_style_icon=edge_style_icon, element_style_version=edge_style_version,
                         element_style_author=edge_style_author,
                         is_frequently_used=is_frequently_used, name_title=name_title,
                         is_name_editable=is_name_editable, element_style_category=edge_style_category,
                         element_style_sub_category=edge_style_sub_category, element_style_shapes=edge_style_shapes)
        self._check_connectible_source_node_title(connectible_source_node_title)
        self.connectible_source_node_title = connectible_source_node_title
        if connectible_source_node_categories is not None:
            self._check_connectible_source_node_categories(connectible_source_node_categories)
            self.connectible_source_node_categories = connectible_source_node_categories
        else:
            self.connectible_source_node_categories = []
        self._check_connectible_target_node_title(connectible_target_node_title)
        self.connectible_target_node_title = connectible_target_node_title
        if connectible_target_node_categories is not None:
            self._check_connectible_target_node_categories(connectible_target_node_categories)
            self.connectible_target_node_categories = connectible_target_node_categories
        else:
            self.connectible_target_node_categories = []
        self._check_arrow_head_style(arrow_head_style)
        self.arrow_head_style = arrow_head_style

    def get_connectible_source_node_title(self):
        return self.connectible_source_node_title

    def set_connectible_source_node_title(self, connectible_source_node_title):
        self._check_connectible_source_node_title(connectible_source_node_title)
        self.connectible_source_node_title = connectible_source_node_title

    def get_connectible_source_node_categories(self):
        return self.connectible_source_node_categories

    def set_connectible_source_node_categories(self, connectible_source_node_categories):
        self._check_connectible_source_node_categories(connectible_source_node_categories)
        self.connectible_source_node_categories = connectible_source_node_categories

    def add_connectible_source_node_category(self, connectible_source_node_category):
        self._check_connectible_source_node_category(connectible_source_node_category)
        self.connectible_source_node_categories.append(connectible_source_node_category)

    def remove_connectible_source_node_category(self, connectible_source_node_category):
        self._check_connectible_source_node_category(connectible_source_node_category)
        self.connectible_source_node_categories.remove(connectible_source_node_category)

    def remove_connectible_source_node_category_by_index(self, connectible_source_node_category_index):
        self._check_connectible_source_node_category_index(connectible_source_node_category_index)
        self.connectible_source_node_categories.pop(connectible_source_node_category_index)

    def get_connectible_target_node_title(self):
        return self.connectible_target_node_title

    def set_connectible_target_node_title(self, connectible_target_node_title):
        self._check_connectible_target_node_title(connectible_target_node_title)
        self.connectible_target_node_title = connectible_target_node_title

    def get_connectible_target_node_categories(self):
        return self.connectible_target_node_categories

    def set_connectible_target_node_categories(self, connectible_target_node_categories):
        self._check_connectible_target_node_categories(connectible_target_node_categories)
        self.connectible_target_node_categories = connectible_target_node_categories

    def add_connectible_target_node_category(self, connectible_target_node_category):
        self._check_connectible_target_node_category(connectible_target_node_category)
        self.connectible_target_node_categories.append(connectible_target_node_category)

    def remove_connectible_target_node_category(self, connectible_target_node_category):
        self._check_connectible_target_node_category(connectible_target_node_category)
        self.connectible_target_node_categories.remove(connectible_target_node_category)

    def remove_connectible_target_node_category_by_index(self, connectible_target_node_category_index):
        self._check_connectible_target_node_category_index(connectible_target_node_category_index)
        self.connectible_target_node_categories.pop(connectible_target_node_category_index)

    def get_arrow_head_style(self):
        return self.arrow_head_style

    def set_arrow_head_style(self, arrow_head_style):
        self._check_arrow_head_style(arrow_head_style)
        self.arrow_head_style = arrow_head_style

    @staticmethod
    def _check_connectible_source_node_title(connectible_source_node_title):
        if not isinstance(connectible_source_node_title, str):
            raise TypeError("connectible_source_node_title must be a string")

    @staticmethod
    def _check_connectible_source_node_categories(connectible_source_node_categories):
        if not hasattr(connectible_source_node_categories, "__len__"):
            raise TypeError("connectible_source_node_categories must be a list, tuple, or other iterable")
        for connectible_source_node_category in connectible_source_node_categories:
            EdgeStylePlugin._check_connectible_source_node_category(connectible_source_node_category)

    @staticmethod
    def _check_connectible_source_node_category(connectible_source_node_category):
        if not isinstance(connectible_source_node_category, str):
            raise TypeError("connectible_source_node_category must be a string")

    def _check_connectible_source_node_category_index(self, connectible_source_node_category_index):
        if not isinstance(connectible_source_node_category_index, int):
            raise TypeError("connectible_source_node_category_index must be an integer")
        if not connectible_source_node_category_index < len(self.connectible_source_node_categories):
            raise IndexError("connectible_source_node_category_index must be less than"
                             "the number of connectible_source_node_categories")

    @staticmethod
    def _check_connectible_target_node_title(connectible_target_node_title):
        if not isinstance(connectible_target_node_title, str):
            raise TypeError("connectible_target_node_title must be a string")

    @staticmethod
    def _check_connectible_target_node_categories(connectible_target_node_categories):
        if not hasattr(connectible_target_node_categories, "__len__"):
            raise TypeError("connectible_target_node_categories must be a list, tuple, or other iterable")
        for connectible_target_node_category in connectible_target_node_categories:
            EdgeStylePlugin._check_connectible_target_node_category(connectible_target_node_category)

    @staticmethod
    def _check_connectible_target_node_category(connectible_target_node_category):
        if not isinstance(connectible_target_node_category, str):
            raise TypeError("connectible_target_node_category must be a string")

    def _check_connectible_target_node_category_index(self, connectible_target_node_category_index):
        if not isinstance(connectible_target_node_category_index, int):
            raise TypeError("connectible_target_node_category_index must be an integer")
        if not connectible_target_node_category_index < len(self.connectible_target_node_categories):
            raise IndexError("connectible_target_node_category_index must be less"
                             "than the number of connectible_target_node_categories")

    @staticmethod
    def _check_arrow_head_style(arrow_head_style):
        if not isinstance(arrow_head_style, ArrowHeadStylePlugin):
            raise TypeError("arrow_head_style must be an instance of ArrowHeadStylePlugin")

    def __str__(self):
        shape_array = []
        for shape in self.get_shapes():
            shape_array.append(json.loads(shape.__repr__()))
        return json.dumps({'name': self.get_name(),
                           'type': self.get_type(),
                           'short_description': self.get_short_description(),
                           'long_description': self.get_long_description(),
                           'icon': self.get_icon(),
                           'version': self.get_version(),
                           'author': self.get_author(),
                           'is-frequently-used': self.get_is_frequently_used(),
                           'name-title': self.get_name_title(),
                           'is-name-editable': self.get_is_name_editable(),
                           'category': self.get_category(),
                           'sub-category': self.get_sub_category(),
                           'shapes': shape_array,
                           'connectible-source-node-title': self.get_connectible_source_node_title(),
                           'connectible-source-node-categories': self.get_connectible_source_node_categories(),
                           'connectible-target-node-title': self.get_connectible_target_node_title(),
                           'connectible-target-node-categories': self.get_connectible_target_node_categories()})


class ArrowHeadStylePlugin(ElementStylePluginBase):
    def __init__(self, arrow_head_style_name, arrow_head_style_short_description="", arrow_head_style_long_description="",
                 arrow_head_style_icon="", arrow_head_style_version="", arrow_head_style_author="",
                 is_frequently_used=False, name_title="", is_name_editable=False,
                 arrow_head_style_category="", arrow_head_style_sub_category="",
                 arrow_head_style_shapes=None):
        super().__init__(element_style_name=arrow_head_style_name, element_style_type="arrowheadstyle",
                         element_style_short_description=arrow_head_style_short_description,
                         element_style_long_description=arrow_head_style_long_description,
                         element_style_icon=arrow_head_style_icon, element_style_version=arrow_head_style_version,
                         element_style_author=arrow_head_style_author,
                         is_frequently_used=is_frequently_used, name_title=name_title,
                         is_name_editable=is_name_editable, element_style_category=arrow_head_style_category,
                         element_style_sub_category=arrow_head_style_sub_category, element_style_shapes=arrow_head_style_shapes)


class ShapeBase:
    def __init__(self, shape_name):
        self._check_shape_name(shape_name)
        self.shape_name = shape_name

    def get_name(self):
        return self.shape_name

    def set_name(self, shape_name):
        self._check_shape_name(shape_name)
        self.shape_name = shape_name

    def __str__(self):
        return json.dumps({'shape': self.get_name()})

    def __repr__(self):
        return self.__str__()

    @staticmethod
    def _check_shape_name(shape_name):
        if not isinstance(shape_name, str):
            raise TypeError("shape_name must be a string")


class Shape1DBase(ShapeBase):
    def __init__(self, shape_name, border_color="black", border_width=2.0):
        super().__init__(shape_name)
        self._check_border_color(border_color)
        self.border_color = border_color
        self._check_border_width(border_width)
        self.border_width = border_width

    def get_border_color(self):
        return self.border_color

    def set_border_color(self, border_color):
        self._check_border_color(border_color)
        self.border_color = border_color

    def get_border_width(self):
        return self.border_width

    def set_border_width(self, border_width):
        self._check_border_width(border_width)
        self.border_width = border_width

    @staticmethod
    def _check_border_color(border_color):
        if not isinstance(border_color, str):
            raise TypeError("border_color must be a string")

    @staticmethod
    def _check_border_width(border_width):
        if not isinstance(border_width, float):
            raise TypeError("border_width must be a float")
        if border_width < 0.1:
            raise ValueError("border_width must be greater than or equal to 0.1")

    def __str__(self):
        return json.dumps({'shape': self.get_name(),
                            'border-color': self.get_border_color(),
                            'border-width': self.get_border_width()})


class LineShape(Shape1DBase):
    def __init__(self, shape_name, border_color="black", border_width=2.0, p1=None, p2=None):
        super().__init__(shape_name, border_color, border_width)
        if p1 is not None:
            self._check_p1(p1)
            self.p1 = p1
        else:
            self.p1 = ShapePoint()
        if p2 is not None:
            self._check_p2(p2)
            self.p2 = p2
        else:
            self.p2 = ShapePoint()

    def get_p1(self):
        return self.p1

    def set_p1(self, p1):
        self._check_p1(p1)
        self.p1 = p1

    def get_p2(self):
        return self.p2

    def set_p2(self, p2):
        self._check_p2(p2)
        self.p2 = p2

    @staticmethod
    def _check_p1(p1):
        if not isinstance(p1, ShapePoint):
            raise TypeError("point must be an instance of ShapePoint")

    @staticmethod
    def _check_p2(p2):
        if not isinstance(p2, ShapePoint):
            raise TypeError("point must be an instance of ShapePoint")

    def __str__(self):
        return json.dumps({'shape': self.get_name(),
                           'border-color': self.get_border_color(),
                           'border-width': self.get_border_width(),
                           'p1': json.loads(self.get_p1().__repr__()),
                           'p2': json.loads(self.get_p2().__repr__())})


class Shape2DBase(Shape1DBase):
    def __init__(self, shape_name, fill_color="transparent", border_color="black", border_width=2.0):
        super().__init__(shape_name, border_color, border_width)
        self._check_fill_color(fill_color)
        self.fill_color = fill_color

    def get_fill_color(self):
        return self.fill_color

    def set_fill_color(self, fill_color):
        self._check_fill_color(fill_color)
        self.fill_color = fill_color

    @staticmethod
    def _check_fill_color(fill_color):
        if not isinstance(fill_color, str):
            raise TypeError("fill_color must be a string")

    def __str__(self):
        return json.dumps({'shape': self.get_name(),
                            'fill-color': self.get_fill_color(),
                            'border-color': self.get_border_color(),
                            'border-width': self.get_border_width()})


class RectangleShape(Shape2DBase):
    def __init__(self, shape_name="rectangle", fill_color="transparent", border_color="black", border_width=2.0, x=0.0, y=0.0, width=20.0, height=20.0, rx=0.0, ry=0.0):
        super().__init__(shape_name, fill_color, border_color, border_width)
        self._check_x(x)
        self.x = x
        self._check_y(y)
        self.y = y
        self._check_width(width)
        self.width = width
        self._check_height(height)
        self.height = height
        self._check_rx(rx)
        self.rx = rx
        self._check_ry(ry)
        self.ry = ry

    def get_x(self):
        return self.x

    def set_x(self, x):
        self._check_x(x)
        self.x = x

    def get_y(self):
        return self.y

    def set_y(self, y):
        self._check_y(y)
        self.y = y

    def get_width(self):
        return self.width

    def set_width(self, width):
        self._check_width(width)
        self.width = width

    def get_height(self):
        return self.height

    def set_height(self, height):
        self._check_height(height)
        self.height = height

    def get_rx(self):
        return self.rx

    def set_rx(self, rx):
        self._check_rx(rx)
        self.rx = rx

    def get_ry(self):
        return self.ry

    def set_ry(self, ry):
        self._check_ry(ry)
        self.ry = ry

    @staticmethod
    def _check_x(x):
        if not isinstance(x, float):
            raise TypeError("x must be a float")

    @staticmethod
    def _check_y(y):
        if not isinstance(y, float):
            raise TypeError("y must be a float")

    @staticmethod
    def _check_width(width):
        if not isinstance(width, float):
            raise TypeError("width must be a float")
        if width < 0.0:
            raise ValueError("width must be greater than or equal to 0.0")

    @staticmethod
    def _check_height(height):
        if not isinstance(height, float):
            raise TypeError("height must be a float")
        if height < 0.0:
            raise ValueError("height must be greater than or equal to 0.0")

    @staticmethod
    def _check_rx(rx):
        if not isinstance(rx, float):
            raise TypeError("rx must be a float")
        if rx < 0.0:
            raise ValueError("rx must be greater than or equal to 0.0")

    @staticmethod
    def _check_ry(ry):
        if not isinstance(ry, float):
            raise TypeError("ry must be a float")
        if ry < 0.0:
            raise ValueError("ry must be greater than or equal to 0.0")

    def __str__(self):
        return json.dumps({'shape': self.get_name(),
                            'fill-color': self.get_fill_color(),
                            'border-color': self.get_border_color(),
                            'border-width': self.get_border_width(),
                            'x': self.get_x(),
                            'y': self.get_y(),
                            'width': self.get_width(),
                            'height': self.get_height(),
                            'rx': self.get_rx(),
                            'ry': self.get_ry()})


class EllipseShape(Shape2DBase):
    def __init__(self, shape_name="ellipse", fill_color="transparent", border_color="black", border_width=2.0, cx=0.0, cy=0.0, rx=20.0, ry=20.0):
        super().__init__(shape_name, fill_color, border_color, border_width)
        self._check_cx(cx)
        self.cx = cx
        self._check_cy(cy)
        self.cy = cy
        self._check_rx(rx)
        self.rx = rx
        self._check_ry(ry)
        self.ry = ry

    def get_cx(self):
        return self.cx

    def set_cx(self, cx):
        self._check_cx(cx)
        self.cx = cx

    def get_cy(self):
        return self.cy

    def set_cy(self, cy):
        self._check_cy(cy)
        self.cy = cy

    def get_rx(self):
        return self.rx

    def set_rx(self, rx):
        self._check_rx(rx)
        self.rx = rx

    def get_ry(self):
        return self.ry

    def set_ry(self, ry):
        self._check_ry(ry)
        self.ry = ry

    @staticmethod
    def _check_cx(cx):
        if not isinstance(cx, float):
            raise TypeError("cx must be a float")

    @staticmethod
    def _check_cy(cy):
        if not isinstance(cy, float):
            raise TypeError("cy must be a float")

    @staticmethod
    def _check_rx(rx):
        if not isinstance(rx, float):
            raise TypeError("rx must be a float")
        if rx < 0.0:
            raise ValueError("rx must be greater than or equal to 0.0")

    @staticmethod
    def _check_ry(ry):
        if not isinstance(ry, float):
            raise TypeError("ry must be a float")
        if ry < 0.0:
            raise ValueError("ry must be greater than or equal to 0.0")

    def __str__(self):
        return json.dumps({'shape': self.get_name(),
                            'fill-color': self.get_fill_color(),
                            'border-color': self.get_border_color(),
                            'border-width': self.get_border_width(),
                            'cx': self.get_cx(),
                            'cy': self.get_cy(),
                            'rx': self.get_rx(),
                            'ry': self.get_ry()})


class PolygonShape(Shape2DBase):
    def __init__(self, shape_name="polygon", fill_color="transparent", border_color="black", border_width=2.0, points=None):
        super().__init__(shape_name, fill_color, border_color, border_width)
        if points is not None:
            self._check_points(points)
            self.points = points
        else:
            self.points = []

    def get_points(self):
        return self.points

    def set_points(self, points):
        self._check_points(points)
        self.points = points

    def add_point(self, point):
        self._check_point(point)
        self.points.append(point)

    def remove_point(self, point):
        self._check_point(point)
        self.points.remove(point)

    def remove_point_by_index(self, point_index):
        self._check_point_index(point_index)
        self.points.pop(point_index)

    @staticmethod
    def _check_points(points):
        if not hasattr(points, "__len__"):
            raise TypeError("points must be a list, tuple, or other iterable")
        for point in points:
            PolygonShape._check_point(point)

    @staticmethod
    def _check_point(point):
        if not isinstance(point, ShapePoint):
            raise TypeError("point must be an instance of ShapePoint")

    def _check_point_index(self, point_index):
        if not isinstance(point_index, int):
            raise TypeError("point_index must be an integer")
        if not point_index < len(self.points):
            raise IndexError("point_index must be less than the number of points")

    def __str__(self):
        point_array = []
        for point in self.get_points():
            point_array.append(json.loads(point.__repr__()))
        return json.dumps({'shape': self.get_name(),
                            'fill-color': self.get_fill_color(),
                            'border-color': self.get_border_color(),
                            'border-width': self.get_border_width(),
                            'points': point_array})


class TextShape(ShapeBase):
    def __init__(self, shape_name, text_color="black", font_family="Arial", font_size=8, set_name_as_default_plain_text=True, x=-20.0, y=-20.0, width=20.0, height=20.0):
        super().__init__(shape_name)
        self._check_text_color(text_color)
        self.text_color = text_color
        self._check_font_family(font_family)
        self.font_family = font_family
        self._check_font_size(font_size)
        self.font_size = font_size
        self._check_set_name_as_default_plain_text(set_name_as_default_plain_text)
        self.set_name_as_default_plain_text = set_name_as_default_plain_text
        self._check_x(x)
        self.x = x
        self._check_y(y)
        self.y = y
        self._check_width(width)
        self.width = width
        self._check_height(height)
        self.height = height

    def get_text_color(self):
        return self.text_color

    def set_text_color(self, text_color):
        self._check_text_color(text_color)
        self.text_color = text_color

    def get_font_family(self):
        return self.font_family

    def set_font_family(self, font_family):
        self._check_font_family(font_family)
        self.font_family = font_family

    def get_font_size(self):
        return self.font_size

    def set_font_size(self, font_size):
        self._check_font_size(font_size)
        self.font_size = font_size

    def get_set_name_as_default_plain_text(self):
        return self.set_name_as_default_plain_text

    def set_set_name_as_default_plain_text(self, set_name_as_default_plain_text):
        self._check_set_name_as_default_plain_text(set_name_as_default_plain_text)
        self.set_name_as_default_plain_text = set_name_as_default_plain_text

    def get_x(self):
        return self.x

    def set_x(self, x):
        self._check_x(x)
        self.x = x

    def get_y(self):
        return self.y

    def set_y(self, y):
        self._check_y(y)
        self.y = y

    def get_width(self):
        return self.width

    def set_width(self, width):
        self._check_width(width)
        self.width = width

    def get_height(self):
        return self.height

    def set_height(self, height):
        self._check_height(height)
        self.height = height

    @staticmethod
    def _check_text_color(text_color):
        if not isinstance(text_color, str):
            raise TypeError("text_color must be a string")

    @staticmethod
    def _check_font_family(font_family):
        if not isinstance(font_family, str):
            raise TypeError("font_family must be a string")

    @staticmethod
    def _check_font_size(font_size):
        if not isinstance(font_size, int):
            raise TypeError("font_size must be an integer")
        if font_size < 1:
            raise ValueError("font_size must be greater than or equal to 1")

    @staticmethod
    def _check_set_name_as_default_plain_text(set_name_as_default_plain_text):
        if not isinstance(set_name_as_default_plain_text, bool):
            raise TypeError("set_name_as_default_plain_text must be a boolean")

    @staticmethod
    def _check_x(x):
        if not isinstance(x, float):
            raise TypeError("x must be a float")

    @staticmethod
    def _check_y(y):
        if not isinstance(y, float):
            raise TypeError("y must be a float")

    @staticmethod
    def _check_width(width):
        if not isinstance(width, float):
            raise TypeError("width must be a float")
        if width < 0.0:
            raise ValueError("width must be greater than or equal to 0.0")

    @staticmethod
    def _check_height(height):
        if not isinstance(height, float):
            raise TypeError("height must be a float")
        if height < 0.0:
            raise ValueError("height must be greater than or equal to 0.0")

    def __str__(self):
        return json.dumps({'shape': self.get_name(),
                            'color': self.get_text_color(),
                            'font-family': self.get_font_family(),
                            'font-size': self.get_font_size(),
                            'set_name_as_default_plain_text': self.get_set_name_as_default_plain_text(),
                            'x': self.get_x(),
                            'y': self.get_y(),
                            'width': self.get_width(),
                            'height': self.get_height()})


class ShapePoint:
    def __init__(self, x=0.0, y=0.0):
        self._check_x(x)
        self.x = x
        self._check_y(y)
        self.y = y

    def get_x(self):
        return self.x

    def set_x(self, x):
        self._check_x(x)
        self.x = x

    def get_y(self):
        return self.y

    def set_y(self, y):
        self._check_y(y)
        self.y = y

    @staticmethod
    def _check_x(x):
        if not isinstance(x, float):
            raise TypeError("x must be a float")

    @staticmethod
    def _check_y(y):
        if not isinstance(y, float):
            raise TypeError("y must be a float")

    def __str__(self):
        return json.dumps({'x': self.get_x(), 'y': self.get_y()})
