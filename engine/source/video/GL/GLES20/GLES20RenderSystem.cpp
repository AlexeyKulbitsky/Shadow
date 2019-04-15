#include "video/GL/GLES20/GLES20RenderSystem.h"
#include "video/GL/GLContext/GLContextManager.h"
#include "device/Device.h"
#include "video/GL/GLDebug.h"

#if defined SHADOW_WINDOWS
#include "video/GL/GLContext/WGLContextManager.h"
#elif defined SHADOW_ANDROID
#endif

namespace sh
{

namespace video
{

    GLES20RenderSystem::GLES20RenderSystem()
    {
#if defined SHADOW_WINDOWS
        WGLContextManager *contextManager = new WGLContextManager();
        void* winId = Device::GetInstance()->GetWinId();
        const bool res = contextManager->CreateContext(winId);
        assert(res && "Can not create Windows GLES20 context");
        m_contextManager = contextManager;

#elif defined SHADOW_ANDROID

#endif
    }

    GLES20RenderSystem::~GLES20RenderSystem()
    {
    }

    bool GLES20RenderSystem::Init()
    {
        //sh::Device::GetInstance()->windowResizeEvent.Connect(
        //    std::bind(&GLES20Driver::OnWindowResized, this, std::placeholders::_1, std::placeholders::_2));

        //sh::Device::GetInstance()->sursafeChangedEvent.Connect(
        //    std::bind(&GLES20Driver::SetWindow, this, std::placeholders::_1, std::placeholders::_2,
        //        std::placeholders::_3));

        GL_CALL(glEnable(GL_DEPTH_TEST));
        //GL_CALL(glEnable(GL_SCISSOR_TEST));
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        GL_CALL(glClearColor(0.7f, 0.7f, 0.7f, 1.0f));

        return true;
    }

    ////////////////////////////////////////////////////////////////////////

    void GLES20RenderSystem::SetWindow(void* winId, uint32_t width, uint32_t height)
    {
        if (m_contextManager)
            m_contextManager->CreateContext(winId);
        //OnWindowResized(width, height);
    }

    ////////////////////////////////////////////////////////////////////////

#if 0
    void GLES20RenderSystem::SetDepthStencilState(const DepthStencilStatePtr& depthStencilState)
    {
        if (depthStencilState->enableDepthTest)
        {
            GL_CALL(glEnable(GL_DEPTH_TEST));
            //glDepthFunc(s_glCompareFunction[static_cast<size_t>(depthStencilState->depthCompareFunction)]);
        }
        else
        {
            GL_CALL(glDisable(GL_DEPTH_TEST));
        }
        //glDepthMask(s_glWriteMask[static_cast<size_t>(depthStencilState->depthWriteMask)]);

        // Stencil part
        if (depthStencilState->enableStencilTest)
        {
            GL_CALL(glEnable(GL_STENCIL_TEST));
            GL_CALL(glStencilFuncSeparate(GL_FRONT,
                s_glCompareFunction[static_cast<size_t>(depthStencilState->frontFace.compare)],
                depthStencilState->reference,
                depthStencilState->stencilReadMask));
            GL_CALL(glStencilMaskSeparate(GL_FRONT, depthStencilState->stencilWriteMask));
            GL_CALL(glStencilOpSeparate(GL_FRONT,
                s_glStencilOperation[static_cast<size_t>(depthStencilState->frontFace.fail)],
                s_glStencilOperation[static_cast<size_t>(depthStencilState->frontFace.depthFail)],
                s_glStencilOperation[static_cast<size_t>(depthStencilState->frontFace.pass)]));

            GL_CALL(glStencilFuncSeparate(GL_BACK,
                s_glCompareFunction[static_cast<size_t>(depthStencilState->backFace.compare)],
                depthStencilState->reference,
                depthStencilState->stencilReadMask));
            GL_CALL(glStencilMaskSeparate(GL_BACK, depthStencilState->stencilWriteMask));
            GL_CALL(glStencilOpSeparate(GL_BACK,
                s_glStencilOperation[static_cast<size_t>(depthStencilState->backFace.fail)],
                s_glStencilOperation[static_cast<size_t>(depthStencilState->backFace.depthFail)],
                s_glStencilOperation[static_cast<size_t>(depthStencilState->backFace.pass)]));
        }
        else
        {
            GL_CALL(glDisable(GL_STENCIL_TEST));
        }
    }

    ////////////////////////////////////////////////////////////////////////

    void GLES20RenderSystem::SetRasterizationState(const RasterizationStatePtr& rasterizationState)
    {
        if (rasterizationState->cullFace != CullFace::CF_NONE)
        {
            GL_CALL(glEnable(GL_CULL_FACE));
            GL_CALL(glCullFace(s_glCullFace[rasterizationState->cullFace]));
            GL_CALL(glFrontFace(s_glFrontFace[rasterizationState->frontFace]));
        }
        else
        {
            GL_CALL(glDisable(GL_CULL_FACE));
        }
    }

    ////////////////////////////////////////////////////////////////////////

