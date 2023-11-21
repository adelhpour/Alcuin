import json
import sbmlplot
import networkx as nx

def items_info():
    return json.dumps({'items': element_styles_info() + import_formats_info() + data_export_formats_info() + print_export_formats_info() + autoLayout_engines_info()})


def element_styles_info():
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

    return [compartment_node_style1, species_node_style1, species_node_style2, species_node_style3, species_node_style4, reaction_node_style1, reaction_node_style2, species_reference_edge_style1, species_reference_edge_style2, species_reference_edge_style3, species_reference_edge_style4, species_reference_edge_style5, tempelate_reaction_edge_style1, tempelate_reaction_edge_style2, tempelate_reaction_edge_style3, tempelate_reaction_edge_style4]


def import_formats_info():
    # json
    jsonformat = {'name' : "JSON", 'type': "importtool", 'file-extension' : "json"}

    # sbml
    sbmlformat = {'name' : "SBML", 'type': "importtool", 'file-extension' : "xml"}

    return [jsonformat, sbmlformat]


def data_export_formats_info():
    # json
    json_format = {'name' : "as JSON", 'type': "dataexporttool", 'file-extension' : "json", 'default-save-file-name' : "network", 'element-types': {'node': ["Reaction", "Compartment"], 'edge': ["Reactant", "Product", "Modifier"]}}

    # sbml
    sbml_format = {'name' : "as SBML", 'type': "dataexporttool", 'file-extension' : "xml", 'default-save-file-name' : "model", 'element-types': {'node': ["Reaction"], 'edge': ["Reactant", "Product", "Modifier"]}}

    return [json_format, sbml_format]


def print_export_formats_info():
    # pdf
    pdf_format = {'name' : "as PDF", 'type': "printexporttool", 'file-extension' : "pdf", 'default-save-file-name' : "Drawing"}

    # svg
    svg_format = {'name' : "as SVG", 'type': "printexporttool", 'file-extension' : "svg", 'default-save-file-name' : "Drawing"}

    return [pdf_format, svg_format]


def autoLayout_engines_info():
    # circular
    circular = {'name' : "Circular", 'type': "autolayoutengine"}
    # planar
    planar = {'name' : "Planar", 'type': "autolayoutengine"}
    # kamad_kawai
    kamad_kawai = {'name' : "Kamda-Kawai", 'type': "autolayoutengine"}
    # spiral
    spiral = {'name' : "Spiral", 'type': "autolayoutengine", 'parameters' : [{'parameter' : "resolution", 'type' : "double", 'min' : 0.01, 'max' : 1.0, 'step' : 0.01, 'default' : 0.35}, {'parameter' : "equidistant", 'type' : "boolean", 'default' : False}]}

    return [circular, planar, kamad_kawai, spiral]


def readGraphInfoFromFile(input):
    file_type = input[0]
    file_name = input[1]
    # json
    if file_type == "JSON":
        return loadJson(file_name)
    # sbml
    elif file_type == "SBML":
        return loadSBML(file_name)

    return (json.dumps({'items': [{'name' : "SBML", 'file-extension' : "xml"}]}),)


def loadJson(file_name):
    f = open(file_name)
    return (json.dumps(json.load(f)),)


def loadSBML(file_name):
    sbml_graph_info = sbmlplot.SBMLGraphInfoImportFromSBMLModel()
    sbml_graph_info.extract_info(file_name)
    network_editor_json = sbmlplot.SBMLGraphInfoExportToNetworkEditor()
    network_editor_json.extract_graph_info(sbml_graph_info)
    graph_info = network_editor_json.export()
    return (json.dumps(graph_info),)


def checkForGraphInfoCompatibility(input):
    graph_info = json.loads(input[0])
    file_type = input[1]
    # json
    if file_type == "as JSON":
        return checkForJSONCompatibility()
    # sbml
    elif file_type == "as SBML":
        return checkForSBMLCompatibility(graph_info)

    return ({'isInfoCompatible': False, 'messages': [{'message': "export format is not supported"}]},)


def checkForJSONCompatibility():
    return (json.dumps({'isInfoCompatible': True}),)


def checkForSBMLCompatibility(graph_info):
    all_species_have_parents = True
    for n_index in range(len(graph_info['nodes'])):
        if graph_info['nodes'][n_index]['style']['category'] == "Species":
            if 'parent' not in list(graph_info['nodes'][n_index].keys()) \
                    or graph_info['nodes'][n_index]['parent'] == "N/A":
                all_species_have_parents = False

    messages = []
    if not all_species_have_parents:
        messages.append({'message': "Some Species in the network do not belong to any Compartments."})

    is_info_compatible = all_species_have_parents
    return (json.dumps({'isInfoCompatible': is_info_compatible, 'messages': messages}),)


def writeGraphInfoToFile(input):
    graph_info = json.loads(input[0])
    file_type = input[1]
    file_name = input[2]

    # json
    if file_type == "as JSON":
        return writeJSON(graph_info, file_name)
    # sbml
    elif file_type == "as SBML":
        return writeSBML(graph_info, file_name)


