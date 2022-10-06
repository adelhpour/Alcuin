#include "negui_base.h"

// MyBase

MyBase::MyBase(const QString& name) {
    _name = name;
}

const QString& MyBase::name() const {
    return _name;
}

// MyPluginItemBase

MyPluginItemBase::MyPluginItemBase(const QString& name) : MyBase(name) {
    _iconSize = QSize(0, 0);
}

const QSize& MyPluginItemBase::iconSize() const {
    return _iconSize;
}

// MySpacerItem

MySpacerItem::MySpacerItem(int w, int h) : QSpacerItem(w, h, QSizePolicy::Fixed, QSizePolicy::Fixed) {
    
}

// MyLabel

MyLabel::MyLabel(const QString& text, QWidget* parent) : QLabel(parent) {
    setContentsMargins(0, 0, 0, 0);
    QFont _font;
    _font.setBold(true);
    _font.setCapitalization(QFont::Capitalize);
    _font.setPointSize(12);
    setFont(_font);
    
    if (!text.isEmpty())
        setText(text);
    
    setFixedSize(120, 20);
}

// MyTitleLabel

MyTitleLabel::MyTitleLabel(const QString& text, QWidget* parent) : MyLabel(text, parent) {
    QFont _font = font();
    _font.setPointSize(18);
    setFont(_font);
    setFixedSize(120, 25);
    setAlignment(Qt::AlignCenter);
}

// MyLineEdit

MyLineEdit::MyLineEdit(const QString &contents, QWidget* parent) : QLineEdit(parent) {
    setContentsMargins(0, 0, 0, 0);
    setStyleSheet("QLineEdit {border: 1px solid LightSlateGray; border-radius: 10px; padding: 0 8px; background: GhostWhite; selection-background-color: LightGray;} QLineEdit::read-only {color: gray}");
    setText(contents);
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    setFixedSize(120, 20);
}

void MyLineEdit::setText(const QString &contents) {
    QLineEdit::setText(contents);
    setToolTip(contents);
    setCursorPosition(0);
}



MyReadOnlyLineEdit::MyReadOnlyLineEdit(const QString &contents, QWidget* parent) : MyLineEdit(contents, parent) {
    setReadOnly(true);
}

// MySpinBox

MySpinBox::MySpinBox(QWidget* parent) : QSpinBox(parent) {
    setContentsMargins(0, 0, 0, 0);
    setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
    setStyleSheet("QSpinBox { border: 1px solid LightSlateGray; border-radius: 5px; background-color: GhostWhite; margin-right: 15px }" "QSpinBox::up-button { background-color: transparent; }" "QSpinBox::down-button { background-color: transparent;}" "QSpinBox::up-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-bottom: 5px solid black; width: 0px; height: 0px; }" "QSpinBox::down-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-top: 5px solid black; width: 0px; height: 0px; }");
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    setFixedSize(120, 20);
}

// MyDoubleSpinBox

MyDoubleSpinBox::MyDoubleSpinBox(QWidget* parent) : QDoubleSpinBox(parent) {
    setContentsMargins(0, 0, 0, 0);
    setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
    setStyleSheet("QDoubleSpinBox { border: 1px solid LightSlateGray; border-radius: 5px; background-color: GhostWhite; margin-right: 15px }" "QDoubleSpinBox::up-button { background-color: transparent; }" "QDoubleSpinBox::down-button { background-color: transparent;}" "QDoubleSpinBox::up-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-bottom: 5px solid black; width: 0px; height: 0px; }" "QDoubleSpinBox::down-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-top: 5px solid black; width: 0px; height: 0px; }");
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    setFixedSize(120, 20);
}

// MyComboBox

MyComboBox::MyComboBox(QWidget* parent) : QComboBox(parent) {
    setContentsMargins(0, 0, 0, 0);
    setStyleSheet("QComboBox { border: 1px solid LightSlateGray; border-radius: 5px; background-color: GhostWhite; color: black; padding: 1px 18px 1px 3px; }" "QComboBox::drop-down { subcontrol-origin: padding; subcontrol-position: top right; width: 15px; border-left-width: 1px; border-left-color: LightSlateGray;border-left-style: solid; border-top-right-radius: 5px; border-bottom-right-radius: 5px; }" "QComboBox::down-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-top: 5px solid black; width: 0px; height: 0px; } QComboBox::disabled { color: gray;} QComboBox QAbstractItemView { border: 1px solid LightSlateGray; background-color: white; min-width: 100px; }");
    
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    setFixedSize(120, 20);
}

