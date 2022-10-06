#include "negui_element_base.h"

// MyElementBase

MyElementBase::MyElementBase(const QString& name) : MyBase(name) {
    
}

MyElementGraphicsItemBase* MyElementBase::graphicsItem() {
    return _graphicsItem;
}


void MyElementBase::updateGraphicsItem() {
    graphicsItem()->clear();
    graphicsItem()->addShapeItems(style()->shapeStyles(), calculateZValue());
}

MyElementStyleBase* MyElementBase::style() {
    return _style;
}

void MyElementBase::setStyle(MyElementStyleBase* style) {
    if (style)
        _style = style;
    
    updateGraphicsItem();
}

const bool MyElementBase::isSelected() {
    return graphicsItem()->isSelected();
}

void MyElementBase::enableNormalMode() {
    setSelected(false);
}

void MyElementBase::setShapeStyles(QList<MyShapeStyleBase*> shapeStyles) {
    style()->setShapeStyles(shapeStyles);
    updateGraphicsItem();
}

QWidget* MyElementBase::getFeatureMenu() {
    MyMenuItemGroupBox* featureMenu = new MyMenuItemGroupBox();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();
    
    // title
    contentLayout->addWidget(new MyTitleLabel(typeAsString()), contentLayout->rowCount(), 0, 1, 2, Qt::AlignCenter);
    
    // spacer
    QLayoutItem* spacerItem = new MySpacerItem(0, 10);
    contentLayout->addItem(spacerItem, contentLayout->rowCount(), 0, 1, 2);
    
    // name
    contentLayout->addWidget(new MyLabel("Name"), contentLayout->rowCount(), 0);
    contentLayout->addWidget(new MyReadOnlyLineEdit(name()), contentLayout->rowCount() - 1, 1);
    
    return featureMenu;
}
