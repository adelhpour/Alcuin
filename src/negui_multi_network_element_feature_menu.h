#ifndef __NEGUI_MULTI_ELEMENT_FEATURE_MENU_H
#define __NEGUI_MULTI_ELEMENT_FEATURE_MENU_H

#include "negui_feature_menu_base.h"
#include "negui_network_element_base.h"

class MyMultiNetworkElementFeatureMenu : public MyFeatureMenuBase {
    Q_OBJECT

public:

    MyMultiNetworkElementFeatureMenu(QList<MyNetworkElementBase*> networkElements, const QString& iconsDirectoryPath, QWidget *parent = nullptr);

    FEATURE_MENU_TYPE type() override;

    void addItems();

    void addShapeStyleButtons();

    void addParameter(MyParameterBase* parameter);

    MyParameterBase* createRepresentativeBorderWidthParameter();

    const qint32 getRepresentativeBorderWidthParameterDefaultValue();

    const qint32 getCommonBorderWidthParameterValueDefaultValue();

    void updateBorderWidthParameters(const qint32& borderWidth);

    MyParameterBase* createRepresentativeBorderColorParameter();

    const QString getRepresentativeBorderColorParameterDefaultValue();

    const QString getCommonBorderColorParameterValueDefaultValue();

    void updateBorderColorParameters(const QString& borderColor);

    MyParameterBase* createRepresentativeFillColorParameter();

    const QString getRepresentativeFillColorParameterDefaultValue();

    const QString getCommonFillColorParameterValueDefaultValue();

    void updateFillColorParameters(const QString& fillColor);

    MyParameterBase* createRepresentativeTextColorParameter();

    const QString getRepresentativeTextColorParameterDefaultValue();

    const QString getCommonTextColorParameterValueDefaultValue();

    void updateTextColorParameters(const QString& textColor);

    void updateNetworkElements();

    QList<MyParameterBase*> getNetworkElementParameters(const QString& parameterName);

signals:

    void askForCreateChangeStageCommand();

private slots:

    void updateShapeStyles(MyShapeStyleBase* shapeStyle);

protected:
    QList<MyNetworkElementBase*> _networkElements;
};

#endif
