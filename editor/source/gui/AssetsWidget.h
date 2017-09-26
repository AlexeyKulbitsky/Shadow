#ifndef SHADOW_EDITOR_ASSETS_WIDGET_INCLUDE
#define SHADOW_EDITOR_ASSETS_WIDGET_INCLUDE

#include "gui/materialview/MaterialEditor.h"

#include "TreeWidget.h"

class FolderTreeItem : public TreeItem
{
public:
	FolderTreeItem(TreeItem* parent, sh::io::FileSystemComponent* fsItem);
	virtual ~FolderTreeItem();

protected:
	virtual void OnContextMenu(sh::s32 x, sh::s32 y) override;

private:
	void OnMenuItemSelected(const sh::String& itemName);

private:
	sh::io::FileSystemComponent* m_item = nullptr;
};

////////////////////////////////////////////////////////////////////////////////////////////

class FileTreeItem : public TreeItem
{
public:
	FileTreeItem(TreeItem* parent, sh::io::FileSystemComponent* fsItem);
	virtual ~FileTreeItem();

private:
	sh::io::FileSystemComponent* m_item = nullptr;
};

////////////////////////////////////////////////////////////////////////////////////////////

class MaterialTreeItem : public TreeItem
{
public:
	MaterialTreeItem(TreeItem* parent, sh::io::FileSystemComponent* fsItem);
	virtual ~MaterialTreeItem();
	virtual void OnToggled(bool toggled) override;

private:
	sh::io::FileSystemComponent* m_item = nullptr;
};

////////////////////////////////////////////////////////////////////////////////////////////

class AssetsTreeWidget : public TreeWidget
{
public:
	sh::Event<void, const sh::video::MaterialPtr&> materialChanged;
};

class AssetsWidget : public sh::gui::Window
{
public:
	AssetsWidget();
	~AssetsWidget();

private:
	void OnMaterialTreeItemChanged(const sh::video::MaterialPtr& material);

private:
	sh::gui::WindowPtr m_window;

	sh::SPtr<MaterialEditor> m_materialEditor;
};

#endif