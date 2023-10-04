#ifndef __NEGUI_INTERFACES_H
#define __NEGUI_INTERFACES_H

#include <QJsonObject>
#include <QtPlugin>

// interface base
class InterfaceBase {
public:
    virtual ~InterfaceBase() = default;
    virtual int initialize(const QString &appPath, const QString &pluginsPath) = 0;
    virtual const QJsonObject loadItemsInfo() = 0;
};

// import interface
class ImportInterface : public InterfaceBase {
public:
    virtual const QJsonObject readGraphInfoFromFile(const QString& fileName, const QString& fileType) = 0;
};

#define ImportInterface_iid "org.sys-bio.alcuin.importinterface/1.0"

Q_DECLARE_INTERFACE(ImportInterface, ImportInterface_iid)

// data export interface
class DataExportInterface : public InterfaceBase {
public:
    virtual void writeGraphInfoToFile(const QJsonObject& graphInfoObject, const QString& fileName, const QString& fileType) = 0;
    virtual const QJsonObject checkForGraphInfoCompatibility(const QJsonObject& graphInfoObject, const QString& fileType) = 0;
};

#define DataExportInterface_iid "org.sys-bio.alcuin.dataexportinterface/1.0"

Q_DECLARE_INTERFACE(DataExportInterface, DataExportInterface_iid)

// print export interface
class PrintExportInterface : public InterfaceBase {
    
};

#define PrintExportInterface_iid "org.sys-bio.alcuin.printexportinterface/1.0"

Q_DECLARE_INTERFACE(PrintExportInterface, PrintExportInterface_iid)

// element style interface
class ElementStyleInterface : public InterfaceBase {
    
};

#define ElementStyleInterface_iid "org.sys-bio.alcuin.elementstyleinterface/1.0"

Q_DECLARE_INTERFACE(ElementStyleInterface, ElementStyleInterface_iid)

// auto layout interface
class AutoLayoutInterface : public InterfaceBase {
public:
    virtual void autoLayout(QJsonObject &graphInfo, QJsonObject &autoLayoutInfo) = 0;
};

#define AutoLayoutInterface_iid "org.sys-bio.alcuin.autolayoutinterface/1.0"

Q_DECLARE_INTERFACE(AutoLayoutInterface, AutoLayoutInterface_iid)

#endif
