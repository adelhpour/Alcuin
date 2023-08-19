#ifndef __NEGUI_BASE_H
#define __NEGUI_BASE_H

#include <QJsonObject>

class MyBase {
    
public:
    
    MyBase(const QString& name);
    
    const QString& name() const;

    void setName(const QString& name);
    
    // import/export info
    virtual void read(const QJsonObject &json) = 0;
    virtual void write(QJsonObject &json) = 0;

protected:
    
    QString _name;
};

#endif