def writeJSON(graph_info, file_name):
    with open(file_name, 'w', encoding='utf8') as js_file:
        json.dump(graph_info, js_file, indent=1)


def writeSBML(graph_info, file_name):
    sbml_graph_info = sbmlplot.SBMLGraphInfoImportFromNetworkEditor()
    sbml_graph_info.extract_info(graph_info)
    sbml_export = sbmlplot.SBMLGraphInfoExportToSBMLModel()
    sbml_export.extract_graph_info(sbml_graph_info)
    sbml_export.export(file_name)


def autoLayout(input):
    # read graph info
    graph_info = json.loads(input[0])
    autolayout_info = json.loads(input[1])
    graph = nx.Graph()
    if 'nodes' in list(graph_info.keys()):
        for n_index in range(len(graph_info['nodes'])):
            if 'id' in list(graph_info['nodes'][n_index].keys()) and 'position' in list(graph_info['nodes'][n_index].keys()) and not is_parent(graph_info['nodes'][n_index]) and not is_centroid_node(graph_info['nodes'][n_index]):
                graph.add_node(graph_info['nodes'][n_index]['id'], pos = '{},{}'.format(graph_info['nodes'][n_index]['position']['x'], graph_info['nodes'][n_index]['position']['y']))

    if 'edges' in list(graph_info.keys()):
        for e_index in range(len(graph_info['edges'])):
            if 'id' in list(graph_info['edges'][e_index].keys()) and 'start' in list(graph_info['edges'][e_index].keys()) and 'end' in list(graph_info['edges'][e_index].keys()):
                if 'node' in list(graph_info['edges'][e_index]['start']) and 'node' in list(graph_info['edges'][e_index]['end']):
                    graph.add_edge(graph_info['edges'][e_index]['start']['node'], graph_info['edges'][e_index]['end']['node'])
    pos_center = [0, 0]
    dim_scale = 1.0
    if 'dimensions' in list(graph_info.keys()) and 'width' in list(graph_info['dimensions'].keys()) and 'height' in list(graph_info['dimensions'].keys()):
        dim_scale = 0.5 * min(graph_info['dimensions']['width'], graph_info['dimensions']['height'])
        if 'position' in list(graph_info.keys()) and 'x' in list(graph_info['position'].keys()) and 'y' in list(graph_info['position'].keys()):
            pos_center = [graph_info['position']['x'], graph_info['position']['y']]

    # apply autolayout algorithm
    node_pos = {}
    if 'name' in list(autolayout_info.keys()):
        if autolayout_info['name'] == "Circular":
            node_pos = nx.circular_layout(graph, center = pos_center, scale = dim_scale)
        elif autolayout_info['name'] == "Planar":
            node_pos = nx.planar_layout(graph, center = pos_center, scale = dim_scale)
        elif autolayout_info['name'] == "Kamda-Kawai":
            node_pos = nx.kamada_kawai_layout(graph, center = pos_center, scale = dim_scale)
        elif autolayout_info['name'] == "Spiral":
            _resolution = 0.35
            _equidistant = False
            if 'parameters' in list(autolayout_info.keys()):
                for p_index in range(len(autolayout_info['parameters'])):
                    if 'parameter' in list(autolayout_info['parameters'][p_index].keys()) and 'value' in list(autolayout_info['parameters'][p_index].keys()):
                        if autolayout_info['parameters'][p_index]['parameter'] == "resolution" :
                            _resolution = autolayout_info['parameters'][p_index]['value']
                        if autolayout_info['parameters'][p_index]['parameter'] == "equidistant" :
                            _equidistant = autolayout_info['parameters'][p_index]['value']
            node_pos = nx.spiral_layout(graph, center = pos_center, scale = dim_scale, resolution = _resolution, equidistant = _equidistant)
        else:
            node_pos = nx.circular_layout(graph, center = pos_center, scale = dim_scale)

    # write graph info
    if 'nodes' in list(graph_info.keys()):
        for n_index in range(len(graph_info['nodes'])):
            if 'id' in list(graph_info['nodes'][n_index].keys()) and 'position' in list(graph_info['nodes'][n_index].keys()):
                if graph_info['nodes'][n_index]['id'] in list(node_pos.keys()):
                    graph_info['nodes'][n_index]['position']['x'] = node_pos[graph_info['nodes'][n_index]['id']][0]
                    graph_info['nodes'][n_index]['position']['y'] = node_pos[graph_info['nodes'][n_index]['id']][1]

    return (json.dumps(graph_info), )


def is_parent(node):
    if "style" in list(node.keys()) and "convertible-parent-category" in list(node['style'].keys()) and node['style']['convertible-parent-category']:
        return True

    return False


def is_centroid_node(node):
    if 'style' in list(node.keys()) and 'shapes' in list(node['style']):
        for shape in node['style']['shapes']:
            if 'shape' in list(shape.keys()) and shape['shape'] == "centroid":
                return True

    return False