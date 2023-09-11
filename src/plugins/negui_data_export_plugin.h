#include "negui_pyhelper.hpp"
#include "negui_interfaces.h"

class DataExportPlugin : public QObject, DataExportInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.sys-bio.networkeditorgui.dataexportinterface/1.0")
    Q_INTERFACES(DataExportInterface)

public:
    int initialize(const QString &appPath, const QString &pluginsPath) override;
    const QJsonObject loadItemsInfo() override;
    void writeGraphInfoToFile(const QJsonObject& graphInfoObject, const QString& fileName, const QString& fileType) override;
    const QJsonObject checkForGraphInfoCompatibility(const QJsonObject& graphInfoObject, const QString& fileType) override;
    
protected:
    CPyInstance* _pyInstance;
    CPyObject _script;
};

