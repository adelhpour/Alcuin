#include "negui_network_element_feature_menu.h"

#include <QGridLayout>

// MySingleNetworkElementFeatureMenu

MySingleNetworkElementFeatureMenu::MySingleNetworkElementFeatureMenu(QWidget* elementFeatureMenu, const QString& iconsDirectoryPath, QWidget *parent) : MyFeatureMenuBase(iconsDirectoryPath, parent) {
    _expandableWidgetSize = QSize(0, 0);
    QGridLayout* contentLayout = (QGridLayout*)layout();

    // element feature menu
    _elementFeatureMenu = elementFeatureMenu;
    connect(_elementFeatureMenu, SIGNAL(askForAddShapeStyle(MyShapeStyleBase*)), this, SLOT(addNewShapeStyle(MyShapeStyleBase*)));
    connect(_elementFeatureMenu, SIGNAL(askForRemoveShapeStyle(MyShapeStyleBase*)), this, SLOT(removeShapeStyle(MyShapeStyleBase*)));
    connect(_elementFeatureMenu, SIGNAL(askForChangeShapeStyle(MyShapeStyleBase*)), this, SLOT(changeShapeStyle(MyShapeStyleBase*)));
    connect(this, SIGNAL(askForSetRemovingMenu(QList<MyShapeStyleBase*>)), _elementFeatureMenu, SIGNAL(askForSetRemovingMenu(QList<MyShapeStyleBase*>)));
    connect((MyFeatureMenuItemFrame*)_elementFeatureMenu, &MyFeatureMenuItemFrame::isUpdated, this, [this] () { isUpdated(shapeStyles()); });
    contentLayout->addWidget(_elementFeatureMenu, contentLayout->rowCount(), 0, 1, 2);

    // shape style tree view
    _shapeStylesTreeView = new MyShapeStyleTreeView(this);
    connect(_shapeStylesTreeView, SIGNAL(extentsAreUpdated(const QSize&)), this, SLOT(setExpandableWidgetSize(const QSize&)));
    contentLayout->addWidget(_shapeStylesTreeView, contentLayout->rowCount(), 0, 1, 2);

    setLayout(contentLayout);
    updateExtents();
}

MyFeatureMenuBase::FEATURE_MENU_TYPE MySingleNetworkElementFeatureMenu::type() {
    return ELEMENT_FEATURE_MENU;
}

QList<MyShapeStyleBase*> MySingleNetworkElementFeatureMenu::shapeStyles() {
    for (MyShapeStyleBase* shapeStyle : qAsConst(_shapeStyles)) {
        for (MyParameterBase* parameter : shapeStyle->parameters() + shapeStyle->outsourcingParameters())
            parameter->setDefaultValue();
    }

    return _shapeStyles;
}

void MySingleNetworkElementFeatureMenu::setShapeStyles(QList<MyShapeStyleBase*> shapeStyles) {
    clearShapeStyles();
    for (MyShapeStyleBase* shapeStyle : shapeStyles)
        addShapeStyle(shapeStyle);
}

void MySingleNetworkElementFeatureMenu::addShapeStyle(MyShapeStyleBase* shapeStyle) {
    _shapeStyles.push_back(shapeStyle);
    connect(shapeStyle, &MyShapeStyleBase::isUpdated, this, [this] () { emit isUpdated(this->shapeStyles()); });
    emit askForSetRemovingMenu(_shapeStyles);
    ((MyShapeStyleTreeView*)_shapeStylesTreeView)->setBranches(_shapeStyles);
}

void MySingleNetworkElementFeatureMenu::addSingleShapeStyle(MyShapeStyleBase* shapeStyle) {
    _shapeStyles.push_front(shapeStyle);
    connect(shapeStyle, &MyShapeStyleBase::isUpdated, this, [this] () { emit isUpdated(this->shapeStyles()); });
    ((MyShapeStyleTreeView*)_shapeStylesTreeView)->setBranches(_shapeStyles);
}

void MySingleNetworkElementFeatureMenu::addNewShapeStyle(MyShapeStyleBase* shapeStyle) {
    addShapeStyle(shapeStyle);
    ((MyShapeStyleTreeView*)_shapeStylesTreeView)->expandLastBranch();
    emit isUpdated(shapeStyles());
}

void MySingleNetworkElementFeatureMenu::removeShapeStyle(MyShapeStyleBase* shapeStyle) {
    _shapeStyles.removeOne(shapeStyle);
    emit askForSetRemovingMenu(_shapeStyles);
    ((MyShapeStyleTreeView*)_shapeStylesTreeView)->setBranches(_shapeStyles);
    emit isUpdated(shapeStyles());
}

void MySingleNetworkElementFeatureMenu::changeShapeStyle(MyShapeStyleBase* shapeStyle) {
    _shapeStyles.removeFirst();
    addSingleShapeStyle(shapeStyle);
    ((MyShapeStyleTreeView*)_shapeStylesTreeView)->expandFirstBranch();
    emit isUpdated(shapeStyles());
}

MyShapeStyleBase* MySingleNetworkElementFeatureMenu::beingModifiedShapeStyle() {
    QString expandedBranchTitle = ((MyShapeStyleTreeView*)_shapeStylesTreeView)->getExpandedBranchTitle();
    for (MyShapeStyleBase* shapeStyle : shapeStyles()) {
        if (shapeStyle->name() == expandedBranchTitle)
            return shapeStyle;
    }

    return NULL;
}

void MySingleNetworkElementFeatureMenu::setBeingModifiedShapeStyle(MyShapeStyleBase* shapeStyle) {
    if (shapeStyle)
        ((MyShapeStyleTreeView*)_shapeStylesTreeView)->expandBranch(shapeStyle);
}

void MySingleNetworkElementFeatureMenu::clearShapeStyles() {
    while(shapeStyles().size())
        delete shapeStyles().takeLast();
}

void MySingleNetworkElementFeatureMenu::setExpandableWidgetSize(const QSize& expandableWidgetSize) {
    _expandableWidgetSize = expandableWidgetSize;
    updateExtents();
}

void MySingleNetworkElementFeatureMenu::updateExtents() {
    qint32 menuWidth = 0;
    QSize elementFeatureMenuSize = ((MyFeatureMenuItemFrame*)_elementFeatureMenu)->extents();
    if (elementFeatureMenuSize.width() > menuWidth)
        menuWidth = elementFeatureMenuSize.width();

    if (_expandableWidgetSize.width() > menuWidth)
        menuWidth = _expandableWidgetSize.width();
    setFixedWidth(qMax(menuWidth, 300));
}