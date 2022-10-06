#include "negui_pyhelper.hpp"
#include <QtPlugin>
#include <QString>
#include "negui_mainwidget.h"

class PrintExportPlugin : public QObject, PrintExportInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.sys-bio.networkeditorgui.printexportinterface/1.0")
    Q_INTERFACES(PrintExportInterface)

public:
    int initialize(const QString &path) override;
    const QJsonObject loadItemsInfo() override;
    
protected:
    QString _path;
    CPyInstance* _pyInstance;
    CPyObject _script;
};


