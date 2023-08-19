#ifndef __NEGUI_STATUS_BAR_H
#define __NEGUI_STATUS_BAR_H

#include <QStatusBar>
#include <QLabel>

class MyStatusBar : public QStatusBar {
    Q_OBJECT

public:

    MyStatusBar(QWidget* parent = nullptr);

public slots:

    void resetMessage();

    void setFileName(const QString& fileName);

    void setCoordinatesToMousePosition(const QPointF& mousePosition);

protected:
    QLabel* _fileNameLabel;
    QLabel* _coordinatesLabel;
};

#endif
