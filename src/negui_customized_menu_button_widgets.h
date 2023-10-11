#ifndef __NEGUI_CUSTOMIZED_MODE_MENU_WIDGETS_H
#define __NEGUI_CUSTOMIZED_MODE_MENU_WIDGETS_H

#include "negui_customized_common_widgets.h"
#include "negui_scene_mode_element_base.h"
#include "negui_plugin_item_base.h"

#include <QWidgetAction>

class MyModeMenuButton : public MyToolButton {
    Q_OBJECT

public:

    MyModeMenuButton(QWidget* parent = nullptr);

    void setActive(const bool& active);

    void setStyleToActiveForm();

    void setStyleToInactiveForm();
};

class MyModeMenuModeButton : public MyModeMenuButton, public MySceneModeElementBase {
    Q_OBJECT

public:

    MyModeMenuModeButton(const QString& mode = "", const QString& alternativeSimilarMode = "", QWidget* parent = nullptr);
};

class MyExtraElementMenu : public QMenu {
    Q_OBJECT

public:

    MyExtraElementMenu(QWidget *parent = nullptr);

protected:

    bool event(QEvent *event) override;

    qint32 _horizontalPadding;

};

class MyExtraElementCategoryButton : public MyToolButton {
    Q_OBJECT

public:

    MyExtraElementCategoryButton(QMenu* elementCategoryMenu, const QString& text, QWidget* parent = nullptr);

signals:

    void categoryItemIsChosen();
};

class MyExtraElementCategoryMenu : public MyMenu {
    Q_OBJECT

public:

    MyExtraElementCategoryMenu(QWidget* parent = nullptr);

signals:

    void categoryItemIsChosen();

protected:

    bool event(QEvent *event) override;

    qint32 _horizontalPadding;
};

class MyMenuButtonWidgetAction : public QWidgetAction {
    Q_OBJECT

public:

    MyMenuButtonWidgetAction(QObject* parent = nullptr);

    void setItems(QList<MyPluginItemBase*> items);

signals:

    void itemIsChosen(MyPluginItemBase*);

protected:

    QWidget* createMenuItemPreviewWidget(QList<MyPluginItemBase*> items);
    QToolButton* createMenuItemPreviewButton(MyPluginItemBase* item);
};

class MyMenuItemPreviewButton : public MyModeMenuButton {
    Q_OBJECT

public:

    MyMenuItemPreviewButton(MyPluginItemBase* item, QWidget *parent = nullptr);

signals:

    void itemIsChosen(MyPluginItemBase*);

};

#endif
