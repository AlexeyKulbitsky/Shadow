#ifndef SHADOW_MATERIAL_EDITOR_INCLUDE
#define SHADOW_MATERIAL_EDITOR_INCLUDE

#include <Shadow.h>

#include "../Vector2LineEdit.h"
#include "../Vector3LineEdit.h"
#include "../Vector4LineEdit.h"
#include "../colorpicker/ColorPicker.h"

class MaterialParamFloatEditor : public sh::gui::Widget
{
public:
	MaterialParamFloatEditor(sh::video::MaterialParam* param);
	void SetValue(float value);

private:
	sh::video::MaterialParam* m_param = nullptr;
};

/////////////////////////////////////////////////////////////////////////////////////////////

class MaterialParamVector2Editor : public sh::gui::Widget
{
public:
	MaterialParamVector2Editor(sh::video::MaterialParam* param);
	void SetValue(const sh::math::Vector2f& value);

private:
	sh::video::MaterialParam* m_param = nullptr;
};

/////////////////////////////////////////////////////////////////////////////////////////////

class MaterialParamVector3Editor : public sh::gui::Widget
{
public:
	MaterialParamVector3Editor(sh::video::MaterialParam* param);
	void SetValue(const sh::math::Vector3f& value);

private:
	sh::video::MaterialParam* m_param = nullptr;
};

/////////////////////////////////////////////////////////////////////////////////////////////

class MaterialParamVector4Editor : public sh::gui::Widget
{
public:
	MaterialParamVector4Editor(sh::video::MaterialParam* param);
	void SetValue(const sh::math::Vector4f& value);

private:
	sh::video::MaterialParam* m_param = nullptr;
};

/////////////////////////////////////////////////////////////////////////////////////////////

class MaterialParamColorEditor : public sh::gui::Widget
{
public:
	MaterialParamColorEditor(sh::video::MaterialParam* param);
	void SetValue(const sh::math::Vector4f& value);
	void OnColorPickerButtonReleased();

private:
	sh::video::MaterialParam* m_param = nullptr;
	sh::SPtr<ColorPicker> m_colorPicker;
	sh::gui::SpriteWidgetPtr m_colorWidget;
};

/////////////////////////////////////////////////////////////////////////////////////////////

class MaterialEditor : public sh::gui::Window
{
public:
	MaterialEditor();
	void SetMaterial(const sh::video::MaterialPtr& material);

private:
	void OnRenderTechniqueChanged(sh::u32 index);
	void ResetLayout();
	void MaterialChanged();

private:
	sh::gui::ComboBoxPtr m_comboBox;
	sh::gui::WidgetPtr m_rtWidget;
	sh::video::Material* m_material = nullptr;
	std::vector<std::weak_ptr<sh::io::FileInfo>> m_rtInfos;
};

#endif