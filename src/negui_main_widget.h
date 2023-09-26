#ifndef __NEGUI_MAINWIDGET_H
#define __NEGUI_MAINWIDGET_H

#include <QObject>
#include <QWidget>
#include <QFrame>

class MyNetworkEditorWidget : public QFrame {
    Q_OBJECT
    
public:
    
    explicit MyNetworkEditorWidget(QWidget *parent = nullptr);
    ~MyNetworkEditorWidget();

    QObject* interactor();
    QWidget* toolBar();
    QWidget* modeMenu();
    QWidget* view();
    QWidget* secondaryView();
    QWidget* statusBar();
    QWidget* featureMenu();
    void readSettings();
    void writeSettings();

signals:

    void askForSetNewNetworkCanvas();
    const QStringList askForListOfPluginItemNames(const QString&);
    const QStringList askForListOfPluginItemCategories(const QString&);
    void askForReadFromFile(const QString&);
    void askForSaveCurrentNetwork();
    void askForWriteDataToFile(const QString&);
    void askForWriteFigureToFile(const QString&);
    void askForTriggerUndoAction();
    void canUndoChanged(const bool&);
    void askForTriggerRedoAction();
    void canRedoChanged(const bool&);
    void askForCutSelectedNetworkElements();
    void elementsCuttableStatusChanged(const bool&);
    void askForCopySelectedNetworkElements();
    void elementsCopyableStatusChanged(const bool&);
    void askForPasteCopiedNetworkElements();
    void pasteElementsStatusChanged(const bool&);
    void askForSelectAllElements();
    void askForSelectAllElements(const QString&);
    void askForZoomIn();
    void askForZoomOut();

private slots:

    void displayFeatureMenu();
    void displayFeatureMenu(QWidget* featureMenu);
    void removeFeatureMenu();

protected:

    void setWidgets();
    void setInteractions();
    void arrangeWidgetLayers();
    const qreal& layoutMenuRow();
    void setReadyToLaunch();
    void closeEvent(QCloseEvent *event) override;

    QObject* _interactor;
    QWidget* _toolBar;
    QWidget* _view;
    QWidget* _secondaryView;
    QWidget* _modeMenu;
    QWidget* _featureMenu;
    QWidget* _statusBar;
    qreal _layoutMenuRow;
};

#endif
