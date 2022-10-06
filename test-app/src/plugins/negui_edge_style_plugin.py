import json
 
def edgeStylesInfo():
    # l-triangular-Head
    style1 = {'name' : "L-Triangular-Head", 'shapes' : [{'shape' : "line", 'stroke' : "black", 'stroke-width' : 2}], 'arrow-head' : {'shapes' : [{'shape' : "polygon", 'fill' : "green", 'stroke' : "black", 'stroke-width' : 2}]}}
    
    # b-triangular-Head
    style2 = {'name' : "B-Triangular-Head", 'shapes' : [{'shape' : "bezier", 'stroke' : "black", 'stroke-width' : 2, 'p1' : {'x' : 40, 'y' : 0}, 'p2' : {'x' : -40, 'y' : 0}}], 'arrow-head' : {'shapes' : [{'shape' : "polygon", 'fill' : "green", 'stroke' : "black", 'stroke-width' : 2}]}}
    
    # circular-Head
    style3 = {'name' : "Circular-Head", 'shapes' : [{'shape' : "line", 'stroke' : "lightblue", 'stroke-width' : 2}], 'arrow-head' : {'shapes' : [{'shape' : "ellipse", 'fill' : "white", 'stroke' : "lightblue", 'stroke-width' : 2}]}}
    
    # rectangular-Head
    style4 = {'name' : "Rectangular-Head", 'shapes' : [{'shape' : "line", 'stroke' : "brown", 'stroke-width' : 3}], 'arrow-head' : {'shapes' : [{'shape' : "rect", 'fill' : "red", 'stroke' : "brown", 'stroke-width' : 3}]}}
    
    style5 = {'name' : "Complex-Head", 'shapes' : [{'shape' : "line", 'stroke' : "brown", 'stroke-width' : 3}], 'arrow-head' : {'shapes' : [{'shape' : "ellipse", 'fill' : "green", 'stroke' : "yellow", 'stroke-width' : 3, 'cx' : -50.0, 'cy' : -60.0, 'rx' : 40.0, 'ry' : 45.0}, {'shape' : "rect", 'fill' : "red", 'stroke' : "yellow", 'stroke-width' : 3, 'x' : 50.0, 'y' : 60.0, 'width' : 80.0, 'height' : 90.0, 'rx' : 2.0, 'ry' : 3.0}, {'shape' : "polygon", 'fill' : "green", 'stroke' : "red", 'stroke-width' : 2, 'points' : [{'x' : -10.0, 'y' : -20}, {'x' : 10.0, 'y' : -20.0}, {'x' : 20.0, 'y' : 0.0}, {'x' : 10.0, 'y' : 20.0}, {'x' : -10.0, 'y' : 20.0}, {'x' : -20.0, 'y' : 0.0}]}, {'shape' : "text", 'plain-text' : "Complex", 'color' : "cyan", 'font-size' : 28, 'font-family' : "serif", 'font-weight': "bold", 'font-style' : "italic", 'x' : 50.0, 'y' : 60.0}]}}
        
    return json.dumps({'items': [style1, style2, style3, style4, style5]})