    void GLES20RenderSystem::SetBlendingState(const BlendingStatePtr& blendingState)
    {
        if (blendingState->enabled)
        {
            GL_CALL(glEnable(GL_BLEND));
            GL_CALL(glBlendFunc(s_glBlendFactor[static_cast<size_t>(blendingState->srcAlpha)],
                s_glBlendFactor[static_cast<size_t>(blendingState->dstAlpha)]));
            GL_CALL(glBlendEquation(s_glBlendOperation[static_cast<size_t>(blendingState->operationAlpha)]));

            /*
            glBlendFuncSeparate(s_glBlendFactor[static_cast<size_t>(blendingState->srcColor)],
            s_glBlendFactor[static_cast<size_t>(blendingState->dstColor)],
            s_glBlendFactor[static_cast<size_t>(blendingState->srcAlpha)],
            s_glBlendFactor[static_cast<size_t>(blendingState->dstAlpha)]);

            glBlendEquationSeparate(s_glBlendOperation[static_cast<size_t>(blendingState->operationColor)],
            s_glBlendOperation[static_cast<size_t>(blendingState->operationAlpha)]);

            glBlendColor(float, float, float, float);
            */
        }
        else
        {
            GL_CALL(glDisable(GL_BLEND));
        }
    }

    ////////////////////////////////////////////////////////////////////////

    void GLES20RenderSystem::SetRenderPipeline(const RenderPipelinePtr& pipeline, const CommandBufferPtr&)
    {
        GLES20RenderPipeline* glPipeline = static_cast<GLES20RenderPipeline*>(pipeline.get());
        SetBlendingState(glPipeline->GetBlendingState());
        SetRasterizationState(glPipeline->GetRasterizationState());
        SetDepthStencilState(glPipeline->GetDepthStencilState());
        GL_CALL(glUseProgram(glPipeline->GetProgramID()));
    }

    ////////////////////////////////////////////////////////////////////////

    void GLES20RenderSystem::SetComputePipeline()
    {

    }

    ////////////////////////////////////////////////////////////////////////

