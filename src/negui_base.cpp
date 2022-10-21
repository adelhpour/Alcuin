#include "negui_base.h"

// MyBase

MyBase::MyBase(const QString& name) {
    _name = name;
}

const QString& MyBase::name() const {
    return _name;
}
