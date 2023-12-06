#include "negui_pyhelper.hpp"
#include "negui_interfaces.h"

class GeneralPlugin : public QObject, GeneralInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.sys-bio.alcuin.generalinterface/1.0")
    Q_INTERFACES(GeneralInterface)

public:
    int initialize(const QString &appPath, const QString &pluginsPath) override;
    const QJsonObject loadItemsInfo() override;
    const QJsonObject call(const QString& functionName, const QStringList& inputList) override;
    const QJsonValue call1(const QString& functionName, const QJsonValue& functionInput) override;

protected:
    void loadPluginScripts(const QString &pluginsPath);
    const QJsonObject loadItemsInfo(CPyObject script);
    const QJsonObject addItemsInfo(QJsonObject itemsInfo, const QJsonObject& newItemsInfo);
    CPyObject createFunctionInput(const QStringList& functionInputList);
    CPyObject processFunctionInput(const QJsonValue& functionInput);
    const QJsonObject processFunctionOutput(CPyObject functionOutput);
    const QJsonValue processFunctionOutput1(CPyObject functionOutput);

    CPyInstance* _pyInstance;
    QList<CPyObject> _scripts;
};


