#include "NamedResourceRefListPropertyEditor.h"

NamedResourceRefListPropertyEditor::NamedResourceRefListPropertyEditor(sh::Serializable* object, sh::Property* objectProperty)
    : m_object(object)
    , m_objectProperty(objectProperty)
{
    SH_ASSERT(object, "Can not create ResourceRefProperty editor for empty object");
    SH_ASSERT(objectProperty, "Can not create ResourceRefProperty editor for empty property");
    SH_ASSERT(objectProperty->GetType() == sh::VAR_NAMED_RESOURCE_REF_LIST, "Can not create ResourceRefProperty editor. Property type does not match");
    
    sh::gui::VerticalLayoutPtr mainLayout(new sh::gui::VerticalLayout());
    sh::gui::LabelPtr label(new sh::gui::Label(objectProperty->GetName()));
    label->SetMaximumWidth(75U);
    label->SetMaximumHeight(20);
    mainLayout->AddWidget(label);
    SetLayout(mainLayout);
    
    auto value = objectProperty->GetValue(object);
    const auto& namedResourceRefList = value.GetNamedResourceRefList();
    
    for (auto& namedResourceRef : namedResourceRefList)
    {
        sh::gui::HorizontalLayoutPtr layout(new sh::gui::HorizontalLayout());
        sh::gui::LabelPtr name(new sh::gui::Label("   " + namedResourceRef.name));
        name->SetMaximumWidth(75U);
        name->SetMaximumHeight(20);
        layout->AddWidget(name);
        sh::gui::WidgetPtr widget(new sh::gui::Widget());
        widget->SetMaximumHeight(20U);widget->SetLayout(layout);
        mainLayout->AddWidget(widget);
        
        auto resourceGroup = sh::ResourceManager::GetInstance()->GetResourceGroup(namedResourceRef.resource.type);
        if (resourceGroup.empty())
            continue;
        sh::gui::ComboBoxPtr comboBox(new sh::gui::ComboBox());
        comboBox->AddItem("-None-");
        size_t index = 0;
        for (size_t i = 0; i < resourceGroup.size(); ++i)
        {
            comboBox->AddItem(resourceGroup[i].name);
            if (resourceGroup[i].name == namedResourceRef.resource.name)
                index = i + 1;
        }
        comboBox->SetSelectedItem(index);
        comboBox->OnItemChanged.Connect([this, namedResourceRef, namedResourceRefList](sh::u32 index)
        {
            auto resourceGroup = sh::ResourceManager::GetInstance()->GetResourceGroup(namedResourceRef.resource.type);
            if (index >= resourceGroup.size())
                return;
            sh::NamedResourceRef ref = namedResourceRef;
            if (index == 0U)
            {
                ref.resource.name = "";
            }
            else
            {
                ref.resource.name = resourceGroup[index - 1].name;
            }
            
            
            sh::NamedResourceRefList list = namedResourceRefList;
            
            bool found = false;
            for(auto& r : list)
            {
                if (r.name == ref.name)
                {
                    r.resource.name = ref.resource.name;
                    r.resource.type = ref.resource.type;
                    
                    found = true;
                    break;
                }
            }
            
            if (found)
            {
                this->m_objectProperty->SetValue(this->m_object, list);
            }
        });

        
        comboBox->SetMaximumHeight(20);
        layout->AddWidget(comboBox);
        
        
    }
}
