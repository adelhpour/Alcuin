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

// MyFeatureMenuItemFrame

MyFeatureMenuItemFrame::MyFeatureMenuItemFrame(QWidget* parent) : MyFrame(parent) {
    setLayout(new QGridLayout());
}

const QSize MyFeatureMenuItemFrame::extents() const {
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
            emit extentsAreUpdated(((MyFeatureMenuItemFrame*)branchWidget)->extents() + collapsedSize());
        this->scrollTo(index, QAbstractItemView::PositionAtTop);
    });
    
    connect(this, &QTreeView::collapsed, this, [this] () {
        emit extentsAreUpdated(collapsedSize());
    });
}

void MyShapeStyleTreeView::setBranches(QList<MyShapeStyleBase*> shapeStyles) {
    clearModel();
    for (MyShapeStyleBase* shapeStyle : shapeStyles) {
        MyFeatureMenuItemFrame* shapeStyleBranch = new MyFeatureMenuItemFrame(this);
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

void MyShapeStyleTreeView::expandLastBranch() {
    expand(treeModel->indexFromItem((treeModel->item(treeModel->rowCount() - 1))));
}

const QSize MyShapeStyleTreeView::collapsedSize() const {
    return QSize(100, 100 + 20 * model()->rowCount());
}


