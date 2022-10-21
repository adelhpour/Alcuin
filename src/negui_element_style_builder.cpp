#include "negui_element_style_builder.h"
#include "negui_node_style.h"
#include "negui_edge_style.h"
#include "negui_arrow_head_style.h"

MyElementStyleBase* createNodeStyle(const QString& name) {
    return new MyNodeStyle(name);
}

MyElementStyleBase* createEdgeStyle(const QString& name) {
    return new MyEdgeStyle(name);
}

MyElementStyleBase* createArrowHeadStyle(const QString& name) {
    return new MyArrowHeadStyle(name);
}
