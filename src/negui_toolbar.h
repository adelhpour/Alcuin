#ifndef __NEGUI_TOOLBAR_H
#define __NEGUI_TOOLBAR_H

#include <QToolBar>
#include <QToolButton>

class MyToolBar : public QToolBar {
    Q_OBJECT
    
public:
    
    MyToolBar(QWidget* parent = nullptr);
    
public slots:
    
    void setButtons(QList<QToolButton*> buttons);
};

#endif
