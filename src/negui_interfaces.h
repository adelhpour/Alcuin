#ifndef __NEGUI_INTERFACES_H
#define __NEGUI_INTERFACES_H

#include <QJsonObject>
#include <QtPlugin>

// interface base
class InterfaceBase {
public:
    virtual ~InterfaceBase() = default;
    virtual int initialize(const QString &path) = 0;
    virtual const QJsonObject loadItemsInfo() = 0;
};

// import interface
class ImportInterface : public InterfaceBase {
public:
    virtual const QJsonObject readGraphInfoFromFile(const QString& fileName, const QString& fileType) = 0;
};

#define ImportInterface_iid "org.sys-bio.networkeditorgui.importinterface/1.0"

Q_DECLARE_INTERFACE(ImportInterface, ImportInterface_iid)

// data export interface
class DataExportInterface : public InterfaceBase {
public:
    virtual void writeGraphInfoToFile(const QJsonObject& graphInfoObject, const QString& fileName, const QString& fileType) = 0;
    virtual const QJsonObject checkForGraphInfoCompatibiliy(const QJsonObject& graphInfoObject, const QString& fileType) = 0;
};

#define DataExportInterface_iid "org.sys-bio.networkeditorgui.dataexportinterface/1.0"

Q_DECLARE_INTERFACE(DataExportInterface, DataExportInterface_iid)

// print export interface
class PrintExportInterface : public InterfaceBase {
    
};

#define PrintExportInterface_iid "org.sys-bio.networkeditorgui.printexportinterface/1.0"

Q_DECLARE_INTERFACE(PrintExportInterface, PrintExportInterface_iid)

// node style interface
class NodeStyleInterface : public InterfaceBase {
    
};

#define NodeStyleInterface_iid "org.sys-bio.networkeditorgui.nodestyleinterface/1.0"

Q_DECLARE_INTERFACE(NodeStyleInterface, NodeStyleInterface_iid)

// edge style interface
class EdgeStyleInterface : public InterfaceBase {
    
};

#define EdgeStyleInterface_iid "org.sys-bio.networkeditorgui.edgestyleinterface/1.0"

Q_DECLARE_INTERFACE(EdgeStyleInterface, EdgeStyleInterface_iid)

// auto layout interface
class AutoLayoutInterface : public InterfaceBase {
public:
    virtual void autoLayout(QJsonObject &graphInfo, QJsonObject &autoLayoutInfo) = 0;
};

#define AutoLayoutInterface_iid "org.sys-bio.networkeditorgui.autolayoutinterface/1.0"

Q_DECLARE_INTERFACE(AutoLayoutInterface, AutoLayoutInterface_iid)

#endif
