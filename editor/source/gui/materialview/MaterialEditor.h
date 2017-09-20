#ifndef SHADOW_MATERIAL_EDITOR_INCLUDE
#define SHADOW_MATERIAL_EDITOR_INCLUDE

#include <Shadow.h>

#include "../Vector3LineEdit.h"

class MaterialParamVector3Editor : public sh::gui::Widget
{
public:
	MaterialParamVector3Editor(sh::video::MaterialParam* param);
	void SetValue(const sh::math::Vector3f& value);

private:
	sh::video::MaterialParam* m_param = nullptr;
};

class MaterialEditor : public sh::gui::Window
{
public:
	MaterialEditor();
	void SetMaterial(const sh::video::MaterialPtr& material);

private:
	sh::video::Material* m_material = nullptr;
};

#endif