#include "negui_element_base.h"
#include "negui_feature_menu.h"

// MyElementBase

MyElementBase::MyElementBase(const QString& name) : MyBase(name) {
    _style = NULL;
    _isActive = false;
    _isSelected = false;
}

MyElementGraphicsItemBase* MyElementBase::graphicsItem() {
    return _graphicsItem;
}

void MyElementBase::updateGraphicsItem() {
    graphicsItem()->update(style()->shapeStyles(), calculateZValue());
}

MyElementStyleBase* MyElementBase::style() {
    return _style;
}

void MyElementBase::setStyle(MyElementStyleBase* style) {
    _style = style;
}

void MyElementBase::updateStyle(QList<MyShapeStyleBase*> shapeStyles) {
    style()->setShapeStyles(shapeStyles);
}

const QString MyElementBase::styleCategory() {
    if (!style()->category().isEmpty())
        return style()->category();
    
    return "N/A";
}

bool MyElementBase::setActive(const bool& active) {
    return _isActive = active;
}

void MyElementBase::setSelected(const bool& selected) {
    _isSelected = selected;
}

const bool MyElementBase::isSelected() {
    return _isSelected;
}

void MyElementBase::enableNormalMode() {
    setSelected(false);
    graphicsItem()->enableNormalMode();
}

void MyElementBase::enableAddNodeMode() {
    graphicsItem()->enableAddNodeMode();
}

void MyElementBase::enableSelectNodeMode() {
    graphicsItem()->enableSelectNodeMode();
}

void MyElementBase::enableAddEdgeMode() {
    graphicsItem()->enableAddEdgeMode();
}

void MyElementBase::enableSelectEdgeMode() {
    graphicsItem()->enableSelectEdgeMode();
}

void MyElementBase::enableRemoveMode() {
    graphicsItem()->enableRemoveMode();
}

QWidget* MyElementBase::getFeatureMenu() {
    MyMenuItemFrame* featureMenu = new MyMenuItemFrame();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();
    
    // title
    contentLayout->addWidget(new MyTitleLabel(styleCategory()), contentLayout->rowCount(), 0, 1, 2, Qt::AlignCenter);
    
    // spacer
    QLayoutItem* spacerItem = new MySpacerItem(0, 10);
    contentLayout->addItem(spacerItem, contentLayout->rowCount(), 0, 1, 2);
    
    // name
    contentLayout->addWidget(new MyLabel("Name"), contentLayout->rowCount(), 0);
    contentLayout->addWidget(new MyReadOnlyLineEdit(name()), contentLayout->rowCount() - 1, 1);

    return featureMenu;
}

void MyElementBase::createFeatureMenu() {
    MyFeatureMenu* featureMenu =  new MyFeatureMenu(getFeatureMenu());
    featureMenu->setShapeStyles(style()->shapeStyles());
    connect(featureMenu, &MyFeatureMenu::isUpdated, this, [this] (QList<MyShapeStyleBase*> shapeStyles) {
        updateStyle(shapeStyles);
        updateGraphicsItem();
        emit askForCreateChangeStageCommand(); } );
    emit askForDisplayFeatureMenu(featureMenu);
}
