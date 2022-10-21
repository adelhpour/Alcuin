#ifndef __NEGUI_MAINWIDGET_H
#define __NEGUI_MAINWIDGET_H

#if defined MAKE_NEGUI_LIB
    #define NEGUI_LIB_EXPORT Q_DECL_EXPORT
#else
    #define NEGUI_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include <QWidget>
#include <QFrame>

class NEGUI_LIB_EXPORT MyNetworkEditorWidget : public QFrame {
    Q_OBJECT
    
public:
    
    explicit MyNetworkEditorWidget(QWidget *parent = nullptr);
    ~MyNetworkEditorWidget();
    
    QObject* interactor();
    QWidget* toolBar();
    QWidget* view();
    
protected:
    
    void setWidgets();
    void setInteractions();
    
    QWidget* _toolBar;
    QWidget* _view;
    QObject* _interactor;
};

#endif
