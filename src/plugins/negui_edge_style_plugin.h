#include "negui_pyhelper.hpp"
#include "negui_interfaces.h"

class EdgeStylePlugin : public QObject, EdgeStyleInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.sys-bio.networkeditorgui.edgestyleinterface/1.0")
    Q_INTERFACES(EdgeStyleInterface)

public:
    int initialize(const QString &path) override;
    const QJsonObject loadItemsInfo() override;
    
protected:
    QString _path;
    CPyInstance* _pyInstance;
    CPyObject _script;
};