    void GLES20RenderSystem::SetGpuParams(const GpuParamsPtr& params, const CommandBufferPtr&)
    {
        const u8* data = params->GetData();
        const auto& samplers = params->GetSamplers();
        const auto& paramsInfo = params->GetParamsInfo();
        int samplerCounter = 0;

        for (size_t i = 0; i < 2U; ++i)
        {
            ShaderType shaderType = static_cast<ShaderType>(i);

            const SPtr<GpuParamsDescription>& desc = paramsInfo->GetParamsDescription(shaderType);
            if (!desc)
                continue;

            // Upload single params
            for (const auto& param : desc->params)
            {
                const GLfloat* dataPtr = reinterpret_cast<const float*>(data + param.second.offset);

                switch (param.second.type)
                {
                case GPDT_FLOAT1:
                {
                    GL_CALL(glUniform1fv(param.second.location, 1, dataPtr));
                }
                break;
                case GPDT_FLOAT2:
                {
                    GL_CALL(glUniform2fv(param.second.location, 1, dataPtr));
                }
                break;
                case GPDT_FLOAT3:
                {
                    GL_CALL(glUniform3fv(param.second.location, 1, dataPtr));
                }
                break;
                case GPDT_FLOAT4:
                {
                    GL_CALL(glUniform4fv(param.second.location, 1, dataPtr));
                }
                break;
                case GPDT_MATRIX3:
                {
                    GL_CALL(glUniformMatrix3fv(param.second.location, 1, GL_FALSE, dataPtr));
                }
                break;
                case GPDT_MATRIX4:
                {
                    GL_CALL(glUniformMatrix4fv(param.second.location, 1, GL_FALSE, dataPtr));
                }
                break;
                }
            }

            // Upload samplers

            for (const auto& samplerDesc : desc->samplers)
            {
                const u32 index = paramsInfo->GetIndex(samplerDesc.second.set, samplerDesc.second.binding);
                const auto& sampler = samplers[index];

                GLTexture* texture = static_cast<GLTexture*>(sampler->GetTexture().get());
                GL_CALL(glActiveTexture(GL_TEXTURE0 + samplerCounter));
                GLenum textureTraget = s_glTextureType[texture->GetDescription().type];
                glBindTexture(textureTraget, texture->GetGLId());

                const auto& description = sampler->GetDescription();
                // Set minification filter
                {
                    switch (description.mipFilter)
                    {
                    case TEX_FILT_NEAREST:
                    {
                        switch (description.minFilter)
                        {
                        case TEX_FILT_NONE:
                        case TEX_FILT_NEAREST:
                            GL_CALL(glTexParameteri(textureTraget, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
                            break;
                        case TEX_FILT_LINEAR:
                            GL_CALL(glTexParameteri(textureTraget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
                            break;
                        default:
                            break;
                        }
                    }
                    break;
                    case TEX_FILT_LINEAR:
                    {
                        switch (description.minFilter)
                        {
                        case TEX_FILT_NONE:
                        case TEX_FILT_NEAREST:
                            GL_CALL(glTexParameteri(textureTraget, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));
                            break;
                        case TEX_FILT_LINEAR:
                            GL_CALL(glTexParameteri(textureTraget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
                            break;
                        default:
                            break;
                        }
                    }
                    break;
                    case TEX_FILT_NONE:
                    {
                        GL_CALL(glTexParameteri(textureTraget, GL_TEXTURE_MIN_FILTER, s_glTextureFiltering[description.minFilter]));
                    }
                    break;
                    default:
                        break;
                    }
                }
                // Set magnification filter
                {
                    GL_CALL(glTexParameteri(textureTraget, GL_TEXTURE_MAG_FILTER, s_glTextureFiltering[description.magFilter]));
                }
                GL_CALL(glTexParameteri(textureTraget, GL_TEXTURE_WRAP_S, s_glTextureTiling[description.tilingU]));
                GL_CALL(glTexParameteri(textureTraget, GL_TEXTURE_WRAP_T, s_glTextureTiling[description.tilingV]));
                if (texture->GetDescription().type == TEX_TYPE_TEXTURE_CUBE)
                    GL_CALL(glTexParameteri(textureTraget, GL_TEXTURE_WRAP_R, s_glTextureTiling[description.tilingW]));

                GL_CALL(glUniform1i(samplerDesc.second.binding, samplerCounter));
                samplerCounter++;
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////

    void GLES20RenderSystem::SetTopology(Topology topology, const CommandBufferPtr&)
    {
        m_currentTopology = s_glTopology[static_cast<size_t>(topology)];
    }

    ////////////////////////////////////////////////////////////////////////

    void GLES20RenderSystem::SetVertexDeclaration(const VertexInputDeclarationPtr& declaration, const CommandBufferPtr&)
    {
        GLES20VertexDeclaration* vertexDeclaration = static_cast<GLES20VertexDeclaration*>(declaration.get());
        for (auto attribute : vertexDeclaration->GetAttributes())
        {
            GL_CALL(glEnableVertexAttribArray(attribute.index));
            GL_CALL(glVertexAttribPointer(attribute.index, attribute.size, attribute.type, false, vertexDeclaration->GetStride(), attribute.pointer));
        }
    }

    ////////////////////////////////////////////////////////////////////////

    void GLES20RenderSystem::SetVertexBuffer(const VertexBufferPtr& buffer, const CommandBufferPtr&)
    {
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLVertexBuffer* glBuffer = static_cast<GLVertexBuffer*>(buffer.get());
        m_currentVertexBuffer = glBuffer->GetGLId();
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_currentVertexBuffer));
    }

    ////////////////////////////////////////////////////////////////////////

    void GLES20RenderSystem::SetIndexBuffer(const IndexBufferPtr& buffer, const CommandBufferPtr&)
    {
        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        GLIndexBuffer* glBuffer = static_cast<GLIndexBuffer*>(buffer.get());
        m_currentIndexBuffer = glBuffer->GetGLId();
        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_currentIndexBuffer));
    }

    ////////////////////////////////////////////////////////////////////////

    void GLES20RenderSystem::Draw(u32 offset, u32 verticesCount, u32 instancesCount, const CommandBufferPtr&)
    {
        GL_CALL(glDrawArrays(m_currentTopology, offset, verticesCount));
    }

    ////////////////////////////////////////////////////////////////////////

    void GLES20RenderSystem::DrawIndexed(u32 offset, u32 indicesCount, u32 instancesCount, const CommandBufferPtr&)
    {
        GL_CALL(glDrawElements(m_currentTopology, indicesCount, GL_UNSIGNED_INT, (void*)(offset * sizeof(u32))));
    }

    ////////////////////////////////////////////////////////////////////////

    void GLES20RenderSystem::SetScissorRect(const math::Rect& scissor, const CommandBufferPtr&)
    {
        const s32 height = static_cast<s32>(m_viewPort.w);
        /*GL_CALL(glScissor(static_cast<s32>(scissor.upperLeftCorner.x),
                  height - static_cast<s32>(scissor.upperLeftCorner.y + scissor.GetHeight()),
                  scissor.GetWidth(),
                  scissor.GetHeight()));*/

        GL_CALL(glScissor(0, 0, scissor.GetWidth(), scissor.GetHeight()));
    }

    ////////////////////////////////////////////////////////////////////////

    void GLES20RenderSystem::GetPixelData(u32 x, u32 y, u8* data)
    {
        GLint viewport[4];
        GL_CALL(glGetIntegerv(GL_VIEWPORT, viewport));
        GL_CALL(glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data));
    }

    ////////////////////////////////////////////////////////////////////////

    RenderTargetPtr GLES20RenderSystem::CreateRenderTarget() const
    {
        RenderTargetPtr result = nullptr;
        result.reset(new GLRenderTarget());
        return result;
    }

    ////////////////////////////////////////////////////////////////////////

    void GLES20RenderSystem::OnWindowResized(int width, int height)
    {
        SetViewport(0, 0, width, height);
        m_contextManager->Update();
        m_surfaceSize.x = width;
        m_surfaceSize.y = height;
    }
#endif

} // video

} // sh



