#include "negui_arrow_head_style_builder.h"
#include "negui_arrow_head_style.h"

MyElementStyleBase* createArrowHeadStyle(const QString& name) {
    return new MyClassicArrowHeadStyle(name);
}
