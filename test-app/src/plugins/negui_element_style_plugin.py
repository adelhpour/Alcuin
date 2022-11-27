import json
 
def elementStylesInfo():
    compartment_node_style1 = {'name' : "simple_rect", 'type' : "nodestyle", 'category' : "Compartment", 'convertible-parent-category' : "Compartment", 'shapes' : [{'shape' : "rect", 'fill' : "white", 'stroke' : "black", 'stroke-width' : 2, 'width' : 60.0, 'height' : 60.0, 'rx' : 12.0, 'ry' : 12.0}]}

    species_node_style1 = {'name' : "simple_rect", 'type' : "nodestyle", 'category' : "Species", 'parent-categories' : ["Compartment"], 'connectable-categories' : ["Reaction"], 'shapes' : [{'shape' : "rect", 'fill' : "white", 'stroke' : "black", 'stroke-width' : 2, 'width' : 40.0, 'height' : 40.0, 'rx' : 8.0, 'ry' : 8.0}]}
    
    species_node_style2 = {'name' : "simple_circle", 'type' : "nodestyle", 'category' : "Species", 'parent-categories' : ["Compartment"], 'connectable-categories' : ["Reaction"], 'shapes' : [{'shape' : "ellipse", 'fill' : "white", 'stroke' : "black", 'stroke-width' : 2}]}
    
    species_node_style3 = {'name' : "hexagon", 'type' : "nodestyle", 'category' : "Species", 'parent-categories' : ["Compartment"], 'connectable-categories' : ["Reaction"], 'shapes' : [{'shape' : "polygon", 'fill' : "white", 'stroke' : "black", 'stroke-width' : 2, 'points' : [{'x' : -10.0, 'y' : -20}, {'x' : 10.0, 'y' : -20.0}, {'x' : 20.0, 'y' : 0.0}, {'x' : 10.0, 'y' : 20.0}, {'x' : -10.0, 'y' : 20.0}, {'x' : -20.0, 'y' : 0.0}]}]}
    
    reaction_node_style1 = {'name' : "simple_rect", 'type' : "nodestyle", 'category' : "Reaction", 'parent-categories' : ["Compartment"], 'connectable-categories' : ["Species"], 'shapes' : [{'shape' : "rect", 'fill' : "white", 'stroke' : "black", 'stroke-width' : 2, "x": 0.0, "y": 0.0, "width": 25.0, "height": 25.0}]}

    reaction_node_style2 = {'name' : "simple_circle", 'type' : "nodestyle", 'category' : "Reaction", 'parent-categories' : ["Compartment"], 'connectable-categories' : ["Species"], 'shapes' : [{'shape' : "ellipse", 'fill' : "white", 'stroke' : "black", 'stroke-width' : 2, "rx": 15.0, "ry": 15.0, "cx": 15.0, "cy": 15.0}]}
    
    reaction_edge_style3 = {'name' : "simple_line", 'type' : "edgestyle", 'category' : "Reaction", 'shapes' : [{'shape' : "line", 'stroke' : "black", 'stroke-width' : 2}], 'arrow-head' : {'shapes' : []}}
    
    # l-triangular-Head
    species_reference_edge_style1 = {'name' : "simple_line", 'type' : "edgestyle", 'category' : "SpeciesReference",  'sub-category' : "Reactant", 'shapes' : [{'shape' : "line", 'stroke' : "black", 'stroke-width' : 2}], 'arrow-head' : { 'shapes' : []}}
    
    species_reference_edge_style2 = {'name' : "L-Triangular-Head", 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Product", 'shapes' : [{'shape' : "line", 'stroke' : "black", 'stroke-width' : 2}], 'arrow-head' : {'name' : "polygon", 'type' : "arrowheadstyle", 'category' : "LineEnding", 'shapes' : [{'shape' : "polygon", 'fill' : "black", 'stroke' : "black", 'stroke-width' : 2}]}}
    
    # circular-Head
    species_reference_edge_style3 = {'name' : "Circular-Head", 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Modifier", 'shapes' : [{'shape' : "line", 'stroke' : "lightblue", 'stroke-width' : 2}], 'arrow-head' : {'name' : "ellipse", 'type' : "arrowheadstyle", 'category' : "LineEnding", 'shapes' : [{'shape' : "ellipse", 'fill' : "white", 'stroke' : "lightblue", 'stroke-width' : 2}]}}
    
    # rectangular-Head
    species_reference_edge_style4 = {'name' : "Rectangular-Head", 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Modifier", 'shapes' : [{'shape' : "line", 'stroke' : "brown", 'stroke-width' : 3}], 'arrow-head' : {'name' : "polygon_2", 'type' : "arrowheadstyle", 'category' : "LineEnding", 'shapes' : [{'shape' : "rect", 'fill' : "red", 'stroke' : "brown", 'stroke-width' : 3}]}}
    
        # b-triangular-Head
    species_reference_edge_style5 = {'name' : "B-Triangular-Head", 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Product", 'shapes' : [{'shape' : "bezier", 'stroke' : "black", 'stroke-width' : 2, 'p1' : {'x' : 40, 'y' : 0}, 'p2' : {'x' : -40, 'y' : 0}}], 'arrow-head' : {'name' : "polygon_3", 'type' : "arrowheadstyle", 'category' : "LineEnding", 'shapes' : [{'shape' : "polygon", 'fill' : "white", 'stroke' : "black", 'stroke-width' : 2}]}}
        
    return json.dumps({'items': [compartment_node_style1, species_node_style1, species_node_style2, species_node_style3, reaction_node_style1, reaction_node_style2, reaction_edge_style3, species_reference_edge_style1, species_reference_edge_style2, species_reference_edge_style3, species_reference_edge_style4, species_reference_edge_style5]})
