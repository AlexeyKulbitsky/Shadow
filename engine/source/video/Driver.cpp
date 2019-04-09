#include "Driver.h"
#include "Material.h"
#include "RenderPipeline.h"
#include "DepthStencilState.h"
#include "RasterizationState.h"
#include "BlendingState.h"

#include "../font/FontManager.h"

#include "../scene/Mesh.h"
#include "../scene/Model.h"

namespace sh
{
	namespace video
	{

		Driver::Driver()
		{
		}

		Driver::~Driver()
		{
		}

		/*void Driver::Render(scene::Model* model)
		{
			size_t meshesCount = model->GetMeshesCount();
			for (size_t i = 0; i < meshesCount; ++i)
			{
				sh::scene::MeshPtr mesh = model->GetMesh(i);
				Render(mesh.get());
			}
		}*/

		/////////////////////////////////////////////////////////////////////////

		/*void Driver::Render(scene::Mesh* mesh)
		{
			const MaterialPtr& material = mesh->GetMaterial();
			const RenderPipelinePtr& renderPipeline = material->GetRenderPipeline(0);

			SetDepthStencilState(renderPipeline->GetDepthStencilState());
			SetRasterizationState(renderPipeline->GetRasterizationState());
			SetBlendingState(renderPipeline->GetBlendingState());
		}*/

		/////////////////////////////////////////////////////////////////////////
		
		void Driver::SetViewport(u32 x, u32 y, u32 width, u32 height)
		{
			/*m_viewPort.x = x;
			m_viewPort.y = y;
			m_viewPort.z = width;
			m_viewPort.w = height;*/
            
            const int xFinal = math::Clamp(static_cast<int>(x), 0, m_surfaceSize.x);
            const int yFinal = math::Clamp(static_cast<int>(y), 0, m_surfaceSize.y);
            const int widthFinal = math::Clamp(static_cast<int>(width), 0, (m_surfaceSize.x - xFinal));
            const int heightFinal = math::Clamp(static_cast<int>(height), 0, (m_surfaceSize.y - yFinal));

			m_vp.upperLeftCorner.x = xFinal;
			m_vp.upperLeftCorner.y = yFinal;
			m_vp.lowerRightCorner.x = x + widthFinal;
			m_vp.lowerRightCorner.y = y + heightFinal;
		}

		/////////////////////////////////////////////////////////////////////////
        
        void Driver::SetViewport(const math::Rect& viewport)
        {
            m_vp = viewport;
        }
        
        /////////////////////////////////////////////////////////////////////////
	
		void Driver::SetDepthRange(f32 zMin, f32 zMax)
		{
			m_depthRange.x = zMin;
			m_depthRange.y = zMax;
		}

		/////////////////////////////////////////////////////////////////////////
	}
}
