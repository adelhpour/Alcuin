#include "negui_pyhelper.hpp"
#include "negui_interfaces.h"

class GeneralPlugin : public QObject, GeneralInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.sys-bio.alcuin.generalinterface/1.0")
    Q_INTERFACES(GeneralInterface)

public:
    int initialize(const QString &appPath, const QString &pluginsPath) override;
    const QJsonArray loadPluginsInfo() override;
    const QJsonValue call(const QString& functionName, const QJsonValue& functionInput) override;

protected:
    void loadPluginScripts(const QString &pluginsPath);
    const QJsonArray loadPluginsInfo(CPyObject script);
    CPyObject createFunctionInput(const QStringList& functionInputList);
    CPyObject processFunctionInput(const QJsonValue& functionInput);
    CPyObject createPythonObject(const QJsonValue& input);
    const QJsonValue processFunctionOutput(CPyObject functionOutput);

    CPyInstance* _pyInstance;
    QList<CPyObject> _scripts;
};


