#include "negui_customized_menu_button_widgets.h"

#include <QVBoxLayout>

// MyModeMenuButton

MyModeMenuButton::MyModeMenuButton(QWidget* parent) : MyToolButton(parent) {

}

void MyModeMenuButton::setActive(const bool& active) {
    if (active)
        setStyleToActiveForm();
    else
        setStyleToInactiveForm();
}

void MyModeMenuButton::setStyleToActiveForm() {
    setStyleSheet("QToolButton {border: 0px; border-radius: 5px; background-color : darkgray; } QToolButton:pressed {background-color : darkgray; border-radius : 5px} QToolButton:hover { background-color: lightgray} QToolButton::menu-indicator {width : 0}");
}

void MyModeMenuButton::setStyleToInactiveForm() {
    setStyleSheet("QToolButton {border: 0px; border-radius: 5px; background-color : white; } QToolButton:pressed {background-color : white; border-radius : 5px} QToolButton:hover { background-color: lightgray} QToolButton::menu-indicator {width : 0}");
}

// MyModeMenuModeButton

MyModeMenuModeButton::MyModeMenuModeButton(const QString& mode, const QString& alternativeSimilarMode, QWidget* parent) : MyModeMenuButton(parent) {
    setSceneMode(mode);
    setAlternativeSimilarSceneMode(alternativeSimilarMode);
}

// MyExtraElementMenu

MyExtraElementMenu::MyExtraElementMenu(QWidget *parent) : QMenu(parent) {
    setStyleSheet("QMenu {background-color: white; border-radius: 0px}");
    _horizontalPadding = 35;
}

bool MyExtraElementMenu::event(QEvent *event) {
    if (event->type() == QEvent::Show)
        move(pos() + QPoint(_horizontalPadding, 0));

    return QMenu::event(event);
}

// MyExtraElementCategoryButton

MyExtraElementCategoryButton::MyExtraElementCategoryButton(QMenu* elementCategoryMenu, const QString& text, QWidget* parent) : MyToolButton(parent) {
    setText(text);
    setToolTip("Add " + text + " to the network");
    setMenu(elementCategoryMenu);
    QFont textFont = font();
    textFont.setPointSize(12);
    setFont(textFont);
    setFixedSize(100, 25);
    connect(elementCategoryMenu, SIGNAL(categoryItemIsChosen()), this, SIGNAL(categoryItemIsChosen()));
}

// MyExtraElementCategoryMenu

MyExtraElementCategoryMenu::MyExtraElementCategoryMenu(QWidget* parent) : MyMenu(parent) {
    _horizontalPadding = 80;
    connect(this, SIGNAL(categoryItemIsChosen()), this, SLOT(close()));
}

bool MyExtraElementCategoryMenu::event(QEvent *event) {
    if (event->type() == QEvent::Show)
        move(pos() + QPoint(_horizontalPadding, 0));

    return QMenu::event(event);
}

// MyMenuButtonWidgetAction

MyMenuButtonWidgetAction::MyMenuButtonWidgetAction(QObject* parent) : QWidgetAction(parent) {

}

void MyMenuButtonWidgetAction::setItems(QList<MyPluginItemBase*> items) {
    setDefaultWidget(createMenuItemPreviewWidget(items));
}

QWidget* MyMenuButtonWidgetAction::createMenuItemPreviewWidget(QList<MyPluginItemBase*> items) {
    QWidget* itemWidget = new QWidget();
    QVBoxLayout* itemWidgetLayoutContent = new QVBoxLayout();
    QToolButton* menuItemPreviewButton = NULL;

    QList<QString> itemsSubCategories = getPluginsSubCategories(items);
    if (itemsSubCategories.size()) {
        for (QString subCategory : itemsSubCategories) {
            itemWidgetLayoutContent->addWidget(new MyLabel(subCategory), itemWidgetLayoutContent->count());
            QList<MyPluginItemBase*> itemsOfSubCategory = getPluginsOfSubCategory(items, subCategory);
            for (MyPluginItemBase* itemOfSubCategory : itemsOfSubCategory)
                itemWidgetLayoutContent->addWidget(createMenuItemPreviewButton(itemOfSubCategory), itemWidgetLayoutContent->count());
        }
    }
    else {
        for (MyPluginItemBase* item : items)
            itemWidgetLayoutContent->addWidget(createMenuItemPreviewButton(item), itemWidgetLayoutContent->count());

    }

    itemWidget->setLayout(itemWidgetLayoutContent);
    itemWidget->setStyleSheet("QWidget { background-color: white; border-radius: 10px;}");

    return itemWidget;
}

QToolButton* MyMenuButtonWidgetAction::createMenuItemPreviewButton(MyPluginItemBase* item) {
    QToolButton* menuItemPreviewButton = new MyMenuItemPreviewButton(item);
    connect(menuItemPreviewButton, SIGNAL(itemIsChosen(MyPluginItemBase*)), this, SIGNAL(itemIsChosen(MyPluginItemBase*)));

    return menuItemPreviewButton;
}

// MyMenuItemPreviewButton

MyMenuItemPreviewButton::MyMenuItemPreviewButton(MyPluginItemBase* item, QWidget *parent) : MyModeMenuButton(parent) {
    setCheckable(true);
    setToolTip(item->name());
    if (item->icon().isNull())
        setText(item->name());
    else {
        setIcon(item->icon());
        setIconSize(item->iconSize());
    }
    setStyleToInactiveForm();
    connect(this, &QToolButton::clicked, this, [this, item] () { emit itemIsChosen(item); clearFocus(); });
}