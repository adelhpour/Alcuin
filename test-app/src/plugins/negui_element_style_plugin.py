import alcuin_plugin
 
def items_info():

    # compartments
    compartment_node_style1 = alcuin_plugin.NodeStylePlugin("Compartment")
    compartment_node_style1.set_is_frequently_used(True)
    compartment_node_style1.set_category("Compartment")
    compartment_node_style1.set_convertible_parent_category("Compartment")
    compartment_node_style1.set_name_title("Id")
    compartment_node_style1.set_is_name_editable(True)
    compartment_node_style1.add_shape(alcuin_plugin.RectangleShape(fill_color="white", x=-200.0, y=-150.0, width=400.0, height=300.0, rx=12.0, ry=12.0))
    compartment_node_style1.add_shape(alcuin_plugin.TextShape(font_size=6, x=-200.0, y=-150.0, width=400.0, height=300.0))

    # species
    species_node_style1 = alcuin_plugin.NodeStylePlugin("Floating_Species")
    species_node_style1.set_is_frequently_used(True)
    species_node_style1.set_category("Species")
    species_node_style1.set_parent_title("Compartment")
    species_node_style1.add_parent_category("Compartment")
    species_node_style1.set_name_title("Id")
    species_node_style1.set_is_name_editable(True)
    species_node_style1.add_shape(alcuin_plugin.RectangleShape(fill_color="white", x=-30.0, y=-20.0, width=60.0, height=40.0, rx=8.0, ry=6.0))
    species_node_style1.add_shape(alcuin_plugin.TextShape(set_name_as_default_plain_text=True, x=-30.0, y=-20.0, width=60.0, height=40.0))

    species_node_style2 = alcuin_plugin.NodeStylePlugin("Boundary_Species")
    species_node_style2.set_is_frequently_used(True)
    species_node_style2.set_category("Species")
    species_node_style2.set_parent_title("Compartment")
    species_node_style2.add_parent_category("Compartment")
    species_node_style2.set_name_title("Id")
    species_node_style2.set_is_name_editable(True)
    species_node_style2.add_shape(alcuin_plugin.RectangleShape(border_width=4.0, fill_color="white", x=-30.0, y=-20.0, width=60.0, height=40.0, rx=8.0, ry=6.0))
    species_node_style2.add_shape(alcuin_plugin.TextShape(set_name_as_default_plain_text=True, x=-30.0, y=-20.0, width=60.0, height=40.0))

    species_node_style3 = alcuin_plugin.NodeStylePlugin("simple_circle")
    species_node_style3.set_is_frequently_used(False)
    species_node_style3.set_category("Species")
    species_node_style3.set_parent_title("Compartment")
    species_node_style3.add_parent_category("Compartment")
    species_node_style3.set_name_title("Id")
    species_node_style3.set_is_name_editable(True)
    species_node_style3.add_shape(alcuin_plugin.EllipseShape(fill_color="white"))
    species_node_style3.add_shape(alcuin_plugin.TextShape(set_name_as_default_plain_text=True))

    species_node_style4 = alcuin_plugin.NodeStylePlugin("hexagon")
    species_node_style4.set_is_frequently_used(False)
    species_node_style4.set_category("Species")
    species_node_style4.set_parent_title("Compartment")
    species_node_style4.add_parent_category("Compartment")
    species_node_style4.set_name_title("Id")
    species_node_style4.set_is_name_editable(True)
    species_node_style4.add_shape(alcuin_plugin.PolygonShape(fill_color="white", border_color="black", border_width=2.0,
                                                             points=[alcuin_plugin.ShapePoint(-10.0, -20.0),
                                                                     alcuin_plugin.ShapePoint(10.0, -20.0),
                                                                     alcuin_plugin.ShapePoint(20.0, 0.0),
                                                                     alcuin_plugin.ShapePoint(10.0, 20.0),
                                                                     alcuin_plugin.ShapePoint(-10.0, 20.0),
                                                                     alcuin_plugin.ShapePoint(-20.0, 0.0)]))


    # reactions
    reaction_node_style1 = alcuin_plugin.NodeStylePlugin("simple_rect")
    reaction_node_style1.set_is_frequently_used(False)
    reaction_node_style1.set_category("Reaction")
    reaction_node_style1.set_parent_title("Compartment")
    reaction_node_style1.add_parent_category("Compartment")
    reaction_node_style1.set_name_title("Id")
    reaction_node_style1.set_is_name_editable(True)
    reaction_node_style1.add_shape(alcuin_plugin.RectangleShape(fill_color="white", border_color="black", border_width=2.0, x=-12.5, y=-12.5, width=25.0, height=25.0))

    reaction_node_style2 = alcuin_plugin.NodeStylePlugin("simple_circle")
    reaction_node_style2.set_is_frequently_used(False)
    reaction_node_style2.set_category("Reaction")
    reaction_node_style2.set_parent_title("Compartment")
    reaction_node_style2.add_parent_category("Compartment")
    reaction_node_style2.set_name_title("Id")
    reaction_node_style2.set_is_name_editable(True)
    reaction_node_style2.add_shape(alcuin_plugin.EllipseShape(fill_color="white", border_color="black", border_width=2.0, cx=0.0, cy=0.0, rx=15.0, ry=15.0))

    # species references
    species_reference_edge_style1 = alcuin_plugin.EdgeStylePlugin("simple_line")
    species_reference_edge_style1.set_is_frequently_used(False)
    species_reference_edge_style1.set_category("SpeciesReference")
    species_reference_edge_style1.set_sub_category("Reactant")
    species_reference_edge_style1.set_connectable_source_node_title("Species")
    species_reference_edge_style1.set_connectable_source_node_categories(["Species"])
    species_reference_edge_style1.set_connectable_target_node_title("Reaction")
    species_reference_edge_style1.set_connectable_target_node_categories(["Reaction"])
    species_reference_edge_style1.set_name_title("Id")
    species_reference_edge_style1.set_is_name_editable(True)
    species_reference_edge_style1.add_shape(alcuin_plugin.LineShape())

    species_reference_edge_style2 = alcuin_plugin.EdgeStylePlugin("White_Triangular_Head")
    species_reference_edge_style2.set_is_frequently_used(False)
    species_reference_edge_style2.set_category("SpeciesReference")
    species_reference_edge_style2.set_sub_category("Product")
    species_reference_edge_style2.set_connectable_source_node_title("Reaction")
    species_reference_edge_style2.set_connectable_source_node_categories(["Reaction"])
    species_reference_edge_style2.set_connectable_target_node_title("Species")
    species_reference_edge_style2.set_connectable_target_node_categories(["Species"])
    species_reference_edge_style2.set_name_title("Id")
    species_reference_edge_style2.set_is_name_editable(True)
    species_reference_edge_style2.add_shape(alcuin_plugin.LineShape(border_color="black", border_width=2.0))
    species_reference_edge_style2_arrow_head = alcuin_plugin.ArrowHeadStylePlugin("polygon")
    species_reference_edge_style2_arrow_head.set_category("LineEnding")
    species_reference_edge_style2_arrow_head.set_name_title("Id")
    species_reference_edge_style2_arrow_head.set_is_name_editable(True)
    species_reference_edge_style2_arrow_head.add_shape(alcuin_plugin.PolygonShape(fill_color="white", border_color="black", border_width=2.0,
                                                                                    points=[alcuin_plugin.ShapePoint(-10.0, -5.0),
                                                                                            alcuin_plugin.ShapePoint(0.0, 0.0),
                                                                                            alcuin_plugin.ShapePoint(-10.0, 5.0),
                                                                                            alcuin_plugin.ShapePoint(-10.0, 5.0)]))
    species_reference_edge_style2.set_arrow_head_style(species_reference_edge_style2_arrow_head)

    species_reference_edge_style3 = alcuin_plugin.EdgeStylePlugin("Black_Triangular_Head")
    species_reference_edge_style3.set_is_frequently_used(False)
    species_reference_edge_style3.set_category("SpeciesReference")
    species_reference_edge_style3.set_sub_category("Product")
    species_reference_edge_style3.set_connectable_source_node_title("Reaction")
    species_reference_edge_style3.set_connectable_source_node_categories(["Reaction"])
    species_reference_edge_style3.set_connectable_target_node_title("Species")
    species_reference_edge_style3.set_connectable_target_node_categories(["Species"])
    species_reference_edge_style3.set_name_title("Id")
    species_reference_edge_style3.set_is_name_editable(True)
    species_reference_edge_style3.add_shape(alcuin_plugin.LineShape(border_color="black", border_width=2.0))
    species_reference_edge_style3_arrow_head = alcuin_plugin.ArrowHeadStylePlugin("polygon")
    species_reference_edge_style3_arrow_head.set_category("LineEnding")
    species_reference_edge_style3_arrow_head.set_name_title("Id")
    species_reference_edge_style3_arrow_head.set_is_name_editable(True)
    species_reference_edge_style3_arrow_head.add_shape(alcuin_plugin.PolygonShape(fill_color="black", border_color="black", border_width=2.0,
                                                                                  points=[alcuin_plugin.ShapePoint(-10.0, -5.0),
                                                                                          alcuin_plugin.ShapePoint(0.0, 0.0),
                                                                                          alcuin_plugin.ShapePoint(-10.0, 5.0),
                                                                                          alcuin_plugin.ShapePoint(-10.0, 5.0)]))
    species_reference_edge_style3.set_arrow_head_style(species_reference_edge_style3_arrow_head)

    species_reference_edge_style4 = alcuin_plugin.EdgeStylePlugin("Circular_Head")
    species_reference_edge_style4.set_is_frequently_used(False)
    species_reference_edge_style4.set_category("SpeciesReference")
    species_reference_edge_style4.set_sub_category("Modifier")
    species_reference_edge_style4.set_connectable_source_node_title("Species")
    species_reference_edge_style4.set_connectable_source_node_categories(["Species"])
    species_reference_edge_style4.set_connectable_target_node_title("Reaction")
    species_reference_edge_style4.set_connectable_target_node_categories(["Reaction"])
    species_reference_edge_style4.set_name_title("Id")
    species_reference_edge_style4.set_is_name_editable(True)
    species_reference_edge_style4.add_shape(alcuin_plugin.LineShape(border_color="lightblue", border_width=2.0))
    species_reference_edge_style4_arrow_head = alcuin_plugin.ArrowHeadStylePlugin("ellipse")
    species_reference_edge_style4_arrow_head.set_category("LineEnding")
    species_reference_edge_style4_arrow_head.set_name_title("Id")
    species_reference_edge_style4_arrow_head.set_is_name_editable(True)
    species_reference_edge_style4_arrow_head.add_shape(alcuin_plugin.EllipseShape(fill_color="white", border_color="lightblue", border_width=2.0, cx=-5.0, cy=0.0, rx=5.0, ry=5.0))
    species_reference_edge_style4.set_arrow_head_style(species_reference_edge_style4_arrow_head)

    species_reference_edge_style5 = alcuin_plugin.EdgeStylePlugin("Rectangular_Head")
    species_reference_edge_style5.set_is_frequently_used(False)
    species_reference_edge_style5.set_category("SpeciesReference")
    species_reference_edge_style5.set_sub_category("Modifier")
    species_reference_edge_style5.set_connectable_source_node_title("Species")
    species_reference_edge_style5.set_connectable_source_node_categories(["Species"])
    species_reference_edge_style5.set_connectable_target_node_title("Reaction")
    species_reference_edge_style5.set_connectable_target_node_categories(["Reaction"])
    species_reference_edge_style5.set_name_title("Id")
    species_reference_edge_style5.set_is_name_editable(True)
    species_reference_edge_style5.add_shape(alcuin_plugin.LineShape(border_color="brown", border_width=3.0))
    species_reference_edge_style5_arrow_head = alcuin_plugin.ArrowHeadStylePlugin("rectangle")
    species_reference_edge_style5_arrow_head.set_category("LineEnding")
    species_reference_edge_style5_arrow_head.set_name_title("Id")
    species_reference_edge_style5_arrow_head.set_is_name_editable(True)
    species_reference_edge_style5_arrow_head.add_shape(alcuin_plugin.RectangleShape(fill_color="red", border_color="brown", border_width=2.0, x=-7.5, y=-3.725, width=7.5, height=7.5))
    species_reference_edge_style5.set_arrow_head_style(species_reference_edge_style5_arrow_head)

    # template reactions
    source_edge_style = alcuin_plugin.EdgeStylePlugin("source_edge")
    source_edge_style.set_is_frequently_used(False)
    source_edge_style.set_category("SpeciesReference")
    source_edge_style.set_sub_category("Reactant")
    source_edge_style.set_connectable_source_node_title("Species")
    source_edge_style.set_connectable_source_node_categories(["Species"])
    source_edge_style.set_connectable_target_node_title("Reaction")
    source_edge_style.set_connectable_target_node_categories(["Reaction"])
    source_edge_style.set_name_title("Id")
    source_edge_style.set_is_name_editable(True)
    source_edge_style.add_shape(alcuin_plugin.ConnectedToTargetCentroidShapeLine(border_color="black", border_width=2.0, p1=alcuin_plugin.ShapePoint(x=40.0, y=0.0), p2=alcuin_plugin.ShapePoint(x=-40.0, y=0.0)))
    source_edge_style.set_arrow_head_style(alcuin_plugin.ArrowHeadStylePlugin("arrowhead"))

    target_edge_style = alcuin_plugin.EdgeStylePlugin("target_edge")
    target_edge_style.set_is_frequently_used(False)
    target_edge_style.set_category("SpeciesReference")
    target_edge_style.set_sub_category("Product")
    target_edge_style.set_connectable_source_node_title("Reaction")
    target_edge_style.set_connectable_source_node_categories(["Reaction"])
    target_edge_style.set_connectable_target_node_title("Species")
    target_edge_style.set_connectable_target_node_categories(["Species"])
    target_edge_style.set_name_title("Id")
    target_edge_style.set_is_name_editable(True)
    target_edge_style.add_shape(alcuin_plugin.ConnectedToSourceCentroidShapeLine(border_color="black", border_width=2.0, p1=alcuin_plugin.ShapePoint(x=40.0, y=0.0), p2=alcuin_plugin.ShapePoint(x=-40.0, y=0.0)))
    target_edge_style_arrow_head = alcuin_plugin.ArrowHeadStylePlugin("arrowhead")
    target_edge_style_arrow_head.add_shape(alcuin_plugin.PolygonShape(fill_color="black", border_color="black", border_width=2.0,
                                                                      points=[alcuin_plugin.ShapePoint(-10.0, -5.0),
                                                                              alcuin_plugin.ShapePoint(0.0, 0.0),
                                                                              alcuin_plugin.ShapePoint(-10.0, 5.0),
                                                                              alcuin_plugin.ShapePoint(-10.0, 5.0)]))
    target_edge_style.set_arrow_head_style(target_edge_style_arrow_head)

    intermediary_node_style = alcuin_plugin.NodeStylePlugin("centroid")
    intermediary_node_style.set_category("Reaction")
    intermediary_node_style.set_parent_title("Compartment")
    intermediary_node_style.add_parent_category("Compartment")
    intermediary_node_style.set_name_title("Id")
    intermediary_node_style.set_is_name_editable(True)
    intermediary_node_style.add_shape(alcuin_plugin.CentroidShape())

    template_reaction_edge_style1 = alcuin_plugin.TemplateStylePlugin("UniUni_Reaction")
    template_reaction_edge_style1.set_is_frequently_used(True)
    template_reaction_edge_style1.set_category("TemplateReaction")
    template_reaction_edge_style1.add_source_edge_style(source_edge_style)
    template_reaction_edge_style1.add_target_edge_style(target_edge_style)
    template_reaction_edge_style1.set_intermediary_node_style(intermediary_node_style)

    template_reaction_edge_style2 = alcuin_plugin.TemplateStylePlugin("BiUni_Reaction")
    template_reaction_edge_style2.set_is_frequently_used(True)
    template_reaction_edge_style2.set_category("TemplateReaction")
    template_reaction_edge_style2.add_source_edge_style(source_edge_style)
    template_reaction_edge_style2.add_source_edge_style(source_edge_style)
    template_reaction_edge_style2.add_target_edge_style(target_edge_style)
    template_reaction_edge_style2.set_intermediary_node_style(intermediary_node_style)

    template_reaction_edge_style3 = alcuin_plugin.TemplateStylePlugin("UniBi_Reaction")
    template_reaction_edge_style3.set_is_frequently_used(True)
    template_reaction_edge_style3.set_category("TemplateReaction")
    template_reaction_edge_style3.add_source_edge_style(source_edge_style)
    template_reaction_edge_style3.add_target_edge_style(target_edge_style)
    template_reaction_edge_style3.add_target_edge_style(target_edge_style)
    template_reaction_edge_style3.set_intermediary_node_style(intermediary_node_style)

    template_reaction_edge_style4 = alcuin_plugin.TemplateStylePlugin("BiBi_Reaction")
    template_reaction_edge_style4.set_is_frequently_used(True)
    template_reaction_edge_style4.set_category("TemplateReaction")
    template_reaction_edge_style4.add_source_edge_style(source_edge_style)
    template_reaction_edge_style4.add_source_edge_style(source_edge_style)
    template_reaction_edge_style4.add_target_edge_style(target_edge_style)
    template_reaction_edge_style4.add_target_edge_style(target_edge_style)
    template_reaction_edge_style4.set_intermediary_node_style(intermediary_node_style)

    return (compartment_node_style1.__repr__(),
            species_node_style1.__repr__(),
            species_node_style2.__repr__(),
            species_node_style3.__repr__(),
            species_node_style4.__repr__(),
            reaction_node_style1.__repr__(),
            reaction_node_style2.__repr__(),
            species_reference_edge_style1.__repr__(),
            species_reference_edge_style2.__repr__(),
            species_reference_edge_style3.__repr__(),
            species_reference_edge_style4.__repr__(),
            species_reference_edge_style5.__repr__(),
            template_reaction_edge_style1.__repr__(),
            template_reaction_edge_style2.__repr__(),
            template_reaction_edge_style3.__repr__(),
            template_reaction_edge_style4.__repr__())
