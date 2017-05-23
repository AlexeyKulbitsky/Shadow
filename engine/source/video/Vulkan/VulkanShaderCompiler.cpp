#include "VulkanShaderCompiler.h"

//#include <ResourceLimits.h>
#include <ResourceLimits.cpp>
#include <Worklist.h>
#include <glslang/Include/ShHandle.h>
#include <glslang/Include/revision.h>
#include <glslang/Public/ShaderLang.h>
#include <SPIRV/GlslangToSpv.h>
#include <SPIRV/GLSL.std.450.h>
#include <SPIRV/doc.h>
#include <SPIRV/disassemble.h>

namespace sh
{

namespace video
{

	VulkanShaderCompiler::VulkanShaderCompiler()
	{
	}

	std::vector<unsigned int> VulkanShaderCompiler::CompileShader( const String& source, ShaderType type )
	{
		EShLanguage language = EShLangCount;
		switch( type )
		{
			case ST_VERTEX:
				language = EShLangVertex;
				break;
			case ST_FRAGMENT:
				language = EShLangFragment;
				break;
			case ST_GEOMETRY:
				language = EShLangGeometry;
				break;
			case ST_TESSELATION_EVALUATION:
				language = EShLangTessEvaluation;
				break;
			case ST_TESSELATION_CONTROL:
				language = EShLangTessControl;
				break;
			case ST_COMPUTE:
				language = EShLangCompute;
				break;
			default:
				language = EShLangCount;
				break;
		}

		glslang::InitializeProcess();

		EShMessages messages = EShMsgDefault;
		messages = EShMessages(messages | EShMsgVulkanRules);
		messages = EShMessages(messages | EShMsgSpvRules);

		const int defaultVersion = 110;
		TBuiltInResource Resources = glslang::DefaultTBuiltInResource;

		glslang::TShader shader(language);
		const char* str = source.c_str();
		shader.setStrings(&str, 1);
		bool res = shader.parse(&Resources, defaultVersion, false, messages);
		SH_ASSERT(res, "Error compiling shader:\n%s", shader.getInfoLog());

		glslang::TProgram program;
		program.addShader(&shader);
		res = program.link(messages);
		SH_ASSERT(res, "Error linking program:\n%s", program.getInfoLog());

		std::vector<unsigned int> spirv;
		std::string warningsErrors;
		spv::SpvBuildLogger logger;
		glslang::GlslangToSpv(*program.getIntermediate(language), spirv, &logger);

		glslang::FinalizeProcess();

		return spirv;
	}

} // video

} // sh