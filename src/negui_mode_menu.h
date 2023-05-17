#ifndef __NEGUI_MODE_MENU_H
#define __NEGUI_MODE_MENU_H

#include "negui_customized_common_widgets.h"

#include <QWidgetAction>

class MyModeMenu : public MyFrame {
    Q_OBJECT

public:

    MyModeMenu(QWidget *parent = nullptr);

    void setAddButtons(QList<QToolButton*> buttons);

    void setRemoveButton(QToolButton* button);

    QToolButton* createAddButton(QList<QToolButton*> buttons);

    QMenu* createAddButtonMenu(QList<QToolButton*> buttons);

    QWidgetAction* createButtonWidgetAction(QToolButton* button);
};

#endif

