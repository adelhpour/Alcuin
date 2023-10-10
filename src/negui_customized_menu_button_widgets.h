#ifndef __NEGUI_CUSTOMIZED_MENU_BUTTON_WIDGETS_H
#define __NEGUI_CUSTOMIZED_MENU_BUTTON_WIDGETS_H

#include "negui_customized_mode_menu_widgets.h"
#include "negui_plugin_item_base.h"

#include <QToolButton>
#include <QWidgetAction>

class MyWidgetAction : public QWidgetAction {
    Q_OBJECT

public:

    MyWidgetAction(QObject* parent = nullptr);

    void setItems(QList<MyPluginItemBase*> items);

    signals:

            void itemIsChosen(MyPluginItemBase*);

protected:

    QWidget* createItemPreviewWidget(QList<MyPluginItemBase*> items);
    QToolButton* createItemPreviewButton(MyPluginItemBase* item);
};

class MyItemPreviewButton : public MyModeMenuToolButton {
    Q_OBJECT

public:

    MyItemPreviewButton(MyPluginItemBase* item, QWidget *parent = nullptr);

    void hoverOut();

    signals:

            void itemIsChosen(MyPluginItemBase*);

};

#endif
