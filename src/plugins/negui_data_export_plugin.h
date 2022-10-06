#include "negui_pyhelper.hpp"
#include <QtPlugin>
#include <QString>
#include "negui_mainwidget.h"

class DataExportPlugin : public QObject, DataExportInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.sys-bio.networkeditorgui.dataexportinterface/1.0")
    Q_INTERFACES(DataExportInterface)

public:
    int initialize(const QString &path) override;
    const QJsonObject loadItemsInfo() override;
    void writeGraphInfoToFile(const QJsonObject& graphInfoObject, const QString& fileName, const QString& fileType) override;
    const QJsonObject checkForGraphInfoCompatibiliy(const QJsonObject& graphInfoObject, const QString& fileType) override;
    
protected:
    QString _path;
    CPyInstance* _pyInstance;
    CPyObject _script;
};

