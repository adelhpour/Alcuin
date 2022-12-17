#include "negui_element_style_base.h"
#include <QPainter>
#include <QJsonObject>
#include <QJsonArray>

// MyElementStyleBase

MyElementStyleBase::MyElementStyleBase(const QString& name) : MyPluginItemBase(name) {
    _addRemoveShapeStylesButtons = NULL;
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

bool MyElementStyleBase::isConnectableToStartNodeCategory(const QString& connectedStartNodeCategory) {
    return false;
}

bool MyElementStyleBase::isConnectableToEndNodeCategory(const QString& connectedEndNodeCategory) {
    return false;
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

const QRectF MyElementStyleBase::getShapesExtents() {
    qreal extentsX = 0.0;
    qreal extentsY = 0.0;
    qreal extentsWidth = 0.0;
    qreal extentsHeight = 0.0;
    for (MyShapeStyleBase* shapeStyle : qAsConst(shapeStyles())) {
        QRectF shapeStyleExtents = shapeStyle->getShapeExtents();
        if (shapeStyleExtents.x() < extentsX) {
            extentsWidth += extentsX - shapeStyleExtents.x();
            extentsX = shapeStyleExtents.x();
        }
        if (shapeStyleExtents.y() < extentsY) {
            extentsHeight += extentsY - shapeStyleExtents.y();
            extentsY = shapeStyleExtents.y();
        }
        if (extentsX + extentsWidth < shapeStyleExtents.x() + shapeStyleExtents.width())
            extentsWidth += shapeStyleExtents.x() + shapeStyleExtents.width() - extentsX - extentsWidth;
        if (extentsY + extentsHeight < shapeStyleExtents.y() + shapeStyleExtents.height())
            extentsHeight += shapeStyleExtents.y() + shapeStyleExtents.height() - extentsY - extentsHeight;
    }
    return QRectF(extentsX, extentsY, extentsWidth, extentsHeight);
}

QDialogButtonBox* MyElementStyleBase::getAddRemoveShapeStylesButtons() {
    return _addRemoveShapeStylesButtons;
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
    
    // shapes
    QJsonArray shapeStylesArray;
    for (MyShapeStyleBase *shapeStyle : qAsConst(shapeStyles())) {
        QJsonObject shapeStyleObject;
        shapeStyle->write(shapeStyleObject);
        shapeStylesArray.append(shapeStyleObject);
    }
    json["shapes"] = shapeStylesArray;
}

// MyAddRemoveShapeStylesButtonsBase

MyAddRemoveShapeStylesButtonsBase::MyAddRemoveShapeStylesButtonsBase(QWidget* parent) : QDialogButtonBox(parent) {
    setContentsMargins(0, 0, 0, 0);
    setOrientation(Qt::Horizontal);
    setFixedSize(150.0, 50.0);
    
    // add button
    _addPushButton = addButton(QString("+"), QDialogButtonBox::YesRole);
    _addingMenu = new QMenu(_addPushButton);
    _addPushButton->setMenu(_addingMenu);
    //setAddingMenu();
    
    // remove button
    _removePushButton = addButton(QString("-"), QDialogButtonBox::NoRole);
    _removingMenu = new QMenu(_removePushButton);
    _removePushButton->setMenu(_removingMenu);
}

void MyAddRemoveShapeStylesButtonsBase::setRemovingMenu(QList<MyShapeStyleBase*> shapeStyles) {
    _removingMenu->clear();
    if (shapeStyles.size() > 1) {
        _removePushButton->setEnabled(true);
        for (MyShapeStyleBase* shapeStyle : qAsConst(shapeStyles)) {
            connect(_removingMenu->addAction(shapeStyle->name()), &QAction::triggered, this, [this, shapeStyle] () { emit askForRemoveShapeStyle(shapeStyle); });
        }
    }
    else
        _removePushButton->setEnabled(false);
}
