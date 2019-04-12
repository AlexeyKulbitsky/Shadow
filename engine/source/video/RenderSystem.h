#ifndef SHADOW_RENDER_SYSTEM_INCLUDE
#define SHADOW_RENDER_SYSTEM_INCLUDE

#include "common/Prerequisities.h"
#include "math/math.h"

namespace sh
{

namespace video
{
    struct RenderPipelineDescription;

    class SHADOW_API RenderSystem
    {
    public:
        RenderSystem();
        ~RenderSystem();

        //virtual const std::string& GetApiName() const = 0;
        //virtual DriverType GetType() const = 0;

        virtual bool Init() = 0;
        virtual void BeginRendering() = 0;
        virtual void EndRendering() = 0;

        virtual void SetWindow(void* winId, uint32_t width, uint32_t height) {}
        virtual void SetWindow(void* winId) {}

        // Viewport management
        virtual void SetViewport(const math::Rect& viewport);
        virtual const math::Rect& GetViewport() const;
        virtual void SetDepthRange(float zMin, float zMax);
        virtual const math::Vector2& GetDepthRange() const;

        // Buffers clearing
        virtual void SetClearColor(const math::Vector4& clearColor);
        virtual void SetClearDepth(float clearDepth);
        virtual void SetClearStencil(float clearStencil);
        const math::Vector4& GetClearColorValue() const;
        float GetClearDepthValue() const;
        uint32_t GetClearStencilValue() const;

        //virtual void ClearColorBuffer() { }
        //virtual void ClearDepthBuffer() { }
        //virtual void ClearStencilBuffer() { }
        //virtual void ClearBuffers() { }

        // State management 
        //virtual void SetDepthStencilState(const DepthStencilStatePtr& depthStencilState) { }
        //virtual void SetRasterizationState(const RasterizationStatePtr& rasterizationState) { }
        //virtual void SetBlendingState(const BlendingStatePtr& blendingState) { }
        //virtual void SetRenderPipeline(const RenderPipelinePtr& pipeline, const CommandBufferPtr& commandBuffer = nullptr) { }
        //virtual void SetComputePipeline() { }


        // Rendering
        //virtual void SetGpuParams(const GpuParamsPtr& params, const CommandBufferPtr& commandBuffer = nullptr) { }
        //virtual void SetTopology(Topology topology, const CommandBufferPtr& commandBuffer = nullptr) { }
        //virtual void SetAutoUniformsBatch(const UniformsBatchPtr& batch, const CommandBufferPtr& commandBuffer = nullptr) { }
        //virtual void SetVertexDeclaration(const VertexInputDeclarationPtr& declaration, const CommandBufferPtr& commandBuffer = nullptr) { }
        //virtual void SetVertexBuffer(const VertexBufferPtr& buffer, const CommandBufferPtr& commandBuffer = nullptr) { }
        //virtual void SetIndexBuffer(const IndexBufferPtr& buffer, const CommandBufferPtr& commandBuffer = nullptr) { }
        //virtual void Draw(uint32_t offset, uint32_t verticesCount, uint32_t instancesCount = 1U, const CommandBufferPtr& commandBuffer = nullptr) { }
        //virtual void DrawIndexed(uint32_t offset, uint32_t indicesCount, uint32_t instancesCount = 1U, const CommandBufferPtr& commandBuffer = nullptr) { }
        //virtual void SetScissorRect(const math::Rect& scissor, const CommandBufferPtr& commandBuffer = nullptr) = 0;
        //virtual void SubmitCommandBuffer(const CommandBufferPtr& commandBuffer) { }

        //virtual void GetPixelData(uint32_t x, uint32_t y, uint8_t* data) {}

        uint32_t GetMaxTextureUnits() const { return m_maxTextureUnits; }
        uint32_t GetMaxTextureSize() const { return m_maxTextureSize; }
        uint32_t GetMaxCubeTextureSize() const { return m_maxCubeTextureSize; }

        // Resources creation interface
        //virtual UniformBufferPtr CreateUniformBuffer() const { return nullptr; }
        //virtual RenderTargetPtr CreateRenderTarget() const { return nullptr; }


    protected:
        math::Rect m_viewport;
        math::Vector2 m_depthRange;
        math::Vector2Int m_surfaceSize;

        // Clear values
        math::Vector4 m_clearColorValue;
        float m_clearDepthValue;
        uint32_t m_clearStencilValue;

        // Global info
        uint32_t m_maxTextureUnits;
        uint32_t m_maxTextureSize;
        uint32_t m_maxCubeTextureSize;

        //Painter* m_painter = nullptr;
    };

} // video

} // sh

#endif
