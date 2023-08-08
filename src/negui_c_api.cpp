#include "negui_c_api.h"
#include "negui_main_widget.h"
#include "negui_interactor.h"
#include <QJsonDocument>

QWidget* createNetworkEditorWidget(QWidget* parent) {
    return new MyNetworkEditorWidget(parent);
}

void deleteNetworkEditorWidget(QWidget* networkEditorWidget) {
    delete networkEditorWidget;
}

void setGraphInfoNetworkEditorWidget(QWidget* networkEditorWidget, const char* graphInfo) {
    if (networkEditorWidget) {
        QJsonDocument doc = QJsonDocument::fromJson(QString(graphInfo).toUtf8());
        if (!doc.isNull())
            ((MyInteractor*)((MyNetworkEditorWidget*)networkEditorWidget)->interactor())->createNetwork(doc.object());
    }
}

const char* getGraphInfoNetworkEditorWidget(QWidget* networkEditorWidget) {
    if (networkEditorWidget) {
        QJsonObject object = ((MyInteractor*)((MyNetworkEditorWidget*)networkEditorWidget)->interactor())->exportNetworkInfo();
        return strdup(QJsonDocument(object).toJson(QJsonDocument::Compact).toStdString().c_str());
    }
    
    return "";
}
