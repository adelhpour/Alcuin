#ifndef __NEGUI_MODE_MENU_H
#define __NEGUI_MODE_MENU_H

#include "negui_customized_common_widgets.h"

#include <QWidgetAction>

class MyModeMenu : public MyFrame {
    Q_OBJECT

public:

    MyModeMenu(QWidget *parent = nullptr);

    void setNormalModeButton(QToolButton* button);

    void setSelectModeButton(QToolButton* button);

    void setAddModeButtons(QList<QToolButton*> buttons);

    void setRemoveModeButton(QToolButton* button);

    QToolButton* createAddButton(QList<QToolButton*> buttons);

    QMenu* createAddButtonMenu(QList<QToolButton*> buttons);

    QWidgetAction* createButtonWidgetAction(QToolButton* button);
};

#endif

