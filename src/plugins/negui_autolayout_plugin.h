#include "negui_pyhelper.hpp"
#include <QtPlugin>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include "negui_interfaces.h"

class AutoLayoutPlugin : public QObject, AutoLayoutInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.sys-bio.networkeditorgui.autolayoutinterface/1.0")
    Q_INTERFACES(AutoLayoutInterface)

public:
    int initialize(const QString &path) override;
    const QJsonObject loadItemsInfo() override;
    void autoLayout(QJsonObject &graphInfo, QJsonObject &autoLayoutInfo) override;
    
protected:
    QString _path;
    CPyInstance* _pyInstance;
    CPyObject _script;
};


