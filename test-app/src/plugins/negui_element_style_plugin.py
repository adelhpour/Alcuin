import json
 
def elementStylesInfo():

    compartment_node_style1 = {'name' : "Compartment", 'is-frequently-used' : True, 'type' : "nodestyle", 'category' : "Compartment", 'convertible-parent-category' : "Compartment", 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "rectangle", 'fill-color' : "white", 'border-color' : "black", 'border-width' : 2, 'x' : -200.0, 'y' : -150.0, 'width' : 400.0, 'height' : 300.0, 'rx' : 12.0, 'ry' : 12.0}, {'shape': "text", 'color': "black", 'font-family': "arial", 'font-size': 6, 'set_name_as_default_plain_text': True, 'x': -200.0, 'y': -150.0, 'width': 400.0, 'height': 300.0 }]}

    species_node_style1 = {'name' : "Floating_Species", 'is-frequently-used' : True, 'type' : "nodestyle", 'category' : "Species", 'parent-title' : "Compartment", 'parent-categories' : ["Compartment"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "rectangle", 'fill-color' : "white", 'border-color' : "black", 'border-width' : 2, 'x' : -30.0, 'y' : -20.0, 'width' : 60.0, 'height' : 40.0, 'rx' : 8.0, 'ry' : 6.0}, {'shape': "text", 'color': "black", 'font-family': "arial", 'font-size': 8, 'set_name_as_default_plain_text': True, 'x': -30.0, 'y': -20.0, 'width': 60.0, 'height': 40.0}]}

    species_node_style2 = {'name' : "Boundary_Species", 'is-frequently-used' : True, 'type' : "nodestyle", 'category' : "Species", 'parent-title' : "Compartment", 'parent-categories' : ["Compartment"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "rectangle", 'fill-color' : "white", 'border-color' : "black", 'border-width' : 4, 'x' : -30.0, 'y' : -20.0, 'width' : 60.0, 'height' : 40.0, 'rx' : 8.0, 'ry' : 6.0}, {'shape': "text", 'color': "black", 'font-family': "arial", 'font-size': 8, 'set_name_as_default_plain_text': True, 'x': -30.0, 'y': -20.0, 'width': 60.0, 'height': 40.0}]}

    species_node_style3 = {'name' : "simple_circle", 'is-frequently-used' : False, 'type' : "nodestyle", 'category' : "Species", 'parent-title' : "Compartment", 'parent-categories' : ["Compartment"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "ellipse", 'fill-color' : "white", 'border-color' : "black", 'border-width' : 2},{'shape' : "ellipse", 'fill-color' : "white", 'border-color' : "black", 'border-width' : 2}, {'shape': "text", 'color': "black", 'font-family': "arial", 'font-size': 8, 'set_name_as_default_plain_text': True }]}

    species_node_style4 = {'name' : "hexagon", 'is-frequently-used' : False, 'type' : "nodestyle", 'category' : "Species", 'parent-title' : "Compartment", 'parent-categories' : ["Compartment"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "polygon", 'fill-color' : "white", 'border-color' : "black", 'border-width' : 2, 'points' : [{'x' : -10.0, 'y' : -20}, {'x' : 10.0, 'y' : -20.0}, {'x' : 20.0, 'y' : 0.0}, {'x' : 10.0, 'y' : 20.0}, {'x' : -10.0, 'y' : 20.0}, {'x' : -20.0, 'y' : 0.0}]}, {'shape': "text", 'color': "black", 'font-family': "times", 'font-size': 6, 'set_name_as_default_plain_text': True }]}
    
    reaction_node_style1 = {'name' : "simple_rect", 'is-frequently-used' : False, 'type' : "nodestyle", 'category' : "Reaction", 'parent-title' : "Compartment", 'parent-categories' : ["Compartment"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "rectangle", 'fill-color' : "white", 'border-color' : "black", 'border-width' : 2, "x": -12.5, "y": -12.5, "width": 25.0, "height": 25.0}]}

    reaction_node_style2 = {'name' : "simple_circle", 'is-frequently-used' : False, 'type' : "nodestyle", 'category' : "Reaction", 'parent-title' : "Compartment", 'parent-categories' : ["Compartment"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "ellipse", 'fill-color' : "white", 'border-color' : "black", 'border-width' : 2, "cx": 0.0, "cy": 0.0, "rx": 15.0, "ry": 15.0}]}

    species_reference_edge_style1 = {'name' : "simple_line", 'is-frequently-used' : False, 'type' : "edgestyle", 'category' : "SpeciesReference",  'sub-category' : "Reactant", 'connectable-source-node-title' : "Species", 'connectable-source-node-categories' : ["Species"], 'connectable-target-node-title' : "Reaction", 'connectable-target-node-categories' : ["Reaction"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "line", 'border-color' : "black", 'border-width' : 2}], 'arrow-head' : { 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  []}}
    
    species_reference_edge_style2 = {'name' : "L_Triangular_Head", 'is-frequently-used' : False, 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Product", 'connectable-source-node-title' : "Reaction", 'connectable-source-node-categories' : ["Reaction"], 'connectable-target-node-title' : "Species", 'connectable-target-node-categories' : ["Species"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "line", 'border-color' : "black", 'border-width' : 2}], 'arrow-head' : {'name' : "polygon", 'type' : "arrowheadstyle", 'category' : "LineEnding", 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "polygon", 'fill-color' : "black", 'border-color' : "black", 'border-width' : 2}]}}
    
    # circular head
    species_reference_edge_style3 = {'name' : "Circular_Head", 'is-frequently-used' : False, 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Modifier", 'connectable-source-node-title' : "Species", 'connectable-source-node-categories' : ["Species"], 'connectable-target-node-title' : "Reaction", 'connectable-target-node-categories' : ["Reaction"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "line", 'border-color' : "lightblue", 'border-width' : 2}], 'arrow-head' : {'name' : "ellipse", 'type' : "arrowheadstyle", 'category' : "LineEnding", 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "ellipse", 'fill-color' : "white", 'border-color' : "lightblue", 'border-width' : 2}]}}
    
    # rectangular head
    species_reference_edge_style4 = {'name' : "Rectangular_Head", 'is-frequently-used' : False, 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Modifier", 'connectable-source-node-title' : "Species", 'connectable-source-node-categories' : ["Species"], 'connectable-target-node-title' : "Reaction", 'connectable-target-node-categories' : ["Reaction"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "line", 'border-color' : "brown", 'border-width' : 3}], 'arrow-head' : {'name' : "polygon_2", 'type' : "arrowheadstyle", 'category' : "LineEnding", 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "rectangle", 'fill-color' : "red", 'border-color' : "brown", 'border-width' : 3}]}}
    
    # bezier triangular head
    species_reference_edge_style5 = {'name' : "B_Triangular_Head", 'is-frequently-used' : False, 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Product", 'connectable-source-node-title' : "Reaction", 'connectable-source-node-categories' : ["Reaction"], 'connectable-target-node-title' : "Species", 'connectable-target-node-categories' : ["Species"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "line", 'border-color' : "black", 'border-width' : 2, 'p1' : {'x' : 40, 'y' : 0}, 'p2' : {'x' : -40, 'y' : 0}}], 'arrow-head' : {'name' : "polygon_3", 'type' : "arrowheadstyle", 'category' : "LineEnding", 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "polygon", 'fill-color' : "white", 'border-color' : "black", 'border-width' : 2}]}}
    
    tempelate_reaction_edge_style1 = {'name' : "UniUni_Reaction", 'is-frequently-used' : True, 'type' : "templatestyle", 'category' : "TemplateReaction", 'source-edges' : [{'name' : "simple-line", 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Reactant", 'connectable-source-node-title' : "Species", 'connectable-source-node-categories' : ["Species"], 'connectable-target-node-title' : "Reaction", 'connectable-target-node-categories' : ["Reaction"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "connected-to-target-centroid-shape-line", 'border-color' : "black", 'border-width' : 2, 'p1' : {'x' : 0, 'y' : 0}, 'p2' : {'x' : -40, 'y' : -40}}], 'arrow-head' : { 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  []}}], 'target-edges' : [{'name' : "B_Triangular_Head", 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Product", 'connectable-source-node-title' : "Reaction", 'connectable-source-node-categories' : ["Reaction"], 'connectable-target-node-title' : "Species", 'connectable-target-node-categories' : ["Species"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "connected-to-source-centroid-shape-line", 'border-color' : "black", 'border-width' : 2, 'p1' : {'x' : 40.0, 'y' : 40.0}, 'p2' : {'x' : 0, 'y' : 0}}], 'arrow-head' : {'name' : "polygon_3", 'type' : "arrowheadstyle", 'category' : "LineEnding", 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "polygon", 'fill-color' : "black", 'border-color' : "black", 'border-width' : 2}]}}], 'intermediary-node' : {'name' : "centroid", 'type' : "nodestyle", 'category' : "Reaction", 'parent-title' : "Compartment", 'parent-categories' : ["Compartment"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "centroid"}]}}
    
    tempelate_reaction_edge_style2 = {'name' : "BiUni_Reaction", 'is-frequently-used' : True, 'type' : "templatestyle", 'category' : "TemplateReaction", 'source-edges' : [{'name' : "simple-line", 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Reactant", 'connectable-source-node-title' : "Species", 'connectable-source-node-categories' : ["Species"], 'connectable-target-node-title' : "Reaction", 'connectable-target-node-categories' : ["Reaction"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "connected-to-target-centroid-shape-line", 'border-color' : "black", 'border-width' : 2, 'p1' : {'x' : 40, 'y' : 0}, 'p2' : {'x' : -40, 'y' : 0}}], 'arrow-head' : { 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  []}}, {'name' : "simple-line", 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Reactant", 'connectable-source-node-title' : "Species", 'connectable-source-node-categories' : ["Species"], 'connectable-target-node-title' : "Reaction", 'connectable-target-node-categories' : ["Reaction"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "connected-to-target-centroid-shape-line", 'border-color' : "black", 'border-width' : 2, 'p1' : {'x' : 40, 'y' : 0}, 'p2' : {'x' : -40, 'y' : 0}}], 'arrow-head' : { 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  []}}], 'target-edges' : [{'name' : "B_Triangular_Head", 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Product", 'connectable-source-node-title' : "Reaction", 'connectable-source-node-categories' : ["Reaction"], 'connectable-target-node-title' : "Species", 'connectable-target-node-categories' : ["Species"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "connected-to-source-centroid-shape-line", 'border-color' : "black", 'border-width' : 2, 'p1' : {'x' : 40, 'y' : 0}, 'p2' : {'x' : -40, 'y' : 0}}], 'arrow-head' : {'name' : "polygon_3", 'type' : "arrowheadstyle", 'category' : "LineEnding", 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "polygon", 'fill-color' : "black", 'border-color' : "black", 'border-width' : 2}]}}], 'intermediary-node' : {'name' : "centroid", 'type' : "nodestyle", 'category' : "Reaction", 'parent-title' : "Compartment", 'parent-categories' : ["Compartment"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "centroid"}]}}
    
    tempelate_reaction_edge_style3 = {'name' : "UniBi_Reaction", 'is-frequently-used' : True, 'type' : "templatestyle", 'category' : "TemplateReaction", 'source-edges' : [{'name' : "simple-line", 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Reactant", 'connectable-source-node-title' : "Species", 'connectable-source-node-categories' : ["Species"], 'connectable-target-node-title' : "Reaction", 'connectable-target-node-categories' : ["Reaction"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "connected-to-target-centroid-shape-line", 'border-color' : "black", 'border-width' : 2, 'p1' : {'x' : 40, 'y' : 0}, 'p2' : {'x' : -40, 'y' : 0}}], 'arrow-head' : { 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  []}}], 'target-edges' : [{'name' : "B_Triangular_Head", 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Product", 'connectable-source-node-title' : "Reaction", 'connectable-source-node-categories' : ["Reaction"], 'connectable-target-node-title' : "Species", 'connectable-target-node-categories' : ["Species"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "connected-to-source-centroid-shape-line", 'border-color' : "black", 'border-width' : 2, 'p1' : {'x' : 40, 'y' : 0}, 'p2' : {'x' : -40, 'y' : 0}}], 'arrow-head' : {'name' : "polygon_3", 'type' : "arrowheadstyle", 'category' : "LineEnding", 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "polygon", 'fill-color' : "black", 'border-color' : "black", 'border-width' : 2}]}}, {'name' : "B_Triangular_Head", 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Product", 'connectable-source-node-title' : "Reaction", 'connectable-source-node-categories' : ["Reaction"], 'connectable-target-node-title' : "Species", 'connectable-target-node-categories' : ["Species"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "connected-to-source-centroid-shape-line", 'border-color' : "black", 'border-width' : 2, 'p1' : {'x' : 40, 'y' : 0}, 'p2' : {'x' : -40, 'y' : 0}}], 'arrow-head' : {'name' : "polygon_3", 'type' : "arrowheadstyle", 'category' : "LineEnding", 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "polygon", 'fill-color' : "black", 'border-color' : "black", 'border-width' : 2}]}}], 'intermediary-node' : {'name' : "centroid", 'type' : "nodestyle", 'category' : "Reaction", 'parent-title' : "Compartment", 'parent-categories' : ["Compartment"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "centroid"}]}}
    
    tempelate_reaction_edge_style4 = {'name' : "BiBi_Reaction", 'is-frequently-used' : True, 'type' : "templatestyle", 'category' : "TemplateReaction", 'source-edges' : [{'name' : "simple-line", 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Reactant", 'connectable-source-node-title' : "Species", 'connectable-source-node-categories' : ["Species"], 'connectable-target-node-title' : "Reaction", 'connectable-target-node-categories' : ["Reaction"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "connected-to-target-centroid-shape-line", 'border-color' : "black", 'border-width' : 2, 'p1' : {'x' : 40, 'y' : 0}, 'p2' : {'x' : -40, 'y' : 0}}], 'arrow-head' : { 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  []}}, {'name' : "simple-line", 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Reactant", 'connectable-source-node-title' : "Species", 'connectable-source-node-categories' : ["Species"], 'connectable-target-node-title' : "Reaction", 'connectable-target-node-categories' : ["Reaction"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "connected-to-target-centroid-shape-line", 'border-color' : "black", 'border-width' : 2, 'p1' : {'x' : 40, 'y' : 0}, 'p2' : {'x' : -40, 'y' : 0}}], 'arrow-head' : { 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  []}}], 'target-edges' : [{'name' : "B_Triangular_Head", 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Product", 'connectable-source-node-title' : "Reaction", 'connectable-source-node-categories' : ["Reaction"], 'connectable-target-node-title' : "Species", 'connectable-target-node-categories' : ["Species"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "connected-to-source-centroid-shape-line", 'border-color' : "black", 'border-width' : 2, 'p1' : {'x' : 40, 'y' : 0}, 'p2' : {'x' : -40, 'y' : 0}}], 'arrow-head' : {'name' : "polygon_3", 'type' : "arrowheadstyle", 'category' : "LineEnding", 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "polygon", 'fill-color' : "black", 'border-color' : "black", 'border-width' : 2}]}}, {'name' : "B_Triangular_Head", 'type' : "edgestyle", 'category' : "SpeciesReference", 'sub-category' : "Product", 'connectable-source-node-title' : "Reaction", 'connectable-source-node-categories' : ["Reaction"], 'connectable-target-node-title' : "Species", 'connectable-target-node-categories' : ["Species"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "connected-to-source-centroid-shape-line", 'border-color' : "black", 'border-width' : 2, 'p1' : {'x' : 40, 'y' : 0}, 'p2' : {'x' : -40, 'y' : 0}}], 'arrow-head' : {'name' : "polygon_3", 'type' : "arrowheadstyle", 'category' : "LineEnding", 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "polygon", 'fill-color' : "black", 'border-color' : "black", 'border-width' : 2}]}}], 'intermediary-node' : {'name' : "centroid", 'type' : "nodestyle", 'category' : "Reaction", 'parent-title' : "Compartment", 'parent-categories' : ["Compartment"], 'name-title' : "Id", 'is-name-editable' : True, 'shapes' :  [{'shape' : "centroid"}]}}

    return json.dumps({'items': [compartment_node_style1, species_node_style1, species_node_style2, species_node_style3, species_node_style4, reaction_node_style1, reaction_node_style2, species_reference_edge_style1, species_reference_edge_style2, species_reference_edge_style3, species_reference_edge_style4, species_reference_edge_style5, tempelate_reaction_edge_style1, tempelate_reaction_edge_style2, tempelate_reaction_edge_style3, tempelate_reaction_edge_style4]})
