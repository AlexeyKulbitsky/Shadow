#ifndef SHADOW_EDITOR_HIERARCHY_WIDGET_INCLUDE
#define SHADOW_EDITOR_HIERARCHY_WIDGET_INCLUDE

#include <Shadow.h>

class HierarchyWidget
{
public:
	HierarchyWidget();
	~HierarchyWidget();

	void AddEntity(sh::Entity* entity);
	void SetSelectedEntity(sh::Entity* entity);

	sh::Event<void, sh::Entity*> OnEntitySelected;

private:
	void OnWindowResized(sh::s32 width, sh::s32 height);
	void OnItemToggled(bool toggled, const sh::gui::ButtonPtr& sender);
	void UpdateGeometry(sh::u32 screenWidth, sh::u32 screenHeight);
	void SelectEntity(sh::u32 index);

private:
	sh::gui::WindowPtr m_window;
	sh::gui::VerticalLayoutPtr m_layout;
	sh::gui::ButtonPtr m_currentSelectedItem;
};

#endif
