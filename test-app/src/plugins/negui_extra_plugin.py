import json

def items_info():
    select_nodes = {'name': "Select Elements",
                    'call-functions': [{'name': "select_elements", 'input-api-functions': ["exportNetworkInfo"]}]}
    select_edges = {'name': "Delete Selected Elements",
                    'call-functions': [{'name': "delete_selected_elements", 'input-api-functions': []}]}
    delete_and_select1 = {'name': "Delete & Select 1",
                         'call-functions': [{'name': "delete_selected_elements", 'input-api-functions': []},
                                            {'name': "select_elements", 'input-api-functions': []}]}
    delete_and_select2 = {'name': "Delete & Select 2",
                          'call-functions': [{'name': "delete_selected_elements", 'input-api-functions': []},
                                             {'name': "select_elements", 'input-api-functions': []}]}
    move_selected_elementes_to_right = {'name': "Move selected elements",
                          'call-functions': [{'name': "move_selected_elements", 'input-api-functions': []}]}


    return json.dumps({'items': [select_nodes, select_edges, delete_and_select1, delete_and_select2]})


def select_elements(input):
    print(input[0])
    function1 = {'name': "selectElements", 'inputs': [True]}

    return json.dumps({'functions': [function1]})


def delete_selected_elements(input):
    function1 = {'name': "deleteSelectedNetworkElements", 'inputs': []}

    return json.dumps({'functions': [function1]})

def delete_and_select(input):
    function1 = {'name': "deleteSelectedNetworkElements", 'inputs': []}
    function2 = {'name': "selectElements", 'inputs': [True]}

    return json.dumps({'functions': [function1, function2]})

def move_selected_elements(input):
    function1 = {'name': "move", 'inputs': []}