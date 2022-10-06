#include "negui_c_api.h"

MyNetworkEditorWidget* MyNetworkEditorWidget_new(QWidget* parent) {
    return new MyNetworkEditorWidget(parent);
}

void MyNetworkEditorWidget_delete(MyNetworkEditorWidget* myNetworkEditorWidget) {
    delete myNetworkEditorWidget;
}

void MyNetworkEditorWidget_setGraphInfo(MyNetworkEditorWidget* myNetworkEditorWidget, const char* graphInfo) {
    if (myNetworkEditorWidget) {
        QJsonDocument doc = QJsonDocument::fromJson(QString(graphInfo).toUtf8());
        if (!doc.isNull())
            myNetworkEditorWidget->interactor()->createNetwork(doc.object());
    }
}

const char* MyNetworkEditorWidget_graphInfo(MyNetworkEditorWidget* myNetworkEditorWidget) {
    if (myNetworkEditorWidget) {
        QJsonObject object;
        myNetworkEditorWidget->interactor()->exportNetworkInfo(object);
        return strdup(QJsonDocument(object).toJson(QJsonDocument::Compact).toStdString().c_str());
    }
    
    return "";
}
