#ifndef __NEGUI_TOOLBAR_H
#define __NEGUI_TOOLBAR_H

#include <QToolBar>
#include <QAbstractButton>

class MyToolBar : public QToolBar {
    Q_OBJECT
    
public:
    
    MyToolBar(QWidget* parent = nullptr);
    
public slots:
    
    void setButtons(QList<QAbstractButton*> buttons);
};

#endif
