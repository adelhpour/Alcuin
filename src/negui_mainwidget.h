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
    QWidget* title();
    QWidget* toolBar();
    QWidget* modeMenu();
    QWidget* view();

private slots:

    void displayFeatureMenu(QWidget* featureMenu);
    void removeFeatureMenu();

protected:
    
    void setWidgets();
    void setInteractions();
    void setReadyToLaunch();

    QObject* _interactor;
    QWidget* _title;
    QWidget* _toolBar;
    QWidget* _view;
    QWidget* _modeMenu;
    QWidget* _featureMenu;
};

#endif
