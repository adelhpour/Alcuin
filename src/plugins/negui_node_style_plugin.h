#include "negui_pyhelper.hpp"
#include "negui_interfaces.h"

class NodeStylePlugin : public QObject, NodeStyleInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.sys-bio.networkeditorgui.nodestyleinterface/1.0")
    Q_INTERFACES(NodeStyleInterface)

public:
    int initialize(const QString &path) override;
    const QJsonObject loadItemsInfo() override;
    
protected:
    QString _path;
    CPyInstance* _pyInstance;
    CPyObject _script;
};


