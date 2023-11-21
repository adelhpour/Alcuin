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
    void loadPluginScripts(const QString &pluginsPath);
    const QJsonObject loadItemsInfo(CPyObject script);
    const QJsonObject addItemsInfo(QJsonObject itemsInfo, const QJsonObject& newItemsInfo);
    const QJsonObject createItemsInfoJsonObject(CPyObject itemsInfo);
    CPyObject createPythonInput(const QStringList& inputList);
    const QStringList processPythonOutput(CPyObject output);

    CPyInstance* _pyInstance;
    QList<CPyObject> _scripts;
};


