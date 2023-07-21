#include "negui_pyhelper.hpp"
#include "negui_interfaces.h"

class ElementStylePlugin : public QObject, ElementStyleInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.sys-bio.networkeditorgui.elementstyleinterface/1.0")
    Q_INTERFACES(ElementStyleInterface)

public:
    int initialize(const QString &appPath, const QString &pluginsPath) override;
    const QJsonObject loadItemsInfo() override;
    
protected:
    CPyInstance* _pyInstance;
    CPyObject _script;
};


