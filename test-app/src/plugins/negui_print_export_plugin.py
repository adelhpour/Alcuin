import json

def printExportInfo():
    # pdf
    pdfformat = {'name' : "as PDF", 'file-extension' : "pdf", 'default-save-file-name' : "Drawing"}
    
    return json.dumps({'items': [pdfformat]})
