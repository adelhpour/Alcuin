#ifndef __NEGUI_MAIN_WINDOW_H
#define __NEGUI_MAIN_WINDOW_H

#include <QMainWindow>

class MyMainWindow : public QMainWindow {
    Q_OBJECT

public:

    MyMainWindow(QWidget* parent = nullptr);

    void setMenuBar();

    void connectToCentralWidget(QWidget* menuBar);
};

#endif
