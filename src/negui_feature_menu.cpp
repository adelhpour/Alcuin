#include "negui_feature_menu.h"
#include "negui_decorate_menu_buttons.h"

#include <QGridLayout>

// MyFeatureMenu

MyFeatureMenu::MyFeatureMenu(QWidget* elementFeatureMenu, const QString& iconsDirectoryPath, QWidget *parent) : MyFrame(parent) {
    _expandableWidgetSize = QSize(0, 0);
    QGridLayout* contentLayout = new QGridLayout(this);

    QPushButton* closeFeatureMenuButton = new QPushButton();
    decorateCloseFeatureMenuButton(closeFeatureMenuButton, iconsDirectoryPath);
    connect(closeFeatureMenuButton, SIGNAL(clicked()), this, SIGNAL(askForRemoveFeatureMenu()));
    contentLayout->addWidget(closeFeatureMenuButton, contentLayout->rowCount(), 0, 1, 2, Qt::AlignLeft);

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

QList<MyShapeStyleBase*> MyFeatureMenu::shapeStyles() {
    for (MyShapeStyleBase* shapeStyle : qAsConst(_shapeStyles)) {
        for (MyParameterBase* parameter : shapeStyle->parameters() + shapeStyle->outsourcingParameters())
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

void MyFeatureMenu::addSingleShapeStyle(MyShapeStyleBase* shapeStyle) {
    _shapeStyles.push_front(shapeStyle);
    connect(shapeStyle, &MyShapeStyleBase::isUpdated, this, [this] () { emit isUpdated(this->shapeStyles()); });
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

void MyFeatureMenu::changeShapeStyle(MyShapeStyleBase* shapeStyle) {
    _shapeStyles.removeFirst();
    addSingleShapeStyle(shapeStyle);
    ((MyShapeStyleTreeView*)_shapeStylesTreeView)->expandFirstBranch();
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
#if defined(Q_OS_WIN)
    setFixedSize(qMax(menuWidth, 600), qMax(menuHeight, 1200));
#else
    setFixedSize(qMax(menuWidth, 300), qMax(menuHeight, 350));
#endif

}


