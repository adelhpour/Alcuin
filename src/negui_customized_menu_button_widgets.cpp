#include "negui_customized_menu_button_widgets.h"

#include <QVBoxLayout>

// MyWidgetAction

MyWidgetAction::MyWidgetAction(QObject* parent) : QWidgetAction(parent) {

}

void MyWidgetAction::setItems(QList<MyPluginItemBase*> items) {
    setDefaultWidget(createItemPreviewWidget(items));
}

QWidget* MyWidgetAction::createItemPreviewWidget(QList<MyPluginItemBase*> items) {
    QWidget* itemWidget = new QWidget();
    QVBoxLayout* itemWidgetLayoutContent = new QVBoxLayout();
    QToolButton* itemPreviewButton = NULL;

    QList<QString> itemsSubCategories = getPluginsSubCategories(items);
    if (itemsSubCategories.size()) {
        for (QString subCategory : itemsSubCategories) {
            itemWidgetLayoutContent->addWidget(new MyLabel(subCategory), itemWidgetLayoutContent->count());
            QList<MyPluginItemBase*> itemsOfSubCategory = getPluginsOfSubCategory(items, subCategory);
            for (MyPluginItemBase* itemOfSubCategory : itemsOfSubCategory)
                itemWidgetLayoutContent->addWidget(createItemPreviewButton(itemOfSubCategory), itemWidgetLayoutContent->count());
        }
    }
    else {
        for (MyPluginItemBase* item : items)
            itemWidgetLayoutContent->addWidget(createItemPreviewButton(item), itemWidgetLayoutContent->count());

    }

    itemWidget->setLayout(itemWidgetLayoutContent);
    itemWidget->setStyleSheet("QWidget { background-color: white; border-radius: 10px;}");

    return itemWidget;
}

QToolButton* MyWidgetAction::createItemPreviewButton(MyPluginItemBase* item) {
    QToolButton* itemPreviewButton = new MyItemPreviewButton(item);
    connect(itemPreviewButton, &QToolButton::clicked, this, [this, item, itemPreviewButton] () {
        ((MyItemPreviewButton*)itemPreviewButton)->hoverOut();
        emit itemIsChosen(item);
    });

    return itemPreviewButton;
}

// MyItemPreviewButton

MyItemPreviewButton::MyItemPreviewButton(MyPluginItemBase* item, QWidget *parent) : MyModeMenuToolButton(parent) {
    setCheckable(true);
    setToolTip(item->name());
    if (item->icon().isNull())
        setText(item->name());
    else {
        setIcon(item->icon());
        setIconSize(item->iconSize());
    }

    connect(this, &QToolButton::clicked, this, [this, item] () { itemIsChosen(item); });
    setStyleToInactiveForm();
}

void MyItemPreviewButton::hoverOut() {
    //setStyleSheet("QToolButton { background-color: transparent}");
}