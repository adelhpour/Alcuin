#include "negui_pyhelper.hpp"
#include "negui_interfaces.h"

class GeneralPlugin : public QObject, GeneralInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.sys-bio.alcuin.generalinterface/1.0")
    Q_INTERFACES(GeneralInterface)

public:
    int initialize(const QString &appPath, const QString &pluginsPath) override;
    const QJsonObject loadItemsInfo() override;
    QStringList call(const QString& functionName, const QStringList& inputList) override;

protected:
    CPyInstance* _pyInstance;
    QList<CPyObject> _scripts;
};


