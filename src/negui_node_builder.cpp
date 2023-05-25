#include "negui_node_builder.h"
#include "negui_node_style_builder.h"
#include "negui_node.h"
#include "negui_node_style.h"

MyNetworkElementBase* createNode(const QString& name, MyElementStyleBase* nodeStyle, const qreal& x, const qreal& y) {
    MyNetworkElementBase* node = NULL;
    if (((MyNodeStyleBase*)nodeStyle)->nodeStyleType() == MyNodeStyleBase::CLASSIC_NODE_STYLE)
        node = new MyClassicNode(name, x, y);
    else if(((MyNodeStyleBase*)nodeStyle)->nodeStyleType() == MyNodeStyleBase::CENTROID_NODE_STYLE)
        node = new MyCentroidNode(name, x, y);

    if (node)
        node->setStyle(nodeStyle);

    return node;
}

MyNetworkElementBase* createNode(const QJsonObject &json) {
    MyNetworkElementBase* node = NULL;
    if (json.contains("id") && json["id"].isString() && json.contains("position") && json["position"].isObject() && json["position"].toObject().contains("x") && json["position"]["x"].isDouble() && json["position"].toObject().contains("y") && json["position"]["y"].isDouble()) {
        if (json.contains("style") && json["style"].isObject()) {
            MyElementStyleBase* nodeStyle = createNodeStyle(json["style"].toObject());
            if (nodeStyle) {
                node = createNode(json["id"].toString(), nodeStyle, json["position"]["x"].toDouble(), json["position"]["y"].toDouble());
                if (json.contains("parent") && json["parent"].isString())
                    ((MyClassicNode*)node)->setParentNodeId(json["parent"].toString());
            }
        }
    }

    return node;
}