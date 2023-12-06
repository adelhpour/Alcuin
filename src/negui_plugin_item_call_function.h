#ifndef __NEGUI_PLUGIN_ITEM_CALL_FUNCTION_H
#define __NEGUI_PLUGIN_ITEM_CALL_FUNCTION_H

#include "negui_base.h"

#include <QObject>

class MyPluginItemCallFunction : public QObject, public MyBase {
    Q_OBJECT

public:

    MyPluginItemCallFunction(const QString& name);
    void call();
    const QJsonValue call(QJsonValue json);

    void read(const QJsonObject &json) override;
    void write(QJsonObject &json) override;

signals:

    QJsonValue askForCallPythonFunction(const QString, const QJsonValue&);
    QJsonValue askForCallCPlusPlusFunction(const QString, const QJsonValue&);

protected:

    const QJsonArray processInputArray(QJsonValue json);

    QJsonValue _callFunctionInfo;
};

#endif
