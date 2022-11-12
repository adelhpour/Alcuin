import json

def printExportInfo():
    # pdf
    pdfformat = {'name' : "as PDF", 'type': "printexporttool", 'file-extension' : "pdf", 'default-save-file-name' : "Drawing"}
    
    return json.dumps({'items': [pdfformat]})
