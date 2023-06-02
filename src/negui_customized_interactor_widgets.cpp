#include "negui_customized_interactor_widgets.h"

// MyUndoStack

MyUndoStack::MyUndoStack(QObject *parent) : QUndoStack(parent) {

}

void MyUndoStack::addCommand(QUndoCommand* command) {
    push(command);
}

void MyUndoStack::clear() {
    QUndoStack::clear();
}

// MyChangeStageCommand

MyChangeStageCommand::MyChangeStageCommand(const QJsonObject& previousStageInfo, const QJsonObject& currentStageInfo, QUndoCommand* parent) : QUndoCommand(parent) {
    _previousStageInfo = previousStageInfo;
    _currentStageInfo = currentStageInfo;
}

void MyChangeStageCommand::undo() {
    emit askForCreateNetwork(_previousStageInfo);
}

void MyChangeStageCommand::redo() {
    emit askForCreateNetwork(_currentStageInfo);
}

const QJsonObject& MyChangeStageCommand::previousStageInfo() {
    return _previousStageInfo;
}

const QJsonObject& MyChangeStageCommand::currentStageInfo() {
    return _currentStageInfo;
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

QPushButton* MyWidgetAction::createItemPreviewButton(MyPluginItemBase* item) {
    QPushButton* itemPreviewButton = new MyItemPreviewButton(item);
    connect(itemPreviewButton, &QPushButton::clicked, this, [this, item] () { emit itemIsChosen(item); });

    return itemPreviewButton;
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
