#include "negui_template_style.h"
#include "negui_element_style_builder.h"
#include "negui_element_icon_builder.h"
#include <QJsonObject>
#include <QJsonArray>

// MyTemplateStyle

MyTemplateStyle::MyTemplateStyle(const QString& name) : MyElementStyleBase(name) {
    _intermediaryNodeStyle = NULL;
    _iconSize = QSize(125, 65);
}

const QString MyTemplateStyle::type() const {
    return "templatestyle";
}

MyElementStyleBase* MyTemplateStyle::intermediaryNodeStyle() {
    return _intermediaryNodeStyle;
}

void MyTemplateStyle::deleteIntermediaryNodeStyle() {
    if (_intermediaryNodeStyle)
        delete _intermediaryNodeStyle;
    _intermediaryNodeStyle = NULL;
}

QList<MyElementStyleBase*> MyTemplateStyle::sourceEdgeStyles() {
    return _sourceEdgeStyles;
}

void MyTemplateStyle::clearSourceEdgeStyles() {
    while(_sourceEdgeStyles.size())
        delete _sourceEdgeStyles.takeLast();
}

QList<MyElementStyleBase*> MyTemplateStyle::targetEdgeStyles() {
    return _targetEdgeStyles;
}

void MyTemplateStyle::clearTargetEdgeStyles() {
    while(_targetEdgeStyles.size())
        delete _targetEdgeStyles.takeLast();
}

void MyTemplateStyle::addDefaultShapeStyle() {
    if (intermediaryNodeStyle())
        intermediaryNodeStyle()->addDefaultShapeStyle();
    for (MyElementStyleBase* sourceEdgeStyle : sourceEdgeStyles())
        sourceEdgeStyle->addDefaultShapeStyle();
    for (MyElementStyleBase* targetEdgeStyle : targetEdgeStyles())
        targetEdgeStyle->addDefaultShapeStyle();
}

MyShapeStyleBase* MyTemplateStyle::createShapeStyle(const QString& shape) {
    return NULL;
}

void MyTemplateStyle::clearShapeStyles() {
    if (intermediaryNodeStyle())
        intermediaryNodeStyle()->clearShapeStyles();
    for (MyElementStyleBase* sourceEdgeStyle : sourceEdgeStyles())
        sourceEdgeStyle->clearShapeStyles();
    for (MyElementStyleBase* targetEdgeStyle : targetEdgeStyles())
        targetEdgeStyle->clearShapeStyles();
}

const QRectF MyTemplateStyle::getShapesExtents(QRectF defaultExtents) {
    if (intermediaryNodeStyle())
        defaultExtents = intermediaryNodeStyle()->getShapesExtents(defaultExtents);
    for (MyElementStyleBase* sourceEdgeStyle : sourceEdgeStyles())
        defaultExtents = sourceEdgeStyle->getShapesExtents(defaultExtents);
    for (MyElementStyleBase* targetEdgeStyle : targetEdgeStyles())
        defaultExtents = targetEdgeStyle->getShapesExtents(defaultExtents);
    return defaultExtents;
}

QObject* MyTemplateStyle::createIconBuilder() {
    return new MyTemplateIconBuilder(this);
}

const QString MyTemplateStyle::toolTipText() {
    if (sourceEdgeStyles().size())
        return sourceEdgeStyles().at(0)->toolTipText();
    
    return "Select Node";
}

void MyTemplateStyle::read(const QJsonObject &json) {
    MyPluginItemBase::read(json);
    
    // intermediary node
    deleteIntermediaryNodeStyle();
    if (json.contains("intermediary-node") && json["intermediary-node"].isObject()) {
        _intermediaryNodeStyle = createNodeStyle(name() + "_IntermediaryNode");
        _intermediaryNodeStyle->read(json["intermediary-node"].toObject());
    }
    
    // source edges
    clearSourceEdgeStyles();
    if (json.contains("source-edges") && json["source-edges"].isArray()) {
        QJsonArray sourceEdgeStylesArray = json["source-edges"].toArray();
        MyElementStyleBase* sourceEdgeStyle = NULL;
        for (int sourceEdgeStyleIndex = 0; sourceEdgeStyleIndex < sourceEdgeStylesArray.size(); ++sourceEdgeStyleIndex) {
            QJsonObject sourceEdgeStyleObject = sourceEdgeStylesArray[sourceEdgeStyleIndex].toObject();
            if (sourceEdgeStyleObject.contains("name") && sourceEdgeStyleObject["name"].isString()) {
                sourceEdgeStyle = createEdgeStyle(sourceEdgeStyleObject["name"].toString());
                if (sourceEdgeStyle) {
                    sourceEdgeStyle->read(sourceEdgeStyleObject);
                    _sourceEdgeStyles.push_back(sourceEdgeStyle);
                }
            }
        }
    }
    
    // target edges
    clearTargetEdgeStyles();
    if (json.contains("target-edges") && json["target-edges"].isArray()) {
        QJsonArray targetEdgeStylesArray = json["target-edges"].toArray();
        MyElementStyleBase* targetEdgeStyle = NULL;
        for (int targetEdgeStyleIndex = 0; targetEdgeStyleIndex < targetEdgeStylesArray.size(); ++targetEdgeStyleIndex) {
            QJsonObject targetEdgeStyleObject = targetEdgeStylesArray[targetEdgeStyleIndex].toObject();
            if (targetEdgeStyleObject.contains("name") && targetEdgeStyleObject["name"].isString()) {
                targetEdgeStyle = createEdgeStyle(targetEdgeStyleObject["name"].toString());
                if (targetEdgeStyle) {
                    targetEdgeStyle->read(targetEdgeStyleObject);
                    _targetEdgeStyles.push_back(targetEdgeStyle);
                }
            }
        }
    }
}

void MyTemplateStyle::write(QJsonObject &json) {
    MyPluginItemBase::write(json);
    json["name"] = name();
    
    // intermediary node
    if (intermediaryNodeStyle()) {
        QJsonObject intermediaryNodeObject;
        intermediaryNodeStyle()->write(intermediaryNodeObject);
        json["intermediary-node"] = intermediaryNodeObject;
    }
    
    // source edges
    QJsonArray sourceEdgeStylesArray;
    for (MyElementStyleBase* sourceEdgeStyle : sourceEdgeStyles()) {
        QJsonObject sourceEdgeStyleObject;
        sourceEdgeStyle->write(sourceEdgeStyleObject);
        sourceEdgeStylesArray.append(sourceEdgeStyleObject);
    }
    json["source-edges"] = sourceEdgeStylesArray;
    
    // target edges
    QJsonArray targetEdgeStylesArray;
    for (MyElementStyleBase* targetEdgeStyle : targetEdgeStyles()) {
        QJsonObject targetEdgeStyleObject;
        targetEdgeStyle->write(targetEdgeStyleObject);
        targetEdgeStylesArray.append(targetEdgeStyleObject);
    }
    json["target-edges"] = targetEdgeStylesArray;
}
