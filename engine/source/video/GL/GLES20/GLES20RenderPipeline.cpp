#include "video/GL/GLES20/GLES20RenderPipeline.h"
#include "video/GL/GLES20/Managers/GLES20RenderStateManager.h"
#include "video/GL/GLES20/GLES20VertexInputDeclaration.h"
#include "video/GL/GLShader.h"
#include "video/GpuPipelineParamsInfo.h"

namespace sh
{

namespace video
{

    GLES20RenderPipeline::GLES20RenderPipeline()
    {
    }

    GLES20RenderPipeline::~GLES20RenderPipeline()
    {
    }

    const VertexInputDeclarationPtr& GLES20RenderPipeline::GetVertexInputDeclaration() const
    {
        return m_vertexDeclaration;
    }

    GLuint GLES20RenderPipeline::GetProgramID() const
    {
        return m_programID;
    }

    void GLES20RenderPipeline::ParseAttributes()
    {
        const size_t bufferSize = 256U;
        char buffer[bufferSize];

        std::shared_ptr<GLES20VertexInputDeclaration> vertexDeclaration(new GLES20VertexInputDeclaration());

        GLint attributesCount = 0;
        glGetProgramiv(m_programID, GL_ACTIVE_ATTRIBUTES, &attributesCount);
        for (GLuint i = 0; i < static_cast<GLuint>(attributesCount); ++i)
        {
            GLsizei readedBytes = 0;
            GLint attributeSize = 0;
            GLenum attributeType;
            glGetActiveAttrib(m_programID, i, bufferSize, &readedBytes, &attributeSize, &attributeType, buffer);

            GLES20VertexInputAttribute attribute;
            attribute.index = glGetAttribLocation(m_programID, buffer);
            attribute.name = buffer;
            attribute.type = GL_FLOAT;
            attribute.semantic = AttributeSemantic::UNDEFINED;

            switch (attributeType)
            {
            case GL_FLOAT:
                attribute.size = 1;
                break;
            case GL_FLOAT_VEC2:
                attribute.size = 2;
                break;
            case GL_FLOAT_VEC3:
                attribute.size = 3;
                break;
            case GL_FLOAT_VEC4:
                attribute.size = 4;
                break;
            case GL_FLOAT_MAT2:
                attribute.size = 4;
                break;
            case GL_FLOAT_MAT3:
                attribute.size = 9;
                break;
            case GL_FLOAT_MAT4:
                attribute.size = 16;
                break;
            default:
                break;
            }

            auto it = attributeSemanticMap.find(buffer);
            if (it != attributeSemanticMap.end())
            {
                attribute.semantic = it->second;
            }

            vertexDeclaration->AddAttribute(attribute);
        }
        m_vertexDeclaration = vertexDeclaration;
    }

