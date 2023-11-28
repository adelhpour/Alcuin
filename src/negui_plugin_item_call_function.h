#ifndef __NEGUI_PLUGIN_ITEM_CALL_FUNCTION_H
#define __NEGUI_PLUGIN_ITEM_CALL_FUNCTION_H

#include "negui_base.h"

#include <QObject>

class MyPluginItemCallFunction : public QObject, public MyBase{
    Q_OBJECT

public:

    MyPluginItemCallFunction(const QString& name);

    const QStringList& inputAPIFunctions() const;
    void read(const QJsonObject &json) override;
    void write(QJsonObject &json) override;

protected:

    QStringList _inputAPIFunctions;
};

#endif
