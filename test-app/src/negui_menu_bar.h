#ifndef __NEGUI_MENUBAR_H
#define __NEGUI_MENUBAR_H

#include <QMenuBar>

class MyMenuBar : public QMenuBar {
    Q_OBJECT

public:

    MyMenuBar(QWidget* parent = nullptr);

signals:

    void askForSetNewNetworkCanvas();
    void askForSelectElements();
};

#endif
