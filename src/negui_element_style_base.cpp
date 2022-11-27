#include "negui_element_style_base.h"
#include <QPainter>
#include <QJsonObject>
#include <QJsonArray>

// MyElementStyleBase

MyElementStyleBase::MyElementStyleBase(const QString& name) : MyPluginItemBase(name) {
    
}

const QString& MyElementStyleBase::convertibleParentCategory() const {
    return _convertibleParentCategory;
}

bool MyElementStyleBase::isConvertibleToParentCategory(QList<QString> parentCategories) {
    for (QString parentCategory : parentCategories) {
        if (parentCategory == _convertibleParentCategory)
            return true;
    }
    
    return false;
}

void MyElementStyleBase::convertToParentCategory() {
    _category = convertibleParentCategory();
}

QList<QString> MyElementStyleBase::parentCategories() {
    return _parentCategories;
}

bool MyElementStyleBase::isConnectableTo(const QString& connectedCategory) {
    for (QString connectableCategory : _connectableCategories) {
        if (connectedCategory == connectableCategory)
            return true;
    }
    
    return false;
}

QList<QString> MyElementStyleBase::connectableCategories() {
    return _connectableCategories;
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
    MyPluginItemBase::read(json);
    _convertibleParentCategory.clear();
    if (json.contains("convertible-parent-category") && json["convertible-parent-category"].isString())
        _convertibleParentCategory = json["convertible-parent-category"].toString();

    _parentCategories.clear();
    if (json.contains("parent-categories") && json["parent-categories"].isArray()) {
        QJsonArray parentCategoriesArray = json["parent-categories"].toArray();
        for (int parentCategoryIndex = 0; parentCategoryIndex < parentCategoriesArray.size(); ++parentCategoryIndex) {
            if (parentCategoriesArray[parentCategoryIndex].isString())
                _parentCategories.push_back(parentCategoriesArray[parentCategoryIndex].toString());
        }
    }
    
    _connectableCategories.clear();
    if (json.contains("connectable-categories") && json["connectable-categories"].isArray()) {
        QJsonArray connectableCategoriesArray = json["connectable-categories"].toArray();
        for (int connectableCategoryIndex = 0; connectableCategoryIndex < connectableCategoriesArray.size(); ++connectableCategoryIndex) {
            if (connectableCategoriesArray[connectableCategoryIndex].isString())
                _connectableCategories.push_back(connectableCategoriesArray[connectableCategoryIndex].toString());
        }
    }
    
    
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
    MyPluginItemBase::write(json);
    
    json["name"] = name();
    json["convertible-parent-category"] = convertibleParentCategory();
    
    QJsonArray parentCategoriesArray;
    for (QString parentCategory : parentCategories())
        parentCategoriesArray.append(parentCategory);
    json["parent-categories"] = parentCategoriesArray;
    
    QJsonArray connectableCategoriesArray;
    for (QString connectableCategory : connectableCategories())
        connectableCategoriesArray.append(connectableCategory);
    json["connectable-categories"] = connectableCategoriesArray;
    
    // shapes
    QJsonArray shapeStylesArray;
    for (MyShapeStyleBase *shapeStyle : qAsConst(shapeStyles())) {
        QJsonObject shapeStyleObject;
        shapeStyle->write(shapeStyleObject);
        shapeStylesArray.append(shapeStyleObject);
    }
    json["shapes"] = shapeStylesArray;
}
