import json

def items_info():
    export_as_pdf = {'name' : " as PDF",
                   'type': "export",
                   'call-functions': [{'name': "saveFigure", 'inputs': [{'name': "getSaveFigureFileName", 'inputs':["pdf"]}]}]}

    export_as_svg = {'name' : " as SVG",
                  'type': "export",
                  'call-functions': [{'name': "saveFigure", 'inputs': [{'name': "getSaveFigureFileName", 'inputs':["svg"]}]}]}
    
    return json.dumps({'items': [export_as_pdf, export_as_svg]})