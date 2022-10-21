#include "negui_element_style_base.h"
#include <QPainter>
#include <QJsonObject>
#include <QJsonArray>

// MyElementStyleBase

MyElementStyleBase::MyElementStyleBase(const QString& name) : MyPluginItemBase(name) {
    
}

void MyElementStyleBase::setShapeStyles(QList<MyShapeStyleBase*> shapeStyles) {
    // delete the previous styles which are removed now
    QList<MyShapeStyleBase*> removedShapeStyles;
    bool shapeStyleExistsInNewList = false;
    for (MyShapeStyleBase* previousShapeStyle : qAsConst(_shapeStyles)) {
        shapeStyleExistsInNewList = false;
        for (MyShapeStyleBase* newShapeStyle : qAsConst(shapeStyles)) {
            if (previousShapeStyle == newShapeStyle) {
                shapeStyleExistsInNewList = true;
                break;
            }
        }
        if (!shapeStyleExistsInNewList)
            removedShapeStyles.push_back(previousShapeStyle);
    }
    
    for (MyShapeStyleBase* removedShapeStyle : qAsConst(removedShapeStyles))
        _shapeStyles.removeOne(removedShapeStyle);
    
    _shapeStyles = shapeStyles;
}

QList<MyShapeStyleBase*>& MyElementStyleBase::shapeStyles() {
    return _shapeStyles;
}

void MyElementStyleBase::clearShapeStyles() {
    while(_shapeStyles.size())
        delete _shapeStyles.takeLast();
}

const QIcon MyElementStyleBase::icon() {
    QList<MyElementGraphicsItemBase*> items = getElementIconGraphicsItems();
    
    QRectF extents;
    extents.setX(INT_MAX);
    extents.setY(INT_MAX);
    
    for (QGraphicsItem* item : qAsConst(items)) {
        if (item->boundingRect().x() < extents.x())
            extents.setX(item->boundingRect().x());
        if (item->boundingRect().y() < extents.y())
            extents.setY(item->boundingRect().y());
        if (extents.x() + extents.width() < item->boundingRect().x() + item->boundingRect().width())
            extents.setWidth(extents.width() + (item->boundingRect().x() + item->boundingRect().width() - extents.x() - extents.width()));
        if (extents.y() + extents.height() < item->boundingRect().y() + item->boundingRect().height())
            extents.setHeight(extents.height() + (item->boundingRect().y() + item->boundingRect().height() - extents.y() - extents.height()));
    }
    
    QPixmap pixMap(extents.width(), extents.height());
    pixMap.fill(Qt::transparent);
    QPainter painter(&pixMap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    painter.translate(-1 * extents.x(),-1 * extents.y());
    QStyleOptionGraphicsItem opt;
    
    for (QGraphicsItem* item : qAsConst(items)) {
        if (QGraphicsItemGroup* group = qgraphicsitem_cast<QGraphicsItemGroup *>(item)) {
            QList<QGraphicsItem*> children = group->childItems();
            for (QGraphicsItem *child : qAsConst(children))
                child->paint(&painter, &opt);
        }
        else
            item->paint(&painter, &opt);
    }
    
    return QIcon(pixMap);
}

void MyElementStyleBase::read(const QJsonObject &json) {
    // shapes
    clearShapeStyles();
    if (json.contains("shapes") && json["shapes"].isArray()) {
        QJsonArray shapeStylesArray = json["shapes"].toArray();
        MyShapeStyleBase* shapeStyle = NULL;
        for (int shapeStyleIndex = 0; shapeStyleIndex < shapeStylesArray.size(); ++shapeStyleIndex) {
            QJsonObject shapeStyleObject = shapeStylesArray[shapeStyleIndex].toObject();
            if (shapeStyleObject.contains("shape") && shapeStyleObject["shape"].isString()) {
                MyShapeStyleBase* shapeStyle = createShapeStyle(shapeStyleObject["shape"].toString());
                if (shapeStyle) {
                    shapeStyle->read(shapeStyleObject);
                    _shapeStyles.push_back(shapeStyle);
                }
            }
        }
    }
    if (!shapeStyles().size())
        addDefaultShapeStyle();
}

void MyElementStyleBase::write(QJsonObject &json) {
    json["name"] = name();
    
    // shapes
    QJsonArray shapeStylesArray;
    for (MyShapeStyleBase *shapeStyle : qAsConst(shapeStyles())) {
        QJsonObject shapeStyleObject;
        shapeStyle->write(shapeStyleObject);
        shapeStylesArray.append(shapeStyleObject);
    }
    json["shapes"] = shapeStylesArray;
}
