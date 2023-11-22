import json

def items_info():
    # json
    first_extra_plugin = {'name': "First", 'name-of-call-functions': ["print_first"]}

    # sbml
    second_extra_plugin = {'name': "Second", 'name-of-call-functions': ["print_second"]}

    return json.dumps({'items': [first_extra_plugin, second_extra_plugin]})


def print_first(input):
    print("First")


def print_second(input):
    print("Second")