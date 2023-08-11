import json

def printExportInfo():
    # pdf
    pdf_format = {'name' : "as PDF", 'type': "printexporttool", 'file-extension' : "pdf", 'default-save-file-name' : "Drawing"}

    # svg
    svg_format = {'name' : "as SVG", 'type': "printexporttool", 'file-extension' : "svg", 'default-save-file-name' : "Drawing"}
    
    return json.dumps({'items': [pdf_format, svg_format]})
