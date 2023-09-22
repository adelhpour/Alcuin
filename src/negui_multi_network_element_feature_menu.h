#ifndef __NEGUI_MULTI_ELEMENT_FEATURE_MENU_H
#define __NEGUI_MULTI_ELEMENT_FEATURE_MENU_H

#include "negui_feature_menu_base.h"
#include "negui_network_element_base.h"

class MyMultiNetworkElementFeatureMenu : public MyFeatureMenuBase {
    Q_OBJECT

public:

    MyMultiNetworkElementFeatureMenu(QList<MyNetworkElementBase*> networkElements, const QString& iconsDirectoryPath, QWidget *parent = nullptr);

    FEATURE_MENU_TYPE type() override;

    void addParameters();

    void addParameter(MyParameterBase* parameter);

    MyParameterBase* createRepresentativeBorderWidthParameter();

    void updateBorderWidthParameters(const qint32& borderWidth);

    MyParameterBase* createRepresentativeBorderColorParameter();

    void updateBorderColorParameters(const QString& borderColor);

    MyParameterBase* createRepresentativeFillColorParameter();

    void updateFillColorParameters(const QString& fillColor);

    void updateNetworkElements();

    QList<MyParameterBase*> getNetworkElementParameters(const QString& parameterName);

signals:

    void askForCreateChangeStageCommand();

protected:
    QList<MyNetworkElementBase*> _networkElements;
};

#endif