// MyGroupBox

MyGroupBox::MyGroupBox(QWidget* parent) : QGroupBox(parent) {
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    setStyleSheet("QGroupBox { background-color: white; border: no-border;}");
    setContentsMargins(0, 0, 0, 0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
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

// MyDialog

MyDialog::MyDialog(QWidget *parent) : QDialog(parent) {
    setStyleSheet("QDialog {background-color: white;}");
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

// MyTreeView

MyTreeView::MyTreeView(QWidget* parent) : QTreeView(parent) {
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

void MyTreeView::addBranchWidget(QWidget* branchWidget, const QString& branchTitle, const QString& rootTitle) {
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

void MyTreeView::clearModel() {
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

void MyTreeView::removeBranches(const QString& rootTitle, const unsigned int& staticbranches) {
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

const QSize MyTreeView::collapsedSize() const {
    return QSize(100, 100 + 20 * model()->rowCount());
}

// MyProxyStyle

MyProxyStyle::MyProxyStyle(QStyle *style) : QProxyStyle(style) {
    
}

int MyProxyStyle::styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* returnData) const {
    if (hint == QStyle::SH_ToolTip_WakeUpDelay)
        return 250;
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}

// MyToolButton

MyToolButton::MyToolButton(QWidget* parent) : QToolButton(parent) {
    setPopupMode(QToolButton::InstantPopup);
    setStyleSheet("QToolButton:pressed {background-color : darkgray; border-radius : 5px} QToolButton::menu-indicator {width : 0}");
}

// MyToolButtonMenu

MyToolButtonMenu::MyToolButtonMenu(QWidget* parent) : QMenu(parent) {
    setStyleSheet("QMenu { background-color: white; border-radius: 10px;} ");
}

// MyWidgetAction

MyWidgetAction::MyWidgetAction(QObject* parent) : QWidgetAction(parent) {
    
}

void MyWidgetAction::setItems(QList<MyPluginItemBase*> items) {
    setDefaultWidget(createItemPreviewWidget(items));
}

QWidget* MyWidgetAction::createItemPreviewWidget(QList<MyPluginItemBase*> items) {
    QWidget* itemWidget = new QWidget();
    QVBoxLayout* itemWidgetLayoutContent = new QVBoxLayout();
    QPushButton* itemPreviewButton = NULL;
    for (MyPluginItemBase* item : items) {
        itemPreviewButton = new MyItemPreviewButton(item);
        connect(itemPreviewButton, &QPushButton::clicked, this, [this, item] () { emit itemIsChosen(item); ((MyToolButtonMenu*)(this->parent()))->close(); });
        itemWidgetLayoutContent->addWidget(itemPreviewButton, itemWidgetLayoutContent->count());
    }
    itemWidget->setLayout(itemWidgetLayoutContent);
    itemWidget->setStyleSheet("QWidget { background-color: white; border-radius: 10px;}");
    
    return itemWidget;
}

// MyItemPreviewButton

MyItemPreviewButton::MyItemPreviewButton(MyPluginItemBase* item, QWidget *parent) : QPushButton(parent) {
    setCheckable(true);
    setToolTip(item->name());
    
    if (item->icon().isNull()) {
        setStyleSheet("QPushButton { border : no-border; text-align : left} QPushButton:hover { color: darkgray}");
        setText(item->name());
    }
    else {
        setStyleSheet("QPushButton { border : no-border;} QPushButton:hover { background-color: darkgray}");
        setIcon(item->icon());
        setIconSize(item->iconSize());
    }
}

// MyUndoStack

MyUndoStack::MyUndoStack(QObject *parent) : QUndoStack(parent) {
    
}

void MyUndoStack::addCommand(QUndoCommand* command) {
    push(command);
}

void MyUndoStack::clear() {
    QUndoStack::clear();
}

