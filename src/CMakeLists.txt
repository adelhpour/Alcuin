set(CMAKE_AUTOMOC ON)

### build the library
## set the required sources and headers
set(source_files
		negui_main_widget.cpp
		negui_toolbar.cpp
		negui_graphics_view.cpp
		negui_secondary_graphics_view.cpp
		negui_graphics_scene.cpp
		negui_status_bar.cpp
		negui_scene_mode_element_base.cpp
		negui_interactor.cpp
		negui_file_manager.cpp
		negui_base.cpp
		negui_plugin_item_base.cpp
		negui_plugin_item_builder.cpp
		negui_plugin_manager.cpp
		negui_network_manager.cpp
		negui_default_network_element_style_manager.cpp
		negui_network_element_base.cpp
		negui_node.cpp
		negui_edge.cpp
		negui_arrow_head.cpp
		negui_node_builder.cpp
		negui_edge_builder.cpp
		negui_arrow_head_builder.cpp
		negui_copied_network_elements_paster.cpp
		negui_new_edge_builder.cpp
		negui_network_element_aligner.cpp
		negui_network_element_aligner_builder.cpp
		negui_network_element_mover.cpp
		negui_network_element_selector.cpp
		negui_selection_area_graphics_item.cpp
		negui_network_element_style_base.cpp
		negui_template_style.cpp
		negui_node_style.cpp
		negui_edge_style.cpp
		negui_arrow_head_style.cpp
		negui_node_style_builder.cpp
		negui_edge_style_builder.cpp
		negui_arrow_head_style_builder.cpp
		negui_template_style_builder.cpp
		negui_network_element_icon_builder.cpp
		negui_network_element_graphics_item_base.cpp
		negui_node_graphics_item.cpp
		negui_edge_graphics_item.cpp
		negui_arrow_head_graphics_item.cpp
		negui_node_graphics_item_builder.cpp
		negui_edge_graphics_item_builder.cpp
		negui_arrow_head_graphics_item_builder.cpp
		negui_shape_style_base.cpp
		negui_1d_shape_style_base.cpp
		negui_2d_shape_style_base.cpp
		negui_ellipse_style.cpp
		negui_rectangle_style.cpp
		negui_polygon_style.cpp
		negui_line_style.cpp
		negui_text_style.cpp
		negui_centroid_style.cpp
		negui_shape_style_builder.cpp
		negui_shape_graphics_item_base.cpp
		negui_1d_shape_graphics_item_base.cpp
		negui_2d_shape_graphics_item_base.cpp
		negui_ellipse_graphics_item.cpp
		negui_rectangle_graphics_item.cpp
		negui_polygon_graphics_item.cpp
		negui_line_graphics_item.cpp
		negui_text_graphics_item.cpp
		negui_centroid_graphics_item.cpp
		negui_shape_graphics_item_builder.cpp
		negui_resize_handled_graphics_item.cpp
		negui_bezier_adjust_customized_graphics_items.cpp
		negui_bezier_adjust_handled_graphics_item.cpp
		negui_centroid_adjust_handled_graphics_item.cpp
		negui_parameters.cpp
		negui_customized_common_widgets.cpp
		negui_customized_context_menu_widgets.cpp
		negui_customized_feature_menu_widgets.cpp
		negui_customized_menu_button_widgets.cpp
		negui_customized_interactor_widgets.cpp
		negui_menu_button_manager.cpp
		negui_menu_button_builder.cpp
		negui_decorate_menu_buttons.cpp
		negui_mode_menu.cpp
		negui_feature_menu_base.cpp
		negui_network_element_feature_menu.cpp
		negui_null_feature_menu.cpp
		negui_multi_network_element_feature_menu.cpp
		negui_context_menu.cpp
		negui_import_tools.cpp
		negui_export_tools.cpp
		negui_autolayout_engines.cpp
		negui_main_widget_builder.cpp
		)

