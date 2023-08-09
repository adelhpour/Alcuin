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
    QWidget* title();
    QWidget* toolBar();
    QWidget* modeMenu();
    QWidget* view();
    void readSettings();
    void writeSettings();

signals:

    void askForSetNewNetworkCanvas();
    const QStringList askForListOfPluginItemNames(const QString&);
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
    void askForSelectElements();

private slots:

    void displayFeatureMenu(QWidget* featureMenu);
    void removeFeatureMenu();

protected:
    
    void setWidgets();
    void setInteractions();
    void setReadyToLaunch();
    void closeEvent(QCloseEvent *event) override;

    QObject* _interactor;
    QWidget* _title;
    QWidget* _toolBar;
    QWidget* _view;
    QWidget* _modeMenu;
    QWidget* _featureMenu;
};

#endif
