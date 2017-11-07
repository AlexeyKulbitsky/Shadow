#ifndef SHADOW_EDITOR_ASSETS_WIDGET_INCLUDE
#define SHADOW_EDITOR_ASSETS_WIDGET_INCLUDE

#include "../materialview/MaterialEditor.h"
#include "../textureview/TextureWidget.h"

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

class AssetsTreeWidget : public sh::gui::TreeWidget
{
public:
	sh::Event<void, const sh::video::MaterialPtr&> materialChanged;
	sh::Event<void, const sh::video::TexturePtr&> textureChanged;
};

class AssetsWidget : public sh::gui::Window
{
public:
	AssetsWidget();
	~AssetsWidget();

	virtual void UpdateLayout() override;

	void RefreshAssetsList();

private:
	void OnMaterialTreeItemChanged(const sh::video::MaterialPtr& material);
	void OnTextureTreeItemChanged(const sh::video::TexturePtr& texture);

private:
	sh::gui::WindowPtr m_window;

	sh::SPtr<AssetsTreeWidget> m_treeWidget;
	sh::SPtr<MaterialEditor> m_materialEditor;
	sh::SPtr<TextureWidget> m_textureWidget;
};

#endif
