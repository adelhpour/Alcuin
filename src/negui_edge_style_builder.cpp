#include "negui_edge_style_builder.h"
#include "negui_edge_style.h"

#include <QJsonArray>

MyNetworkElementStyleBase* createEdgeStyle(const QJsonObject &json) {
    MyNetworkElementStyleBase* style = NULL;
    if (json.contains("name") && json["name"].isString()) {
        if (isConnectedToSourceCentroidNode(json))
            style = new MyConnectedToSourceCentroidNodeEdgeStyle(json["name"].toString());
        else if (isConnectedToTargetCentroidNode(json))
            style = new MyConnectedToTargetCentroidNodeEdgeStyle(json["name"].toString());
        else
            style = new MyClassicEdgeStyle(json["name"].toString());
        style->read(json);
    }

    return style;
}

const bool isConnectedToSourceCentroidNode(const QJsonObject &json) {
    if (json.contains("shapes") && json["shapes"].isArray()) {
        QJsonArray shapesArray = json["shapes"].toArray();
        for (int shapeIndex = 0; shapeIndex < shapesArray.size(); ++shapeIndex) {
            QJsonObject shapeObject = shapesArray[shapeIndex].toObject();
            if (shapeObject.contains("shape") && shapeObject["shape"].isString()) {
                if (shapeObject["shape"].toString() == "connected-to-source-centroid-shape-line")
                    return  true;
            }
        }
    }

    return false;
}

const bool isConnectedToTargetCentroidNode(const QJsonObject &json) {
    if (json.contains("shapes") && json["shapes"].isArray()) {
        QJsonArray shapesArray = json["shapes"].toArray();
        for (int shapeIndex = 0; shapeIndex < shapesArray.size(); ++shapeIndex) {
            QJsonObject shapeObject = shapesArray[shapeIndex].toObject();
            if (shapeObject.contains("shape") && shapeObject["shape"].isString()) {
                if (shapeObject["shape"].toString() == "connected-to-target-centroid-shape-line")
                    return  true;
            }
        }
    }

    return false;
}