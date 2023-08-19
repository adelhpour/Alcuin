#ifndef __NEGUI_STATUS_BAR_H
#define __NEGUI_STATUS_BAR_H

#include <QStatusBar>

class MyStatusBar : public QStatusBar {
    Q_OBJECT

public:

    MyStatusBar(QWidget* parent = nullptr);

public slots:

    void resetMessage();

    void setMessageToMousePosition(const QPointF& mousePosition);
};

#endif
