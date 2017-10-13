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
		u32 startIndex = 0U;
		u32 indicesCount = 0U;
		u32 verticesCount = 0U;
	};

	// Batches pack for one sub layer (background, text, etc.)
	struct GuiSubLayerBatch
	{
		Map<video::Material*, GuiBatch> batches;
		std::vector<video::Painter::Vertex> vertexArray;
		std::vector<u32> indexArray;
	};

	// Batches for one rendering layer
	struct GuiLayerBatch
	{
		std::array<GuiSubLayerBatch, GUI_LAYER_COUNT> batches;
	};

} // gui

} // sh

#endif
