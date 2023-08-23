#include "negui_node_style_builder.h"
#include "negui_node_style.h"
#include <QJsonArray>

MyNetworkElementStyleBase* createNodeStyle(const QJsonObject &json) {
    MyNetworkElementStyleBase* style = NULL;
    if (json.contains("name") && json["name"].isString()) {
        if (isCentroidNodeStyle(json))
            style = new MyCentroidNodeStyle(json["name"].toString());
        else if (isSimpleClassicNodeStyle(json))
            style = new MySimpleClassicNodeStyle(json["name"].toString());
        else
            style = new MyComplexClassicNodeStyle(json["name"].toString());
        style->read(json);
    }

    return style;
}

const bool isCentroidNodeStyle(const QJsonObject &json) {
    if (json.contains("shapes") && json["shapes"].isArray()) {
        QJsonArray shapesArray = json["shapes"].toArray();
        for (int shapeIndex = 0; shapeIndex < shapesArray.size(); ++shapeIndex) {
            QJsonObject shapeObject = shapesArray[shapeIndex].toObject();
            if (shapeObject.contains("shape") && shapeObject["shape"].isString()) {
                if (shapeObject["shape"].toString() == "centroid")
                    return  true;
            }
        }
    }

    return  false;
}

const bool isSimpleClassicNodeStyle(const QJsonObject &json) {
    if (json.contains("shapes") && json["shapes"].isArray())
        return whetherHaveTwoShapesWithOneOfThemATextShape(json["shapes"].toArray());

    return  false;
}

const bool whetherHaveTwoShapesWithOneOfThemATextShape(const QJsonArray& shapesArray) {
    if (shapesArray.size() == 2) {
        for (unsigned int shapeIndex = 0; shapeIndex < shapesArray.size(); shapeIndex++) {
            QJsonObject shapeObject = shapesArray[shapeIndex].toObject();
            if (shapeObject.contains("shape") && shapeObject["shape"].isString()) {
                if (shapeObject["shape"].toString() == "text")
                    return true;
            }
        }
    }

    return false;
}