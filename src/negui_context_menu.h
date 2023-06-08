#ifndef __NEGUI_CONTEXT_MENU_H
#define __NEGUI_CONTEXT_MENU_H

#include <QMenu>

class MyContextMenu : public QMenu {
    Q_OBJECT

public:

    MyContextMenu(QWidget *parent = nullptr);
};

#endif
