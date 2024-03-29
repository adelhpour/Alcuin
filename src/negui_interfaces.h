#ifndef __NEGUI_INTERFACES_H
#define __NEGUI_INTERFACES_H

#include <QJsonObject>
#include <QtPlugin>

// interface base
class InterfaceBase {
public:
    virtual ~InterfaceBase() = default;
    virtual int initialize(const QString &appPath, const QString &pluginsPath) = 0;
    virtual const QJsonArray loadPluginsInfo() = 0;
};

class GeneralInterface : public InterfaceBase {
public:
    virtual const QJsonValue call(const QString& functionName, const QJsonValue& functionInput) = 0;
};

#define GeneralInterface_iid "org.sys-bio.alcuin.generalinterface/1.0"

Q_DECLARE_INTERFACE(GeneralInterface, GeneralInterface_iid)

#endif
