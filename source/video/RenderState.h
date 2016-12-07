#ifndef SHADOW_RENDER_STATE_INCLUDE
#define SHADOW_RENDER_STATE_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		enum class FrontFace
		{
			CLOCKWISE,
			COUNTER_CLOCKWISE
		};

		enum class CullFace
		{
			FRONT,
			BACK,
			FRONT_AND_BACK
		};

		enum class PolygonMode
		{
			FILL,
			POINT,
			LINE
		};

		class RenderState
		{
		public:
			RenderState();
			virtual ~RenderState();
			virtual void Init();
			virtual void Load(const pugi::xml_node &node);
			virtual void Apply(){}
			virtual RenderState* Clone() { return nullptr; }

			virtual void SetFrontFace(FrontFace frontFace) { m_frontFace = frontFace; }
			virtual void SetCullFace(CullFace cullFace) { m_cullFace = cullFace; }
			virtual void SetPolygonMode(PolygonMode polygonMode) { m_polygoneMode = polygonMode; }
			virtual void SetPointSize(float pointSize) { m_pointSize = pointSize; }
			virtual void SetLineWidth(float lineWidth) { m_lineWidth = lineWidth; }
			virtual void EnableDepthTest(bool enable) { m_enableDepthTest = enable; }
			virtual void EnableScissorTest(bool enable) { m_enableScissorTest = enable; }
			virtual void EnableBlending(bool enable) { m_enableBlending = enable; }
			virtual void EnableCullFace(bool enable) { m_enableCullFace = enable; }
			virtual void EnableStencilTest(bool enable) { m_enableStencilTest = enable; }

		protected:
			FrontFace m_frontFace;
			CullFace m_cullFace;
			PolygonMode m_polygoneMode;
			float m_pointSize;
			float m_lineWidth;
			bool m_enableDepthTest;
			bool m_enableScissorTest;
			bool m_enableBlending;
			bool m_enableCullFace;
			bool m_enableStencilTest;
		};
	}
}

#endif