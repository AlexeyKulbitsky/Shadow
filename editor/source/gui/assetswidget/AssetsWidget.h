#ifndef SHADOW_EDITOR_ASSETS_WIDGET_INCLUDE
#define SHADOW_EDITOR_ASSETS_WIDGET_INCLUDE

#include "../materialview/MaterialEditor.h"


class FolderTreeItem : public sh::gui::TreeItem
{
public:
	FolderTreeItem(sh::gui::TreeItem* parent, sh::io::FileSystemComponent* fsItem);
	virtual ~FolderTreeItem();

protected:
	void OnExpanded(bool expanded);
	virtual void OnContextMenu(sh::s32 x, sh::s32 y) override;

private:
	void OnMenuItemSelected(const sh::String& itemName);

private:
	sh::io::FileSystemComponent* m_item = nullptr;
};

////////////////////////////////////////////////////////////////////////////////////////////

class FileTreeItem : public sh::gui::TreeItem
{
public:
	FileTreeItem(sh::gui::TreeItem* parent, sh::io::FileSystemComponent* fsItem);
	virtual ~FileTreeItem();

private:
	sh::io::FileSystemComponent* m_item = nullptr;
};

////////////////////////////////////////////////////////////////////////////////////////////

class MaterialTreeItem : public sh::gui::TreeItem
{
public:
	MaterialTreeItem(sh::gui::TreeItem* parent, sh::io::FileSystemComponent* fsItem);
	virtual ~MaterialTreeItem();
	virtual void OnToggled(bool toggled) override;

protected:
	virtual void OnContextMenu(sh::s32 x, sh::s32 y) override;
	virtual void OnEdit(sh::s32 x, sh::s32 y) override;

private:
	void OnMenuItemSelected(const sh::String& itemName);

private:
	sh::io::FileSystemComponent* m_item = nullptr;
};

////////////////////////////////////////////////////////////////////////////////////////////

class AssetsTreeWidget : public sh::gui::TreeWidget
{
public:
	sh::Event<void, const sh::video::MaterialPtr&> materialChanged;
};

class AssetsWidget : public sh::gui::Window
{
public:
	AssetsWidget();
	~AssetsWidget();

	virtual void UpdateLayout() override;

private:
	void OnMaterialTreeItemChanged(const sh::video::MaterialPtr& material);

private:
	sh::gui::WindowPtr m_window;

	sh::SPtr<MaterialEditor> m_materialEditor;
};

#endif
