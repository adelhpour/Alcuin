#ifndef __NEGUI_CUSTOMIZED_CONTEXT_MENU_WIDGETS_H
#define __NEGUI_CUSTOMIZED_CONTEXT_MENU_WIDGETS_H

#include <QAction>

class MyCheckableAction : public QAction {
    Q_OBJECT

public:

    MyCheckableAction(const QString &text, QObject *parent = nullptr);
};

#endif
