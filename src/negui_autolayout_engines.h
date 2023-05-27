#ifndef __NEGUI_AUTOLAYOUT_ENGINES_H
#define __NEGUI_AUTOLAYOUT_ENGINES_H

#include "negui_plugin_item_base.h"
#include "negui_parameters.h"
#include "negui_customized_common_widgets.h"

class MyAutoLayoutEngine : public MyPluginItemBase {
    
public:
    
    MyAutoLayoutEngine(const QString& name);
    
    ~MyAutoLayoutEngine();
    const QString type() const override;
    
    // parameters
    void addParameter(MyParameterBase* p);
    void removeParameter(MyParameterBase* p);
    QList<MyParameterBase*>& parameters();
    int takeParameters();
    void clearParameters();
    
    // icon
    const QIcon icon() override;
    const QString& iconDirectory() const;
    
    // import/export info
    void read(const QJsonObject &json) override;
    void write(QJsonObject &json) override;

protected:
    
    QList<MyParameterBase*> _parameters;
    QString _iconDirectory;
};

class MyParameterSetDialog : public MyDialog {
    Q_OBJECT

public:

    MyParameterSetDialog(QList<MyParameterBase*> parameters, QWidget *parent = nullptr);
};

#endif
