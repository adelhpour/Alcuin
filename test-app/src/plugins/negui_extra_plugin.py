import json

def items_info():
    select_nodes = {'name': "Select Elements", 'name-of-call-functions': ["select_elements"]}
    select_edges = {'name': "Delete Selected Elements", 'name-of-call-functions': ["delete_selected_elements"]}
    delete_and_select = {'name': "Delete & Select", 'name-of-call-functions': ["delete_and_select"]}

    return json.dumps({'items': [select_nodes, select_edges, delete_and_select]})


def select_elements(input):
    function1 = {'name': "selectElements", 'inputs': [True]}

    return json.dumps({'functions': [function1]})


def delete_selected_elements(input):
    function1 = {'name': "deleteSelectedNetworkElements", 'inputs': []}

    return json.dumps({'functions': [function1]})

def delete_and_select(input):
    function1 = {'name': "deleteSelectedNetworkElements", 'inputs': []}
    function2 = {'name': "selectElements", 'inputs': [True]}

    return json.dumps({'functions': [function1, function2]})