    void GLES20RenderPipeline::ParseUniforms()
    {
        std::shared_ptr<GpuParamsDescription> paramsDescription(new GpuParamsDescription());

        const size_t bufferSize = 256U;
        char buffer[bufferSize];

        GLint uniformsCount = 0;
        glGetProgramiv(m_programID, GL_ACTIVE_UNIFORMS, &uniformsCount);
        for (GLuint i = 0; i < static_cast<GLuint>(uniformsCount); ++i)
        {
            GLsizei readedBytes = 0;
            GLint uniformSize = 0;
            GLenum uniformType;
            glGetActiveUniform(m_programID, i, bufferSize, &readedBytes, &uniformSize, &uniformType, buffer);
            GLint location = glGetUniformLocation(m_programID, buffer);

            switch (uniformType)
            {
            case GL_FLOAT:
            {
                GpuParamDataDescription desc;
                desc.name = buffer;
                desc.type = GPDT_FLOAT1;
                desc.size = sizeof(float);
                desc.location = location;

                paramsDescription->params[desc.name] = desc;
            }

            break;
            case GL_FLOAT_VEC2:
            {
                GpuParamDataDescription desc;
                desc.name = buffer;
                desc.type = GPDT_FLOAT2;
                desc.size = sizeof(float) * 2;
                desc.location = location;

                paramsDescription->params[desc.name] = desc;
            }
            break;
            case GL_FLOAT_VEC3:
            {
                GpuParamDataDescription desc;
                desc.name = buffer;
                desc.type = GPDT_FLOAT3;
                desc.size = sizeof(float) * 3;
                desc.location = location;

                paramsDescription->params[desc.name] = desc;
            }
            break;
            case GL_FLOAT_VEC4:
            {
                GpuParamDataDescription desc;
                desc.name = buffer;
                desc.type = GPDT_FLOAT4;
                desc.size = sizeof(float) * 4;
                desc.location = location;

                paramsDescription->params[desc.name] = desc;
            }
            break;
            case GL_INT:
            {
                GpuParamDataDescription desc;
                desc.name = buffer;
                desc.type = GPDT_INT1;
                desc.size = sizeof(int32_t);
                desc.location = location;

                paramsDescription->params[desc.name] = desc;
            }
            break;
            case GL_INT_VEC2:
            {
                GpuParamDataDescription desc;
                desc.name = buffer;
                desc.type = GPDT_INT2;
                desc.size = sizeof(int32_t) * 2;
                desc.location = location;

                paramsDescription->params[desc.name] = desc;
            }
            break;
            case GL_INT_VEC3:
            {
                GpuParamDataDescription desc;
                desc.name = buffer;
                desc.type = GPDT_INT3;
                desc.size = sizeof(int32_t) * 3;
                desc.location = location;

                paramsDescription->params[desc.name] = desc;
            }
            break;
            case GL_INT_VEC4:
            {
                GpuParamDataDescription desc;
                desc.name = buffer;
                desc.type = GPDT_INT4;
                desc.size = sizeof(int32_t) * 4;
                desc.location = location;

                paramsDescription->params[desc.name] = desc;
            }
            break;
            case GL_BOOL:
            {
                GpuParamDataDescription desc;
                desc.name = buffer;
                desc.type = GPDT_BOOL1;
                desc.size = sizeof(bool);
                desc.location = location;

                paramsDescription->params[desc.name] = desc;
            }
            break;
            case GL_BOOL_VEC2:
            {
                GpuParamDataDescription desc;
                desc.name = buffer;
                desc.type = GPDT_BOOL2;
                desc.size = sizeof(bool) * 2;
                desc.location = location;

                paramsDescription->params[desc.name] = desc;
            }
            break;
            case GL_BOOL_VEC3:
            {
                GpuParamDataDescription desc;
                desc.name = buffer;
                desc.type = GPDT_BOOL3;
                desc.size = sizeof(bool) * 3;
                desc.location = location;

                paramsDescription->params[desc.name] = desc;
            }
            break;
            case GL_BOOL_VEC4:
            {
                GpuParamDataDescription desc;
                desc.name = buffer;
                desc.type = GPDT_BOOL4;
                desc.size = sizeof(bool) * 4;
                desc.location = location;

                paramsDescription->params[desc.name] = desc;
            }
            break;
            case GL_FLOAT_MAT2:
            {
                GpuParamDataDescription desc;
                desc.name = buffer;
                desc.type = GPDT_MATRIX2;
                desc.size = sizeof(float) * 4;
                desc.location = location;

                paramsDescription->params[desc.name] = desc;
            }
            break;
            case GL_FLOAT_MAT3:
            {
                GpuParamDataDescription desc;
                desc.name = buffer;
                desc.type = GPDT_MATRIX3;
                desc.size = sizeof(float) * 9;
                desc.location = location;

                paramsDescription->params[desc.name] = desc;
            }
            break;
            case GL_FLOAT_MAT4:
            {
                GpuParamDataDescription desc;
                desc.name = buffer;
                desc.type = GPDT_MATRIX4;
                desc.size = sizeof(float) * 16;
                desc.location = location;

                paramsDescription->params[desc.name] = desc;
            }
            break;
            case GL_SAMPLER_2D:
            {
                GpuParamObjectDescription desc;
                desc.name = buffer;
                desc.type = GPOT_SAMPLER_2D;
                desc.binding = location;

                paramsDescription->samplers[desc.name] = desc;
            }
            break;
            case GL_SAMPLER_CUBE:
            {
                GpuParamObjectDescription desc;
                desc.name = buffer;
                desc.type = GPOT_SAMPLER_CUBE;
                desc.binding = location;

                paramsDescription->samplers[desc.name] = desc;
            }
            break;
            default:
                break;
            }
        }

        GpuPipelineParamsDescription pipelineParamsDescription;

        pipelineParamsDescription.vertexParams = paramsDescription;

        m_paramsInfo = GpuPipelineParamsInfo::Create(pipelineParamsDescription);
    }

    GLES20RenderPipeline::GLES20RenderPipeline(const RenderPipelineDescription& description)
    {
        m_description = description;

        // Create shader program
        m_programID = glCreateProgram();

        GLShader* vertShader = static_cast<GLShader*>(m_description.vertexShader.get());
        GLShader* fragShader = static_cast<GLShader*>(m_description.fragmentShader.get());
        glAttachShader(m_programID, vertShader->GetGLId());
        glAttachShader(m_programID, fragShader->GetGLId());

        glLinkProgram(m_programID);

        GLint linked;
        glGetProgramiv(m_programID, GL_LINK_STATUS, &linked);
        if (!linked)
        {
            GLsizei len;
            glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &len);

            char* log = new char[len + 1];
            glGetProgramInfoLog(m_programID, len, &len, log);

            assert(0 && "Error while linking GLES20 shader program!");
        }

        ParseAttributes();
        ParseUniforms();
    }

} // video

} // sh