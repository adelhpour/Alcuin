#include "negui_network_element_style_base.h"
#include "negui_element_icon_builder.h"
#include <QJsonObject>
#include <QJsonArray>

// MyNetworkElementStyleBase

MyNetworkElementStyleBase::MyNetworkElementStyleBase(const QString& name) : MyPluginItemBase(name) {
    
}

void MyNetworkElementStyleBase::setShapeStyles(QList<MyShapeStyleBase*> shapeStyles) {
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

QList<MyShapeStyleBase*>& MyNetworkElementStyleBase::shapeStyles() {
    return _shapeStyles;
}

void MyNetworkElementStyleBase::clearShapeStyles() {
    while(_shapeStyles.size())
        delete _shapeStyles.takeLast();
}

const QRectF MyNetworkElementStyleBase::getShapesExtents(QRectF defaultExtents) {
    qreal extentsX = defaultExtents.x();
    qreal extentsY = defaultExtents.y();
    qreal extentsWidth = defaultExtents.width();
    qreal extentsHeight = defaultExtents.height();
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

const QIcon MyNetworkElementStyleBase::icon() {
    QObject* iconBuilder = createIconBuilder();
    ((MyElementIconBuilderBase*)iconBuilder)->build();
    return ((MyElementIconBuilderBase*)iconBuilder)->icon();
}

void MyNetworkElementStyleBase::read(const QJsonObject &json) {
    MyPluginItemBase::read(json);
    // shapes
    clearShapeStyles();
    if (json.contains("shapes") && json["shapes"].isArray()) {
        QJsonArray shapeStylesArray = json["shapes"].toArray();
        MyShapeStyleBase* shapeStyle = NULL;
        for (int shapeStyleIndex = 0; shapeStyleIndex < shapeStylesArray.size(); ++shapeStyleIndex) {
            QJsonObject shapeStyleObject = shapeStylesArray[shapeStyleIndex].toObject();
            if (shapeStyleObject.contains("shape") && shapeStyleObject["shape"].isString()) {
                shapeStyle = createShapeStyle(shapeStyleObject["shape"].toString());
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

void MyNetworkElementStyleBase::write(QJsonObject &json) {
    MyPluginItemBase::write(json);
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
