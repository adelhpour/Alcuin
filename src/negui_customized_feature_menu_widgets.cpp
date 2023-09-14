#include "negui_customized_feature_menu_widgets.h"

#include <QGridLayout>
#include <QWidgetAction>
#include <QColorDialog>
#include <QFontDialog>

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

// MyRestrictedToNameConventionsLineEdit

MyRestrictedToNameConventionsLineEdit::MyRestrictedToNameConventionsLineEdit(const QString &contents, QWidget* parent) : MyLineEdit(contents, parent) {
    setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_]+"), this));
}

void MyRestrictedToNameConventionsLineEdit::setText(const QString& newText) {
    if (newText != text())
        MyLineEdit::setText(newText);
}

// MyColorPickerButton

MyColorPickerButton::MyColorPickerButton(const QString& colorName, QWidget *parent) : QToolButton(parent) {
    setToolButtonStyle(Qt::ToolButtonIconOnly);
    setBackgroundColor("white");
    connect(this, &QToolButton::clicked, this, [this, colorName] () {
        QColor color = QColorDialog::getColor(QColor(currentColor()), nullptr, "Select " + colorName, QColorDialog::DontUseNativeDialog);
        if (color.isValid())
            setCurrentColor(color.name());
    });
    setFixedSize(120, 20);
}

void MyColorPickerButton::setBackgroundColor(const QString& color)  {
    if (!color.isEmpty())
        setStyleSheet("QToolButton { border: 1px solid LightSlateGray; border-radius: 5px; background-color:" + color + " } QToolButton::menu-indicator { image: none; } " );
}

void MyColorPickerButton::setCurrentColor(const QString& color)  {
    _currentColor = color;
    setBackgroundColor(color);
    emit colorIsChanged();
}

const QString& MyColorPickerButton::currentColor() const {
    return _currentColor;
}

// MyFontPickerButton

MyFontPickerButton::MyFontPickerButton(QWidget *parent) : QPushButton(parent) {
    setStyleSheet("QPushButton { border: 1px solid LightSlateGray; border-radius: 6px; background-color:  gainsboro}"
                  "QPushButton:pressed { border: 1px solid LightSlateGray; border-radius: 6px; background-color: darkgray}" );
    setText("Font Features");
    connect(this, &QPushButton::clicked, this, [this] () { setCurrentFont(); } );
    setFixedSize(120, 20);
}

void MyFontPickerButton::setCurrentFont(const QFont& font) {
    _currentFont = font;
}

void MyFontPickerButton::setCurrentFont() {
    bool ok;
    QFont font = QFontDialog::getFont(&ok, _currentFont, this, "Select Font Features", QFontDialog::DontUseNativeDialog);
    if (ok) {
        _currentFont = font;
        emit fontIsChanged();
    }
}

const QFont& MyFontPickerButton::currentFont() const {
    return _currentFont;
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
    setModel(new QStandardItemModel(this));

    connect(this, QOverload<const QModelIndex&>::of(&QTreeView::expanded), this, [this] (const QModelIndex& index) {
        std::vector<QStandardItem*> familyItems;
        std::list<QStandardItem*> items;
        QStandardItem* root = ((QStandardItemModel*)model())->invisibleRootItem();;
        QStandardItem* item = ((QStandardItemModel*)model())->itemFromIndex(index);
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
                        this->collapse(((QStandardItemModel*)model())->indexFromItem(item));

                    //if (!item->hasChildren())
                    //break;

                    for (int j = 0; j < item->rowCount(); ++j)
                        items.push_back(item->child(j));

                    n--;
                }
            }
        }

        QWidget* branchWidget = indexWidget(((QStandardItemModel*)model())->itemFromIndex(index)->child(0)->index());
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
    qreal fontSize = 12.0;
#if defined(Q_OS_WIN)
    fontSize = 8.0;
#endif
    MyStandardItem* branch = new MyStandardItem(branchTitle, fontSize, true);
    if (((QStandardItemModel*)model())->findItems(rootTitle).empty())
        ((QStandardItemModel*)model())->invisibleRootItem()->appendRow(branch);
    else
        ((QStandardItemModel*)model())->findItems(rootTitle).first()->appendRow(branch);

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
    ((QStandardItemModel*)model())->clear();
}

void MyShapeStyleTreeView::removeBranches(const QString& rootTitle, const unsigned int& staticbranches) {
    QList<QStandardItem *> roots;
    if (!rootTitle.isEmpty())
        roots = ((QStandardItemModel*)model())->findItems(rootTitle);
    else
        roots.push_back(((QStandardItemModel*)model())->invisibleRootItem());
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

void MyShapeStyleTreeView::expandBranch(MyShapeStyleBase* shapeStyle) {
    for (constBranchIt bIt = BranchesBegin(); bIt != BranchesEnd(); ++bIt) {
        if (shapeStyle->name() == (*bIt).first->text()) {
            expandBranch(bIt - BranchesBegin());
            return;
        }
    }
}

void MyShapeStyleTreeView::expandBranch(const qint32& branchIndex) {
    expand(((QStandardItemModel*)model())->indexFromItem(((QStandardItemModel*)model())->item(branchIndex)));
}

void MyShapeStyleTreeView::expandFirstBranch() {
    expand(((QStandardItemModel*)model())->indexFromItem(((QStandardItemModel*)model())->item(0)));
}

void MyShapeStyleTreeView::expandLastBranch() {
    expand(((QStandardItemModel*)model())->indexFromItem(((QStandardItemModel*)model())->item(model()->rowCount() - 1)));
}

const QSize MyShapeStyleTreeView::collapsedSize() const {
    return QSize(100, 100 + 20 * model()->rowCount());
}

const QString MyShapeStyleTreeView::getExpandedBranchTitle() {
    for (unsigned int i = 0; i < model()->rowCount(); i++) {
        if (isExpanded(((QStandardItemModel*)model())->indexFromItem(((QStandardItemModel*)model())->item(i))))
            return ((QStandardItemModel*)model())->item(i)->text();
    }

    return "";
}

// MyNoSelectedElementTitle

MyNoSelectedElementTitle::MyNoSelectedElementTitle(QWidget* parent) : QLabel(parent) {
    setContentsMargins(0, 0, 0, 0);
    QFont _font;
    _font.setItalic(true);
    qreal fontSize;
#if defined(Q_OS_WIN)
    fontSize = 8;
#else
    fontSize = 12;
#endif
    _font.setPointSize(fontSize);
    setFont(_font);
    setText("There are no elements selected");
}