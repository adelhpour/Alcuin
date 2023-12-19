#ifndef __NEGUI_CALL_API_FUNCTION_H
#define __NEGUI_CALL_API_FUNCTION_H

#include <QObject>
#include <QJsonValue>

const QJsonValue callAPIFunction(QObject* mainWidget, const QString& functionName, const QJsonValue& inputs);

#endif
