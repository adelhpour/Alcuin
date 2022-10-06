#include "negui_feature_menu.h"

// MyFeatureMenu

MyFeatureMenu::MyFeatureMenu(QWidget* elementFeatureMenu, QWidget *parent) : MyDialog(parent) {
    _expandableWidgetSize = QSize(0, 0);
    setWindowTitle("Features");
    QGridLayout* contentLayout = new QGridLayout(this);
    
    // element feature menu
    _elementFeatureMenu = elementFeatureMenu;
    contentLayout->addWidget(elementFeatureMenu, contentLayout->rowCount(), 0, 1, 2);
    
    // add/remove shape style buttons
    _addRemoveShapeStyleButtons = new MyAddRemoveShapeStylesButtons();
    connect(_addRemoveShapeStyleButtons, SIGNAL(askForAddShapeStyle(MyShapeStyleBase*)), this, SLOT(addShapeStyle(MyShapeStyleBase*)));
    connect(_addRemoveShapeStyleButtons, SIGNAL(askForRemoveShapeStyle(MyShapeStyleBase*)), this, SLOT(removeShapeStyle(MyShapeStyleBase*)));
    contentLayout->addWidget(_addRemoveShapeStyleButtons, contentLayout->rowCount(), 1, Qt::AlignRight);
    
    // shape style tree view
    _shapeStylesTreeView = new MyShapeStyleTreeView(this);
    connect(_shapeStylesTreeView, SIGNAL(extentsAreUpdated(const QSize&)), this, SLOT(setExpandableWidgetSize(const QSize&)));
    contentLayout->addWidget(_shapeStylesTreeView, contentLayout->rowCount(), 0, 1, 2);
    
    // apply/cancel buttons
    QDialogButtonBox* dialogBoxButtons = new QDialogButtonBox(Qt::Horizontal, this);
    dialogBoxButtons->addButton(QDialogButtonBox::Cancel);
    dialogBoxButtons->addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    QObject::connect(dialogBoxButtons, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(dialogBoxButtons, SIGNAL(rejected()), this, SLOT(reject()));
    contentLayout->addWidget(dialogBoxButtons, contentLayout->rowCount(), 0, 1, 2);
    
    setLayout(contentLayout);
    updateExtents();
}

void MyFeatureMenu::setShapeStyles(QList<MyShapeStyleBase*> shapeStyles) {
    _shapeStyles = shapeStyles;
    setShapeStyles();
}

void MyFeatureMenu::setShapeStyles() {
    QList<MyShapeStyleBase*> temporaryShapeStyles = getTemporaryShapeStyles();
    _addRemoveShapeStyleButtons->setRemovingMenu(temporaryShapeStyles);
    _shapeStylesTreeView->setBranches(temporaryShapeStyles);
}

void MyFeatureMenu::addShapeStyle(MyShapeStyleBase* shapeStyle) {
    _temporaryAddedShapeStyles.push_back(shapeStyle);
    setShapeStyles();
    _shapeStylesTreeView->exapndLastBranch();
}

void MyFeatureMenu::removeShapeStyle(MyShapeStyleBase* shapeStyle) {
    _temporaryRemovedShapeStyles.push_back(shapeStyle);
    setShapeStyles();
}

QList<MyShapeStyleBase*> MyFeatureMenu::getShapeStyles() {
    QList<MyShapeStyleBase*> temporaryShapeStyles = getTemporaryShapeStyles();
    for (MyShapeStyleBase* temporaryShapeStyle : qAsConst(temporaryShapeStyles)) {
        for (MyParameterBase* parameter : qAsConst(temporaryShapeStyle->parameters()))
             parameter->setDefaultValue();
    }
        
    return temporaryShapeStyles;
}

QList<MyShapeStyleBase*> MyFeatureMenu::getTemporaryShapeStyles() {
    QList<MyShapeStyleBase*> temporaryShapeStyles;
    bool shapeStyleIsRemoved = false;
    
    for (MyShapeStyleBase* shapeStyle : qAsConst(_shapeStyles)) {
        shapeStyleIsRemoved = false;
        for (MyShapeStyleBase* temporaryremovedShapeStyle : qAsConst(_temporaryRemovedShapeStyles)) {
            if (shapeStyle == temporaryremovedShapeStyle)
                shapeStyleIsRemoved = true;
        }
        
        if (!shapeStyleIsRemoved)
            temporaryShapeStyles.push_back(shapeStyle);
    }
    
    for (MyShapeStyleBase* temporaryAddedShapeStyle : qAsConst(_temporaryAddedShapeStyles)) {
        shapeStyleIsRemoved = false;
        for (MyShapeStyleBase* temporaryremovedShapeStyle : qAsConst(_temporaryRemovedShapeStyles)) {
            if (temporaryAddedShapeStyle == temporaryremovedShapeStyle)
                shapeStyleIsRemoved = true;
        }
        
        if (!shapeStyleIsRemoved)
            temporaryShapeStyles.push_back(temporaryAddedShapeStyle);
    }
    
    return temporaryShapeStyles;
}

void MyFeatureMenu::setExpandableWidgetSize(const QSize& expandableWidgetSize) {
    _expandableWidgetSize = expandableWidgetSize;
    updateExtents();
}

void MyFeatureMenu::updateExtents() {
    qint32 menuWidth = 0;
    qint32 menuHeight = 0;
    
    QSize elementFeatureMenuSize = ((MyMenuItemGroupBox*)_elementFeatureMenu)->extents();
    if (elementFeatureMenuSize.width() > menuWidth)
        menuWidth = elementFeatureMenuSize.width();
    menuHeight += elementFeatureMenuSize.height();
    
    // add/remove buttons
    menuHeight += 50.0;
    
    if (_expandableWidgetSize.width() > menuWidth)
        menuWidth = _expandableWidgetSize.width();
    menuHeight += _expandableWidgetSize.height();
    
    setFixedSize(qMax(menuWidth, 300), qMax(menuHeight, 350));
}

// MyAddRemoveShapeStylesButtons

MyAddRemoveShapeStylesButtons::MyAddRemoveShapeStylesButtons(QWidget* parent) : QDialogButtonBox(parent) {
    setContentsMargins(0, 0, 0, 0);
    setOrientation(Qt::Horizontal);
    //setFixedHeight(100);
    
    // add button
    _addPushButton = addButton(QString("+"), QDialogButtonBox::YesRole);
    _addingMenu = new QMenu(_addPushButton);
    _addPushButton->setMenu(_addingMenu);
    
    // adding menu
    connect(_addingMenu->addAction("ellipse"), &QAction::triggered, this, [this] () {
        emit askForAddShapeStyle(new MyNodeEllipseStyle("ellipse")); });
    connect(_addingMenu->addAction("rect"), &QAction::triggered, this, [this] () { emit askForAddShapeStyle(new MyNodeRectStyle("rect")); });
    connect(_addingMenu->addAction("polygon"), &QAction::triggered, this, [this] () { MyNodePolygonStyle* polygonShapeStyle = new MyNodePolygonStyle("polygon");
        polygonShapeStyle->addDefaultPoints();
        emit askForAddShapeStyle(polygonShapeStyle); });
    connect(_addingMenu->addAction("text"), &QAction::triggered, this, [this] () { emit askForAddShapeStyle(new MyTextStyle("text")); });
    
    // remove button
    _removePushButton = addButton(QString("-"), QDialogButtonBox::NoRole);
    _removingMenu = new QMenu(_removePushButton);
    _removePushButton->setMenu(_removingMenu);
}

void MyAddRemoveShapeStylesButtons::setRemovingMenu(QList<MyShapeStyleBase*> shapeStyles) {
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

// MyShapeStyleTreeView

MyShapeStyleTreeView::MyShapeStyleTreeView(QWidget* parent) : MyTreeView(parent) {
    
}

void MyShapeStyleTreeView::setBranches(QList<MyShapeStyleBase*> shapeStyles) {
    clearModel();
    for (MyShapeStyleBase* shapeStyle : shapeStyles) {
        MyMenuItemGroupBox* shapeStyleBranch = new MyMenuItemGroupBox(this);
        QGridLayout* shapeStylesContentLayout = (QGridLayout*)(shapeStyleBranch->layout());
        shapeStyle->populateFeaturesMenu(shapeStylesContentLayout);
        addBranchWidget(shapeStyleBranch, shapeStyle->name());
    }
}

void MyShapeStyleTreeView::exapndLastBranch() {
    expand(treeModel->indexFromItem((treeModel->item(treeModel->rowCount() - 1))));
}


