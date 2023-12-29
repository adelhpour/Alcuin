import json


class PluginBase:
    def __init__(self, name, category, short_description, long_description, icon, version, author):
        self._check_plugin_name(name)
        self.name = name
        self._check_plugin_category(category)
        self.category = category
        self._check_plugin_short_description(short_description)
        self.short_description = short_description
        self._check_plugin_long_description(long_description)
        self.long_description = long_description
        self._check_plugin_icon(icon)
        self.icon = icon
        self._check_plugin_version(version)
        self.version = version
        self._check_plugin_author(author)
        self.author = author

    def run(self):
        raise NotImplementedError

    def get_name(self):
        return self.name

    def set_name(self, name):
        self.name = name

    def get_category(self):
        return self.category

    def set_category(self, category):
        self.category = category

    def get_short_description(self):
        return self.short_description

    def set_short_description(self, short_description):
        self.short_description = short_description

    def get_long_description(self):
        return self.long_description

    def set_long_description(self, long_description):
        self.long_description = long_description

    def get_icon(self):
        return self.icon

    def set_icon(self, icon):
        self.icon = icon

    def get_version(self):
        return self.version

    def set_version(self, version):
        self.version = version

    def get_author(self):
        return self.author

    def set_author(self, author):
        self.author = author

    @staticmethod
    def _check_plugin_name(plugin_name):
        if not isinstance(plugin_name, str):
            raise TypeError("plugin_name must be a string")

    @staticmethod
    def _check_plugin_category(plugin_category):
        if not isinstance(plugin_category, str):
            raise TypeError("plugin_category must be a string")

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


class Plugin(PluginBase):
    def __init__(self, name, commands=None, category="",
                 short_description="", long_description="", icon="", version="", author=""):
        super().__init__(name, category, short_description, long_description, icon, version, author)
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
        return json.dumps({'name': self.name,
                            'category': self.get_category(),
                            'short_description': self.get_short_description(),
                            'long_description': self.get_long_description(),
                            'icon': self.get_icon(),
                            'version': self.get_version(),
                            'author': self.get_author(),
                            'call-functions': json.loads(self.get_commands().__repr__())})

    def __repr__(self):
        return self.__str__()


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
