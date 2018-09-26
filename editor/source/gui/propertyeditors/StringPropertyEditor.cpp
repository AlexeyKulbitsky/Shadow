#include "StringPropertyEditor.h"

StringPropertyEditor::StringPropertyEditor(sh::Serializable* object, sh::Property* objectProperty)
    : m_object(object)
    , m_objectProperty(objectProperty)
{
    SH_ASSERT(object, "Can not create FloatProperty editor for empty object");
    SH_ASSERT(objectProperty, "Can not create FloatProperty editor for empty property");
    SH_ASSERT(objectProperty->GetType() == sh::VAR_STRING, "Can not create FloatProperty editor. Property type does not match");
    
    sh::gui::HorizontalLayoutPtr mainLayout(new sh::gui::HorizontalLayout());
    sh::gui::LabelPtr label(new sh::gui::Label(objectProperty->GetName()));
    label->SetMaximumWidth(75U);
    mainLayout->AddWidget(label);
    SetLayout(mainLayout);
    SetMaximumHeight(20);
    
    const auto value = objectProperty->GetValue(object).GetString();
    sh::gui::LineEditPtr lineEdit(new sh::gui::LineEdit());
    mainLayout->AddWidget(lineEdit);
    lineEdit->SetText(value);
    lineEdit->OnTextChanged.Connect(std::bind(&StringPropertyEditor::OnValueChanged, this, std::placeholders::_1));
}

void StringPropertyEditor::OnValueChanged(const sh::String& value)
{
    m_objectProperty->SetValue(m_object, value);
}

