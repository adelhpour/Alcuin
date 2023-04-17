#include "negui_feature_menu.h"
#include "negui_parameters.h"

// MyFeatureMenu

MyFeatureMenu::MyFeatureMenu(QWidget* elementFeatureMenu, QWidget *parent) : MyGroupBox(parent) {
    _expandableWidgetSize = QSize(0, 0);
    setWindowTitle("Features");
    setStyleSheet("QDialog {background-color: white;}");
    QGridLayout* contentLayout = new QGridLayout(this);

    // element feature menu
    _elementFeatureMenu = elementFeatureMenu;
    connect(_elementFeatureMenu, SIGNAL(askForAddShapeStyle(MyShapeStyleBase*)), this, SLOT(addShapeStyle(MyShapeStyleBase*)));
    connect(_elementFeatureMenu, SIGNAL(askForRemoveShapeStyle(MyShapeStyleBase*)), this, SLOT(removeShapeStyle(MyShapeStyleBase*)));
    connect(this, SIGNAL(askForSetRemovingMenu(QList<MyShapeStyleBase*>)), _elementFeatureMenu, SIGNAL(askForSetRemovingMenu(QList<MyShapeStyleBase*>)));
    contentLayout->addWidget(elementFeatureMenu, contentLayout->rowCount(), 0, 1, 2);

    // shape style tree view
    _shapeStylesTreeView = new MyShapeStyleTreeView(this);
    connect(_shapeStylesTreeView, SIGNAL(extentsAreUpdated(const QSize&)), this, SLOT(setExpandableWidgetSize(const QSize&)));
    contentLayout->addWidget(_shapeStylesTreeView, contentLayout->rowCount(), 0, 1, 2);

    setLayout(contentLayout);
    updateDialogBoxExtents();
}

void MyFeatureMenu::setShapeStyles(QList<MyShapeStyleBase*> shapeStyles) {
    _shapeStyles = shapeStyles;
    setShapeStyles();
}

void MyFeatureMenu::setShapeStyles() {
    QList<MyShapeStyleBase*> temporaryShapeStyles = getTemporaryShapeStyles();
    emit askForSetRemovingMenu(temporaryShapeStyles);
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
    
    if (_expandableWidgetSize.width() > menuWidth)
        menuWidth = _expandableWidgetSize.width();
    menuHeight += _expandableWidgetSize.height();
    
    setFixedSize(qMax(menuWidth, 300), qMax(menuHeight, 350));
}

// MyMenuItemGroupBox

MyMenuItemGroupBox::MyMenuItemGroupBox(QWidget* parent) : MyGroupBox(parent) {
    setLayout(new QGridLayout());
}

const QSize MyMenuItemGroupBox::extents() const {
    qint32 totalWidth = 0;
    qint32 totalHeight = 0;
    qint32 rowWidth = 0;
    qint32 rowHeight = 0;
    QGridLayout* contentLayout = (QGridLayout*)layout();
    QLayoutItem* item = NULL;
    for (qint32 row = 0; row < contentLayout->rowCount(); ++row) {
        rowWidth = 0;
        rowHeight = 0;
        for (qint32 column = 0; column < contentLayout->columnCount(); ++column) {
            item = contentLayout->itemAtPosition(row, column);
            if (item && item->widget()) {
                rowWidth += item->widget()->size().width();
                if (item->widget()->size().height() > rowHeight)
                    rowHeight = item->widget()->size().height();
            }
        }
        
        if (rowWidth > totalWidth)
            totalWidth = rowWidth;
        totalHeight+= rowHeight;
    }

    return QSize(totalWidth + 10 * contentLayout->columnCount(), totalHeight + 10 *  contentLayout->rowCount() );
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


