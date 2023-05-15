#include "negui_feature_menu.h"

// MyFeatureMenu

MyFeatureMenu::MyFeatureMenu(QWidget* elementFeatureMenu, QWidget *parent) : MyFrame(parent) {
    _expandableWidgetSize = QSize(0, 0);
    QGridLayout* contentLayout = new QGridLayout(this);

    // element feature menu
    _elementFeatureMenu = elementFeatureMenu;
    connect(_elementFeatureMenu, SIGNAL(askForAddShapeStyle(MyShapeStyleBase*)), this, SLOT(addNewShapeStyle(MyShapeStyleBase*)));
    connect(_elementFeatureMenu, SIGNAL(askForRemoveShapeStyle(MyShapeStyleBase*)), this, SLOT(removeShapeStyle(MyShapeStyleBase*)));
    connect(this, SIGNAL(askForSetRemovingMenu(QList<MyShapeStyleBase*>)), _elementFeatureMenu, SIGNAL(askForSetRemovingMenu(QList<MyShapeStyleBase*>)));
    contentLayout->addWidget(_elementFeatureMenu, contentLayout->rowCount(), 0, 1, 2);

    // shape style tree view
    _shapeStylesTreeView = new MyShapeStyleTreeView(this);
    connect(_shapeStylesTreeView, SIGNAL(extentsAreUpdated(const QSize&)), this, SLOT(setExpandableWidgetSize(const QSize&)));
    contentLayout->addWidget(_shapeStylesTreeView, contentLayout->rowCount(), 0, 1, 2);

    setLayout(contentLayout);
    updateExtents();
}

QList<MyShapeStyleBase*> MyFeatureMenu::shapeStyles() {
    for (MyShapeStyleBase* shapeStyle : qAsConst(_shapeStyles)) {
        for (MyParameterBase* parameter : qAsConst(shapeStyle->parameters()))
            parameter->setDefaultValue();
    }

    return _shapeStyles;
}

void MyFeatureMenu::setShapeStyles(QList<MyShapeStyleBase*> shapeStyles) {
    clearShapeStyles();
    for (MyShapeStyleBase* shapeStyle : shapeStyles)
        addShapeStyle(shapeStyle);
}

void MyFeatureMenu::addShapeStyle(MyShapeStyleBase* shapeStyle) {
    _shapeStyles.push_back(shapeStyle);
    connect(shapeStyle, &MyShapeStyleBase::isUpdated, this, [this] () { emit isUpdated(this->shapeStyles()); });
    emit askForSetRemovingMenu(_shapeStyles);
    ((MyShapeStyleTreeView*)_shapeStylesTreeView)->setBranches(_shapeStyles);
}

void MyFeatureMenu::addNewShapeStyle(MyShapeStyleBase* shapeStyle) {
    addShapeStyle(shapeStyle);
    ((MyShapeStyleTreeView*)_shapeStylesTreeView)->expandLastBranch();
    emit isUpdated(shapeStyles());
}

void MyFeatureMenu::removeShapeStyle(MyShapeStyleBase* shapeStyle) {
    _shapeStyles.removeOne(shapeStyle);
    emit askForSetRemovingMenu(_shapeStyles);
    ((MyShapeStyleTreeView*)_shapeStylesTreeView)->setBranches(_shapeStyles);
    emit isUpdated(shapeStyles());
}

void MyFeatureMenu::clearShapeStyles() {
    while(shapeStyles().size())
        delete shapeStyles().takeLast();
}

void MyFeatureMenu::setExpandableWidgetSize(const QSize& expandableWidgetSize) {
    _expandableWidgetSize = expandableWidgetSize;
    updateExtents();
}

void MyFeatureMenu::updateExtents() {
    qint32 menuWidth = 0;
    qint32 menuHeight = 0;
    
    QSize elementFeatureMenuSize = ((MyFeatureMenuItemFrame*)_elementFeatureMenu)->extents();
    if (elementFeatureMenuSize.width() > menuWidth)
        menuWidth = elementFeatureMenuSize.width();
    menuHeight += elementFeatureMenuSize.height();
    
    if (_expandableWidgetSize.width() > menuWidth)
        menuWidth = _expandableWidgetSize.width();
    menuHeight += _expandableWidgetSize.height();
    
    setFixedSize(qMax(menuWidth, 300), qMax(menuHeight, 350));
}


