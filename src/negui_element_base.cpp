#include "negui_element_base.h"
#include "negui_feature_menu.h"

#include <QGridLayout>

// MyElementBase

MyElementBase::MyElementBase(const QString& name) : MyBase(name) {
    _style = NULL;
    _isActive = false;
    _isSelected = false;
    _canDisplayFeatureMenu = false;
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
    _canDisplayFeatureMenu = true;
    graphicsItem()->enableNormalMode();
}

void MyElementBase::enableAddNodeMode() {
    _canDisplayFeatureMenu = false;
    graphicsItem()->enableAddNodeMode();
}

void MyElementBase::enableSelectNodeMode() {
    _canDisplayFeatureMenu = false;
    graphicsItem()->enableSelectNodeMode();
}

void MyElementBase::enableAddEdgeMode() {
    _canDisplayFeatureMenu = false;
    graphicsItem()->enableAddEdgeMode();
}

void MyElementBase::enableSelectEdgeMode() {
    _canDisplayFeatureMenu = false;
    graphicsItem()->enableSelectEdgeMode();
}

void MyElementBase::enableRemoveMode() {
    _canDisplayFeatureMenu = false;
    graphicsItem()->enableRemoveMode();
}

QWidget* MyElementBase::getFeatureMenu() {
    MyFeatureMenuItemFrame* featureMenu = new MyFeatureMenuItemFrame();
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
    if (_canDisplayFeatureMenu) {
        MyFeatureMenu* featureMenu =  new MyFeatureMenu(getFeatureMenu());
        featureMenu->setShapeStyles(style()->shapeStyles());
        connect(featureMenu, &MyFeatureMenu::isUpdated, this, [this] (QList<MyShapeStyleBase*> shapeStyles) {
            updateStyle(shapeStyles);
            updateGraphicsItem();
            emit askForCreateChangeStageCommand(); } );
        emit askForDisplayFeatureMenu(featureMenu);
    }
}
