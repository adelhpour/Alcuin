#include "negui_pyhelper.hpp"
#include "negui_interfaces.h"

class ImportPlugin : public QObject, ImportInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.sys-bio.alcuin.importinterface/1.0")
    Q_INTERFACES(ImportInterface)

public:
    int initialize(const QString &appPath, const QString &pluginsPath) override;
    const QJsonObject loadItemsInfo() override;
    const QJsonObject readGraphInfoFromFile(const QString& fileName, const QString& fileType) override;
    
protected:
    CPyInstance* _pyInstance;
    CPyObject _script;
};


