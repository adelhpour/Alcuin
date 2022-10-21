#include "negui_element_builder.h"
#include "negui_node.h"
#include "negui_edge.h"
#include "negui_arrow_head.h"

MyElementBase* createNode(const QString& name, const qreal& x, const qreal& y) {
    return new MyNode(name, x, y);
}

MyElementBase* createEdge(const QString& name, MyElementBase* startNode, MyElementBase* endNode) {
    return new MyEdge(name, startNode, endNode);
}

MyElementBase* createArrowHead(const QString& name, MyElementBase* edge) {
    return new MyArrowHead(name, edge);
}
