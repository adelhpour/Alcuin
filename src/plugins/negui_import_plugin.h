#include "negui_pyhelper.hpp"
#include "negui_interfaces.h"

class ImportPlugin : public QObject, ImportInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.sys-bio.networkeditorgui.importinterface/1.0")
    Q_INTERFACES(ImportInterface)

public:
    int initialize(const QString &path) override;
    const QJsonObject loadItemsInfo() override;
    const QJsonObject readGraphInfoFromFile(const QString& fileName, const QString& fileType) override;
    
protected:
    QString _path;
    CPyInstance* _pyInstance;
    CPyObject _script;
};

