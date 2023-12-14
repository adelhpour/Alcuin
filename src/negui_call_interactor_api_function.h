#ifndef __NEGUI_CALL_INTERACTOR_API_FUNCTION_H
#define __NEGUI_CALL_INTERACTOR_API_FUNCTION_H

#include <QObject>
#include <QJsonValue>

const QJsonValue callInteractorAPIFunction(QObject* interactor, const QString& functionName, const QJsonValue& inputs);

#endif
