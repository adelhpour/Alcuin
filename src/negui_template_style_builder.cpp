#include "negui_template_style_builder.h"
#include "negui_template_style.h"

MyElementStyleBase* createTemplateStyle(const QString& name) {
    return new MyTemplateStyle(name);
}
