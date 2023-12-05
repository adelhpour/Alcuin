#ifndef __NEGUI_PLUGIN_ITEM_CALL_FUNCTION_H
#define __NEGUI_PLUGIN_ITEM_CALL_FUNCTION_H

#include "negui_base.h"

#include <QObject>

class MyPluginItemCallFunction : public QObject, public MyBase {
    Q_OBJECT

public:

    MyPluginItemCallFunction(const QString& name);
    void call();
    const QJsonObject call(QJsonObject json);

    void read(const QJsonObject &json) override;
    void write(QJsonObject &json) override;

signals:

    QJsonObject askForCallPythonFunction(const QString, const QJsonObject&);
    QJsonObject askForCallCPlusPlusFunction(const QString, const QJsonObject&);

protected:

    const QJsonArray processInputArray(QJsonObject json);

    QJsonObject _callFunctionInfo;
};

#endif
