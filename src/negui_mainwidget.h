#ifndef __NEGUI_MAINWIDGET_H
#define __NEGUI_MAINWIDGET_H

#include "negui_base.h"
#include "negui_interactor.h"
#include "negui_toolbar.h"
#include "negui_graphics_view.h"

class MyInteractor;
class MyToolBar;
class MyGraphicsView;

class NEGUI_LIB_EXPORT MyNetworkEditorWidget : public QFrame {
    Q_OBJECT
    
public:
    
    explicit MyNetworkEditorWidget(QWidget *parent = nullptr);
    ~MyNetworkEditorWidget();
    
    MyInteractor* interactor();
    MyToolBar* toolBar();
    MyGraphicsView* view();
    
protected:
    
    void setWidgets();
    void setInteractions();
    
    MyInteractor* _interactor;
    MyToolBar* _toolBar;
    MyGraphicsView* _view;
};

#endif
