#ifndef __NEGUI_AUTOLAYOUT_ENGINES_H
#define __NEGUI_AUTOLAYOUT_ENGINES_H

#include "negui_plugin_item_base.h"
#include "negui_parameters.h"
#include <QDialog>

class MyAutoLayoutEngine : public MyPluginItemBase {
    
public:
    
    MyAutoLayoutEngine(const QString& name);
    
    ~MyAutoLayoutEngine();
    
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

class MyDialog : public QDialog {
    Q_OBJECT

public:
    
    MyDialog(QWidget *parent = nullptr);
};

#endif
