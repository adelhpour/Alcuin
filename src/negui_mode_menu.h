#ifndef __NEGUI_MODE_MENU_H
#define __NEGUI_MODE_MENU_H

#include "negui_customized_common_widgets.h"

#include <QWidgetAction>

class MyModeMenu : public MyFrame {
    Q_OBJECT

public:

    MyModeMenu(QWidget *parent = nullptr);

    void setNormalModeButton(QToolButton* button);

    QToolButton* decorateNormalButton(QToolButton* button);

    void setSelectModeButton(QToolButton* button);

    QToolButton* decorateSelectButton(QToolButton* button);

    void setAddModeButtons(QList<QToolButton*> buttons);

    QToolButton* decorateAddButton(QList<QToolButton*> buttons);

    QMenu* createAddButtonMenu(QList<QToolButton*> buttons);

    QWidgetAction* createButtonWidgetAction(QToolButton* button);

    void setRemoveModeButton(QToolButton* button);

    QToolButton* decorateRemoveButton(QToolButton* button);

    void setZoomInButton(QToolButton* button);

    QToolButton* decorateZoomInButton(QToolButton* button);

    void setZoomOutButton(QToolButton* button);

    QToolButton* decorateZoomOutButton(QToolButton* button);
};

#endif