set(header_files
        negui_main_widget.h
		negui_toolbar.h
		negui_graphics_view.h
		negui_secondary_graphics_view.h
		negui_graphics_scene.h
		negui_status_bar.h
		negui_scene_mode_element_base.h
		negui_interactor.h
		negui_file_manager.h
		negui_base.h
		negui_plugin_item_base.h
		negui_plugin_item_builder.h
		negui_plugin_manager.h
		negui_network_manager.h
		negui_default_network_element_style_manager.h
		negui_network_element_base.h
		negui_node.h
		negui_edge.h
		negui_arrow_head.h
		negui_node_builder.h
		negui_edge_builder.h
		negui_arrow_head_builder.h
        negui_copied_network_elements_paster.h
		negui_new_edge_builder.h
		negui_network_element_aligner.h
		negui_network_element_aligner_builder.h
		negui_network_element_mover.h
		negui_network_element_selector.h
		negui_selection_area_graphics_item.h
		negui_network_element_style_base.h
		negui_template_style.h
		negui_node_style.h
		negui_edge_style.h
		negui_arrow_head_style.h
		negui_node_style_builder.h
		negui_edge_style_builder.h
		negui_arrow_head_style_builder.h
		negui_template_style_builder.h
		negui_network_element_icon_builder.h
		negui_network_element_graphics_item_base.h
		negui_node_graphics_item.h
		negui_edge_graphics_item.h
		negui_arrow_head_graphics_item.h
		negui_node_graphics_item_builder.h
		negui_edge_graphics_item_builder.h
		negui_arrow_head_graphics_item_builder.h
		negui_shape_style_base.h
		negui_1d_shape_style_base.h
		negui_2d_shape_style_base.h
		negui_ellipse_style.h
		negui_rectangle_style.h
		negui_polygon_style.h
		negui_line_style.h
		negui_text_style.h
		negui_centroid_style.h
		negui_shape_style_builder.h
		negui_shape_graphics_item_base.h
		negui_1d_shape_graphics_item_base.h
		negui_2d_shape_graphics_item_base.h
		negui_ellipse_graphics_item.h
		negui_rectangle_graphics_item.h
		negui_polygon_graphics_item.h
		negui_line_graphics_item.h
		negui_text_graphics_item.h
		negui_centroid_graphics_item.h
		negui_shape_graphics_item_builder.h
		negui_resize_handled_graphics_item.h
		negui_bezier_adjust_customized_graphics_items.h
		negui_bezier_adjust_handled_graphics_item.h
		negui_centroid_adjust_handled_graphics_item.h
		negui_parameters.h
		negui_customized_common_widgets.h
		negui_customized_context_menu_widgets.h
        negui_customized_feature_menu_widgets.h
		negui_customized_menu_button_widgets.h
		negui_customized_interactor_widgets.h
		negui_menu_button_manager.h
		negui_menu_button_builder.h
		negui_decorate_menu_buttons.h
		negui_mode_menu.h
		negui_feature_menu_base.h
		negui_network_element_feature_menu.h
		negui_null_feature_menu.h
		negui_multi_network_element_feature_menu.h
		negui_context_menu.h
		negui_interfaces.h
		negui_import_tools.h
		negui_export_tools.h
		negui_autolayout_engines.h
        negui_main_widget_builder.h
		)

## build
add_library(Alcuin SHARED ${source_files} ${header_files})

## link libraries
target_link_libraries(Alcuin
		Qt5::Core
		Qt5::Gui
		Qt5::Widgets
		Qt5::PrintSupport
		Qt5::Svg)

## set library dist
install(TARGETS Alcuin RUNTIME DESTINATION bin COMPONENT alcuin_lib LIBRARY DESTINATION lib COMPONENT alcuin_lib ARCHIVE DESTINATION lib COMPONENT alcuin_lib)

## install headers
file(GLOB headers ${CMAKE_SOURCE_DIR}/src/*.h)
install(FILES ${headers} DESTINATION include COMPONENT alcuin_lib)

## install resources
install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_INSTALL_PREFIX})

add_subdirectory(plugins)