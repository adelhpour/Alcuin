#include "negui_network_element_base.h"
#include "negui_feature_menu.h"

#include <QGridLayout>

// MyNetworkElementBase

MyNetworkElementBase::MyNetworkElementBase(const QString& name) : MyBase(name) {
    _style = NULL;
    _isActive = false;
    _isSelected = false;
}

MyNetworkElementGraphicsItemBase* MyNetworkElementBase::graphicsItem() {
    return _graphicsItem;
}

void MyNetworkElementBase::updateGraphicsItem() {
    graphicsItem()->update(style()->shapeStyles(), calculateZValue());
}

void MyNetworkElementBase::connectGraphicsItem() {
    connect(_graphicsItem, &MyNetworkElementGraphicsItemBase::mouseLeftButtonIsPressed, this, [this] () { emit elementObject(this); });
    connect(_graphicsItem, &MyNetworkElementGraphicsItemBase::askForDeleteNetworkElement, this, [this] () { emit askForDeleteNetworkElement(this); });
    connect(_graphicsItem, SIGNAL(askForWhetherGraphicsItemIsSelected()), this, SLOT(isSelected()));
    connect(_graphicsItem, SIGNAL(askForCreateFeatureMenu()), this, SLOT(createFeatureMenu()));
    connect(_graphicsItem, &MyNetworkElementGraphicsItemBase::askForCopyNetworkElement, this, [this] () { emit askForCopyNetworkElement(this); } );
    connect(_graphicsItem, &MyNetworkElementGraphicsItemBase::askForCutNetworkElement, this, [this] () { emit askForCutNetworkElement(this); } );
    connect(_graphicsItem, &MyNetworkElementGraphicsItemBase::askForCopyNetworkElementStyle, this, [this] () { emit askForCopyNetworkElementStyle(this->style()); } );
    connect(_graphicsItem, &MyNetworkElementGraphicsItemBase::askForPasteNetworkElementStyle, this, [this] () { emit askForPasteNetworkElementStyle(this); } );
    connect(_graphicsItem, SIGNAL(askForWhetherAnyOtherElementsAreSelected()), this, SLOT(areAnyOtherElementsSelected()));
    connect(_graphicsItem, SIGNAL(askForWhetherElementStyleIsCopied()), this, SIGNAL(askForWhetherElementStyleIsCopied()));
    connect(_graphicsItem, SIGNAL(askForCreateChangeStageCommand()), this, SIGNAL(askForCreateChangeStageCommand()));
}

MyNetworkElementStyleBase* MyNetworkElementBase::style() {
    return _style;
}

void MyNetworkElementBase::setStyle(MyNetworkElementStyleBase* style) {
    _style = style;
}

void MyNetworkElementBase::updateStyle(QList<MyShapeStyleBase*> shapeStyles) {
    style()->setShapeStyles(shapeStyles);
}

const QString MyNetworkElementBase::styleCategory() {
    if (!style()->category().isEmpty())
        return style()->category();
    
    return "N/A";
}

bool MyNetworkElementBase::setActive(const bool& active) {
    return _isActive = active;
}

void MyNetworkElementBase::setSelected(const bool& selected) {
    _isSelected = selected;
}

const bool MyNetworkElementBase::isSelected() {
    return _isSelected;
}

const bool MyNetworkElementBase::areAnyOtherElementsSelected() {
    return askForWhetherAnyOtherElementsAreSelected(this);
}

void MyNetworkElementBase::enableNormalMode() {
    MySceneModeElementBase::enableNormalMode();
    setSelected(false);
    graphicsItem()->enableNormalMode();
}

void MyNetworkElementBase::enableAddNodeMode() {
    MySceneModeElementBase::enableAddNodeMode();
    graphicsItem()->enableAddNodeMode();
}

void MyNetworkElementBase::enableSelectNodeMode() {
    MySceneModeElementBase::enableSelectNodeMode();
    graphicsItem()->enableSelectNodeMode();
}

void MyNetworkElementBase::enableAddEdgeMode() {
    MySceneModeElementBase::enableAddEdgeMode();
    graphicsItem()->enableAddEdgeMode();
}

void MyNetworkElementBase::enableSelectEdgeMode() {
    MySceneModeElementBase::enableSelectEdgeMode();
    graphicsItem()->enableSelectEdgeMode();
}

QWidget* MyNetworkElementBase::getFeatureMenu() {
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

void MyNetworkElementBase::createFeatureMenu() {
    if (getSceneMode() == NORMAL_MODE) {
        MyFeatureMenu* featureMenu =  new MyFeatureMenu(getFeatureMenu());
        featureMenu->setShapeStyles(style()->shapeStyles());
        connect(featureMenu, &MyFeatureMenu::isUpdated, this, [this] (QList<MyShapeStyleBase*> shapeStyles) {
            updateStyle(shapeStyles);
            updateGraphicsItem();
            emit askForCreateChangeStageCommand(); } );
        emit askForDisplayFeatureMenu(featureMenu);
    }
}
