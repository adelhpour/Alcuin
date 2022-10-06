import json
    
def nodeStylesInfo():
    style1 = {'name' : "Style1", 'shapes' : [{'shape' : "rect", 'fill' : "red", 'stroke' : "black", 'stroke-width' : 2, 'x' : 10.0}, {'shape' : "polygon", 'fill' : "green", 'stroke' : "cyan", 'stroke-width' : 4}]}
    style2 = {'name' : "Style2", 'shapes' : [{'shape' : "ellipse", 'fill' : "green", 'stroke' : "black", 'stroke-width' : 2}, {'shape' : "text", 'plain-text' : "HI", 'color' : "blue", 'font-size' : 16}]}
    style3 = {'name' : "Style3", 'shapes' : [{'shape' : "ellipse", 'fill' : "green", 'stroke' : "yellow", 'stroke-width' : 3, 'cx' : -50.0, 'cy' : -60.0, 'rx' : 40.0, 'ry' : 45.0}, {'shape' : "rect", 'fill' : "red", 'stroke' : "cyan", 'stroke-width' : 3, 'x' : 50.0, 'y' : 60.0, 'width' : 80.0, 'height' : 90.0, 'rx' : 10.0, 'ry' : 20.0}, {'shape' : "polygon", 'fill' : "green", 'stroke' : "red", 'stroke-width' : 2, 'points' : [{'x' : -10.0, 'y' : -20}, {'x' : 10.0, 'y' : -20.0}, {'x' : 20.0, 'y' : 0.0}, {'x' : 10.0, 'y' : 20.0}, {'x' : -10.0, 'y' : 20.0}, {'x' : -20.0, 'y' : 0.0}]}, {'shape' : "text", 'plain-text' : "Complex", 'color' : "cyan", 'font-size' : 28, 'font-family' : "serif", 'font-weight': "bold", 'font-style' : "italic", 'x' : 50.0, 'y' : 60.0}]}
    style4 = {'name' : "Style4", 'shapes' : [{'shape' : "polygon", 'fill' : "green", 'stroke' : "red", 'stroke-width' : 2, 'points' : [{'x' : -10.0, 'y' : -20}, {'x' : 10.0, 'y' : -20.0}, {'x' : 20.0, 'y' : 0.0}, {'x' : 10.0, 'y' : 20.0}, {'x' : -10.0, 'y' : 20.0}, {'x' : -20.0, 'y' : 0.0}]}]}
    style5 = {'name' : "Style5", 'shapes' : [{'shape' : "rect", 'fill' : "green", 'stroke' : "red", 'stroke-width' : 2}]}
        
    return json.dumps({'items': [style1, style2, style3, style4, style5]})
