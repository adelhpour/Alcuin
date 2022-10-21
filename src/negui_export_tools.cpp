#include "negui_export_tools.h"
#include <QFileDialog>
#include <QIcon>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>

// MyExportToolBase

MyExportToolBase::MyExportToolBase(const QString& name) : MyPluginItemBase(name) {
    _iconSize = QSize(75, 25);
}

const QString MyExportToolBase::getSaveFileName() const {
    return QFileDialog::getSaveFileName(NULL, "Save (." + fileExtension() + ") File", defaultSaveFileName(), "(*." + fileExtension() + ")");
}

const QString& MyExportToolBase::fileExtension() const {
    return _fileExtension;
}

const QIcon MyExportToolBase::icon() {
    QPixmap pixMap(iconDirectory());
    return QIcon(pixMap);
}

const QString& MyExportToolBase::iconDirectory() const {
    return _iconDirectory;
}

const QString& MyExportToolBase::defaultSaveFileName() const {
    return _defaultSaveFileName;
}

void MyExportToolBase::read(const QJsonObject &json) {
    // file extension
    if (json.contains("file-extension") && json["file-extension"].isString())
        _fileExtension = json["file-extension"].toString();
    
    // icon directory
    if (json.contains("icon-file") && json["icon-file"].isString())
        _iconDirectory = json["icon-file"].toString();
    
    // default save file name
    if (json.contains("default-save-file-name") && json["default-save-file-name"].isString())
        _defaultSaveFileName = json["default-save-file-name"].toString();
}

void MyExportToolBase::write(QJsonObject &json) {
    // name
    json["name"] = name();
    
    // extension
    json["file-extension"] = fileExtension();
    
    // icon directory
    json["icon-file"] = iconDirectory();
    
    // default save file name
    json["default-save-file-name"] = defaultSaveFileName();
}

// MyDataExportTool

MyDataExportTool::MyDataExportTool(const QString& name) : MyExportToolBase(name) {
    _nodeTypeIndex = 0;
    _edgeTypeIndex = 0;
    _isInfoCompatible = false;
}

QJsonObject MyDataExportTool::getGraphInfoObject() {
    return _graphInfo;
}

const QList<QString>& MyDataExportTool::nodeTypes() const {
    return _nodeTypes;
}

const QList<QString>& MyDataExportTool::edgeTypes() const {
    return _edgeTypes;
}

void MyDataExportTool::readCompatibilityInfo(const QJsonObject &json) {
    // compatibility
    if (json.contains("isInfoCompatible") && json["isInfoCompatible"].isBool())
        _isInfoCompatible = json["isInfoCompatible"].toBool();
    
    // messages
    _messages.clear();
    if (json.contains("messages") && json["messages"].isArray()) {
        QJsonArray messagesArray = json["messages"].toArray();
        for (int messageIndex = 0; messageIndex < messagesArray.size(); ++messageIndex) {
            QJsonObject messageObject = messagesArray[messageIndex].toObject();
            if (messageObject.contains("message") && messageObject["message"].isString())
                _messages.push_back(messageObject["message"].toString());
        }
    }
}

const QString MyDataExportTool::defaultUnsuccessfullExport() const {
    return "The graph info could not be exported.";
}

const QString MyDataExportTool::messagesAsString() const {
    QString messagesString;
    for (int messageIndex = 0; messageIndex < _messages.size(); ++messageIndex) {
        messagesString += "Message " + QString::number(messageIndex + 1) + ": ";
        messagesString += _messages.at(messageIndex);
        if (messageIndex != _messages.size() - 1)
            messagesString += "\n\n";
    }
    return messagesString;
}

void MyDataExportTool::showMessages() {
    if (!isInfoCompatible()) {
        QMessageBox* messageBox = new QMessageBox();
        messageBox->setWindowTitle("Export Info");
        messageBox->setText(defaultUnsuccessfullExport());
        messageBox->setInformativeText(messagesAsString());
        messageBox->setIcon(QMessageBox::Critical);
        messageBox->exec();
    }
}

void MyDataExportTool::annotateElementsWithTypes(QJsonObject &json) {
    _graphInfo = json;
    _nodeTypeIndex = -1;
    _edgeTypeIndex = -1;

    selectElements();
}

void MyDataExportTool::selectElements() {
    emit elementSelectionStarted();
    
    QString enterKeyName = "Enter";
#if defined(Q_OS_MAC)
    enterKeyName = "return";
#endif
    
    if (_nodeTypeIndex + 1 < nodeTypes().size()) {
        ++_nodeTypeIndex;
        QMessageBox* messageBox = new QMessageBox();
        messageBox->setWindowTitle("Select " + nodeTypes().at(_nodeTypeIndex));
        messageBox->setText("Select the \"" + nodeTypes().at(_nodeTypeIndex) + "\" nodes on the Scene and press " + enterKeyName);
        messageBox->setIcon(QMessageBox::Information);
        if (messageBox->exec())
            emit askForNodeSelection(nodeTypes().at(_nodeTypeIndex));
    }
    else if (_edgeTypeIndex + 1 < edgeTypes().size()) {
        ++_edgeTypeIndex;
        QMessageBox* messageBox = new QMessageBox();
        messageBox->setWindowTitle("Select " + edgeTypes().at(_edgeTypeIndex));
        messageBox->setText("Select the \"" + edgeTypes().at(_edgeTypeIndex) + "\" edges on the Scene and press " + enterKeyName);
        messageBox->setIcon(QMessageBox::Information);
        if (messageBox->exec())
            emit askForEdgeSelection(edgeTypes().at(_edgeTypeIndex));
    }
    else
        emit elementSelectionFinished(this);
}

