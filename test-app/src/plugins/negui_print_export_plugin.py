import json

def items_info():
    pdf_format = {'name' : " as PDF",
                   'type': "printexporttool",
                   'call-functions': [{'name': "saveFigure", 'inputs': [{'name': "getSaveFigureFileName", 'inputs':["pdf"]}]}]}

    svg_format = {'name' : " as SVG",
                  'type': "printexporttool",
                  'call-functions': [{'name': "saveFigure", 'inputs': [{'name': "getSaveFigureFileName", 'inputs':["svg"]}]}]}
    
    return json.dumps({'items': [pdf_format, svg_format]})
