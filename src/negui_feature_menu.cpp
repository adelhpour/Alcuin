#include "negui_feature_menu.h"
#include "negui_parameters.h"
#include "negui_shape_style_builder.h"
#include "negui_polygon_style.h"

// MyFeatureMenu

MyFeatureMenu::MyFeatureMenu(QWidget* elementFeatureMenu, QWidget *parent) : QDialog(parent) {
    _expandableWidgetSize = QSize(0, 0);
    setWindowTitle("Features");
    setStyleSheet("QDialog {background-color: white;}");
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
    updateDialogBoxExtents();
}

void MyFeatureMenu::setShapeStyles(QList<MyShapeStyleBase*> shapeStyles) {
    _shapeStyles = shapeStyles;
    setShapeStyles();
}

void MyFeatureMenu::setShapeStyles() {
    QList<MyShapeStyleBase*> temporaryShapeStyles = getTemporaryShapeStyles();
    ((MyAddRemoveShapeStylesButtons*)_addRemoveShapeStyleButtons)->setRemovingMenu(temporaryShapeStyles);
    ((MyShapeStyleTreeView*)_shapeStylesTreeView)->setBranches(temporaryShapeStyles);
}

void MyFeatureMenu::addShapeStyle(MyShapeStyleBase* shapeStyle) {
    _temporaryAddedShapeStyles.push_back(shapeStyle);
    setShapeStyles();
    ((MyShapeStyleTreeView*)_shapeStylesTreeView)->exapndLastBranch();
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
    updateDialogBoxExtents();
}

void MyFeatureMenu::updateDialogBoxExtents() {
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
        emit askForAddShapeStyle(createNodeEllipseStyle("ellipse")); });
    connect(_addingMenu->addAction("rect"), &QAction::triggered, this, [this] () { emit askForAddShapeStyle(createNodeRectStyle("rect")); });
    connect(_addingMenu->addAction("polygon"), &QAction::triggered, this, [this] () { MyShapeStyleBase* polygonShapeStyle = createNodePolygonStyle("polygon");
        ((MyNodePolygonStyle*)polygonShapeStyle)->addDefaultPoints();
        emit askForAddShapeStyle(polygonShapeStyle); });
    connect(_addingMenu->addAction("text"), &QAction::triggered, this, [this] () { emit askForAddShapeStyle(createTextStyle("text")); });
    
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

// MyStandardItem

MyStandardItem::MyStandardItem(const QString& text, const qreal& fontsize, const bool& isbold, const QColor& color) {
    QFont font;
    font.setPointSize(fontsize);
    font.setCapitalization(QFont::Capitalize);
    font.setBold(isbold);
    
    setEditable(false);
    setSelectable(false);
    setForeground(color);
    setFont(font);
    setText(text);
}

// MyShapeStyleTreeView

MyShapeStyleTreeView::MyShapeStyleTreeView(QWidget* parent) : QTreeView(parent) {
    setHeaderHidden(true);
    setStyleSheet("QTreeView { background-color: white; border: no-border;}" "QTreeView::item:selected { background-color: white; border: no-border;}" "QTreeView::item:hover { background-color: white; border: no-border;}");
    setContentsMargins(0, 0, 0, 0);
    //setAnimated(true);
    
    treeModel = new QStandardItemModel(this);
    setModel(treeModel);
    
    connect(this, QOverload<const QModelIndex&>::of(&QTreeView::expanded), this, [this] (const QModelIndex& index) {
        std::vector<QStandardItem*> familyItems;
        std::list<QStandardItem*> items;
        QStandardItem* root = treeModel->invisibleRootItem();;
        QStandardItem* item = treeModel->itemFromIndex(index);
        int n = 0;
        int m = 0;
        
        familyItems.push_back(item);
        while (item->parent()) {
            item = item->parent();
            familyItems.push_back(item);
        }
        
        for (int i = 0; i < root->rowCount(); ++i) {
            items.push_back(root->child(i));
            
            while (!items.empty()) {
                n = items.size();
                
                while (n > 0) {
                    item = items.front();
                    items.pop_front();
                    
                    m = 0;
                    for (m = 0; m < familyItems.size(); ++m) {
                        if (item->text() == familyItems.at(m)->text())
                            break;
                    }
                    
                    if (m == familyItems.size())
                        this->collapse(treeModel->indexFromItem(item));
                    
                    //if (!item->hasChildren())
                        //break;
                    
                    for (int j = 0; j < item->rowCount(); ++j)
                        items.push_back(item->child(j));
                    
                    n--;
                }
            }
        }
        
        QWidget* branchWidget = indexWidget(treeModel->itemFromIndex(index)->child(0)->index());
        if (branchWidget)
            emit extentsAreUpdated(((MyMenuItemGroupBox*)branchWidget)->extents() + collapsedSize());
        this->scrollTo(index, QAbstractItemView::PositionAtTop);
    });
    
    connect(this, &QTreeView::collapsed, this, [this] () {
        emit extentsAreUpdated(collapsedSize());
    });
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

void MyShapeStyleTreeView::addBranchWidget(QWidget* branchWidget, const QString& branchTitle, const QString& rootTitle) {
    MyStandardItem* branch = new MyStandardItem(branchTitle, 12.0, true);
    if (treeModel->findItems(rootTitle).empty())
        treeModel->invisibleRootItem()->appendRow(branch);
    else
        treeModel->findItems(rootTitle).first()->appendRow(branch);
    
    if (branchWidget) {
        MyStandardItem* branchContent = new MyStandardItem();
        branch->appendRow(branchContent);
        setIndexWidget(branchContent->index(), branchWidget);
        _branches.push_back(std::pair<MyStandardItem*, MyStandardItem*>(branch, branchContent));
    }
    else
        _branches.push_back(std::pair<MyStandardItem*, MyStandardItem*>(branch, NULL));
}

void MyShapeStyleTreeView::clearModel() {
    for (constBranchIt bIt = BranchesBegin(); bIt != BranchesEnd(); ++bIt) {
        if ((*bIt).second) {
            (*bIt).second->removeRows(0, (*bIt).second->rowCount());
            delete (*bIt).second;
        }
        delete (*bIt).first;
    }
    _branches.clear();
    treeModel->clear();
}

void MyShapeStyleTreeView::removeBranches(const QString& rootTitle, const unsigned int& staticbranches) {
    QList<QStandardItem *> roots;
    if (!rootTitle.isEmpty())
        roots = treeModel->findItems(rootTitle);
    else
        roots.push_back(treeModel->invisibleRootItem());
    for (int i = 0; i < roots.size(); ++i) {
        while (roots.at(i)->rowCount() > staticbranches) {
            for (constBranchIt bIt = BranchesBegin(); bIt != BranchesEnd(); ++bIt) {
                if ((*bIt).first->text() == roots.at(i)->child(staticbranches)->text()) {
                    if ((*bIt).second) {
                        (*bIt).second->clearData();
                        (*bIt).second->removeRows(0, (*bIt).second->rowCount());
                        delete (*bIt).second;
                    }
                    (*bIt).first->clearData();
                    (*bIt).first->removeRows(0, (*bIt).first->rowCount());
                    delete (*bIt).first;
                    _branches.erase(bIt);
                    roots.at(i)->removeRow(staticbranches);
                    break;
                }
            }
        }
    }
}

void MyShapeStyleTreeView::exapndLastBranch() {
    expand(treeModel->indexFromItem((treeModel->item(treeModel->rowCount() - 1))));
}

const QSize MyShapeStyleTreeView::collapsedSize() const {
    return QSize(100, 100 + 20 * model()->rowCount());
}