void MyDataExportTool::annotateSelectedElements() {
    // nodes
    QList<QString> selectedNodes = emit askForSelectedNodes();
    if (selectedNodes.size())
        annotateSelectedNodesWithNodeType(selectedNodes, _nodeTypes.at(_nodeTypeIndex));
    
    // edges
    QList<QString> selectedEdges = emit askForSelectedEdges();
    if (selectedEdges.size())
        annotateSelectedEdgesWithEdgeType(selectedEdges, _edgeTypes.at(_edgeTypeIndex));
    
    selectElements();
}

void MyDataExportTool::annotateSelectedNodesWithNodeType(QList<QString> selectedNodes, const QString& nodeType) {
    if (_graphInfo.contains("nodes") && _graphInfo["nodes"].isArray()) {
        QJsonArray nodesArray = _graphInfo["nodes"].toArray();
        for (int nodeIndex = 0; nodeIndex < nodesArray.size(); ++nodeIndex) {
            QJsonObject nodeObject = nodesArray[nodeIndex].toObject();
            if (nodeObject.contains("id") && nodeObject["id"].isString()) {
                for (QString selectedNode : qAsConst(selectedNodes)) {
                    if (nodeObject["id"].toString() == selectedNode)
                        nodeObject["type"] = nodeType;
                }
            }
            nodesArray[nodeIndex] = nodeObject;
        }
        _graphInfo["nodes"] = nodesArray;
    }
}

void MyDataExportTool::annotateSelectedEdgesWithEdgeType(QList<QString> selectedEdges, const QString& edgeType) {
    if (_graphInfo.contains("edges") && _graphInfo["edges"].isArray()) {
        QJsonArray edgesArray = _graphInfo["edges"].toArray();
        for (int edgeIndex = 0; edgeIndex < edgesArray.size(); ++edgeIndex) {
            QJsonObject edgeObject = edgesArray[edgeIndex].toObject();
            if (edgeObject.contains("id") && edgeObject["id"].isString()) {
                for (QString selectedEdge : qAsConst(selectedEdges)) {
                    if (edgeObject["id"].toString() == selectedEdge)
                        edgeObject["type"] = edgeType;
                }
            }
            edgesArray[edgeIndex] = edgeObject;
        }
        _graphInfo["edges"] = edgesArray;
    }
}

void MyDataExportTool::read(const QJsonObject &json) {
    MyExportToolBase::read(json);
    
    // element-types
    _nodeTypes.clear();
    _edgeTypes.clear();
    if (json.contains("element-types") && json["element-types"].isObject()) {
        QJsonObject elementTypesObject = json["element-types"].toObject();
        
        // node types
        if (elementTypesObject.contains("node") && elementTypesObject["node"].isArray()) {
            QJsonArray nodeTypesArray = elementTypesObject["node"].toArray();
            for (int nodeTypeIndex = 0; nodeTypeIndex < nodeTypesArray.size(); ++nodeTypeIndex) {
                if (nodeTypesArray[nodeTypeIndex].isString())
                    _nodeTypes.push_back(nodeTypesArray[nodeTypeIndex].toString());
            }
        }
        
        // edge types
        if (elementTypesObject.contains("edge") && elementTypesObject["edge"].isArray()) {
            QJsonArray edgeTypesArray = elementTypesObject["edge"].toArray();
            for (int edgeTypeIndex = 0; edgeTypeIndex < edgeTypesArray.size(); ++edgeTypeIndex) {
                if (edgeTypesArray[edgeTypeIndex].isString())
                    _edgeTypes.push_back(edgeTypesArray[edgeTypeIndex].toString());
            }
        }
    }
}

void MyDataExportTool::write(QJsonObject &json) {
    MyExportToolBase::write(json);
    
    QJsonObject elementTypesObject;
    
    // node types
    QJsonArray nodeTypesArray;
    for (QString nodeType : qAsConst(nodeTypes()))
        nodeTypesArray.append(nodeType);
    elementTypesObject["node"] = nodeTypesArray;

    // edge types
    QJsonArray edgeTypesArray;
    for (QString edgeType : qAsConst(edgeTypes()))
        edgeTypesArray.append(edgeType);
    elementTypesObject["edge"] = edgeTypesArray;
    
    // name
    json["element-types"] = elementTypesObject;
}

// MyPrintExportTool

MyPrintExportTool::MyPrintExportTool(const QString& name) : MyExportToolBase(name) {
    _outputFormat = QPrinter::NativeFormat;
    
    if (_fileExtension == "pdf")
        _outputFormat = QPrinter::PdfFormat;
}

QPrinter::OutputFormat MyPrintExportTool::outputFormat() {
    return _outputFormat;
}
