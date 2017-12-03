#ifndef SHADOW_GUI_BATCH_INCLUDE
#define SHADOW_GUI_BATCH_INCLUDE

#include "../Globals.h"
#include "../video/Painter.h"

namespace sh
{

namespace gui
{

	enum GuiSubLayerType
	{
		GUI_LAYER_BACKGROUND = 0,
		GUI_LAYER_TEXT,

		GUI_LAYER_COUNT
	};

	// Batch inside one sublayer
	struct GuiBatch
	{
		sh::video::Material* material = nullptr;
		math::Rect clipRect;
		u32 startIndex = 0U;
		u32 indicesCount = 0U;

		
	};

	// Batches pack for one sub layer (background, text, etc.)
	struct GuiSubLayerBatch
	{
		std::vector<GuiBatch> batches;
		std::vector<float> vertexArray;
		std::vector<u32> indexArray;

		void AddRect(const math::Vector2Int& ulPos, const math::Vector2& ulUV, const math::Vector4& ulColor,
			const math::Vector2Int& brPos, const math::Vector2& brUV, const math::Vector4& brColor)
		{
			const auto lastIndex = indexArray.size();
			vertexArray.push_back(static_cast<float>(ulPos.x)); vertexArray.push_back(static_cast<float>(brPos.y));
			vertexArray.push_back(static_cast<float>(ulUV.x)); vertexArray.push_back(static_cast<float>(brUV.y));
			vertexArray.push_back(static_cast<float>(ulColor.x)); vertexArray.push_back(static_cast<float>(ulColor.y));
			vertexArray.push_back(static_cast<float>(ulColor.z)); vertexArray.push_back(static_cast<float>(ulColor.w));

			vertexArray.push_back(static_cast<float>(ulPos.x)); vertexArray.push_back(static_cast<float>(ulPos.y));
			vertexArray.push_back(static_cast<float>(ulUV.x)); vertexArray.push_back(static_cast<float>(ulUV.y));
			vertexArray.push_back(static_cast<float>(ulColor.x)); vertexArray.push_back(static_cast<float>(ulColor.y));
			vertexArray.push_back(static_cast<float>(ulColor.z)); vertexArray.push_back(static_cast<float>(ulColor.w));

			vertexArray.push_back(static_cast<float>(brPos.x)); vertexArray.push_back(static_cast<float>(ulPos.y));
			vertexArray.push_back(static_cast<float>(brUV.x)); vertexArray.push_back(static_cast<float>(ulUV.y));
			vertexArray.push_back(static_cast<float>(ulColor.x)); vertexArray.push_back(static_cast<float>(ulColor.y));
			vertexArray.push_back(static_cast<float>(ulColor.z)); vertexArray.push_back(static_cast<float>(ulColor.w));

			vertexArray.push_back(static_cast<float>(brPos.x)); vertexArray.push_back(static_cast<float>(brPos.y));
			vertexArray.push_back(static_cast<float>(brUV.x)); vertexArray.push_back(static_cast<float>(brUV.y));
			vertexArray.push_back(static_cast<float>(ulColor.x)); vertexArray.push_back(static_cast<float>(ulColor.y));
			vertexArray.push_back(static_cast<float>(ulColor.z)); vertexArray.push_back(static_cast<float>(ulColor.w));

			indexArray.push_back(lastIndex);
			indexArray.push_back(lastIndex + 2);
			indexArray.push_back(lastIndex + 1);

			indexArray.push_back(lastIndex);
			indexArray.push_back(lastIndex + 3);
			indexArray.push_back(lastIndex + 2);
		}
	};

	// Batches for one rendering layer
	struct GuiLayerBatch
	{
		std::array<GuiSubLayerBatch, GUI_LAYER_COUNT> batches;
	};

} // gui

} // sh

#endif
