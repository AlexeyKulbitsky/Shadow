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
		glslang::InitializeProcess();

		const char* vertShaderStr =
			"#version 450\n"													\
			"#extension GL_ARB_separate_shader_objects : enable\n"				\
			"layout(std140, push_constant) uniform PushConsts\n"				\
			"{\n"																\
			"	mat4 MVP;\n"													\
			"} pushConsts;\n"													\
			"layout(location = 0) in vec3 aPosition;\n"							\
			"void main()\n"														\
			"{\n"																\
			"	gl_Position = pushConsts.MVP * vec4(aPosition, 1.0);\n"			\
			"}";
		//*/
		const char* fragShaderStr =
			"#version 450\n"													\
			"#extension GL_ARB_separate_shader_objects : enable\n"				\
			"layout(location = 0) out vec4 outColor;\n"							\
			"void main()\n"														\
			"{\n"																\
			"	outColor = vec4(1.0);\n"										\
			"}";


		EShMessages messages = EShMsgDefault;
		messages = EShMessages(messages | EShMsgVulkanRules);
		messages = EShMessages(messages | EShMsgSpvRules);

		const int defaultVersion = 110;
		TBuiltInResource Resources = glslang::DefaultTBuiltInResource;

		glslang::TShader shaderVertex(EShLangVertex);
		shaderVertex.setStrings(&vertShaderStr, 1);
		bool res = shaderVertex.parse(&Resources, defaultVersion, false, messages);
		printf("MESSAGE %s", shaderVertex.getInfoLog());

		glslang::TShader shaderFragment(EShLangFragment);
		shaderFragment.setStrings(&fragShaderStr, 1);
		res = shaderFragment.parse(&Resources, defaultVersion, false, messages);
		printf("MESSAGE %s", shaderFragment.getInfoLog());

		glslang::TProgram program;
		program.addShader(&shaderVertex);
		program.addShader(&shaderFragment);
		res = program.link(messages);
		printf("MESSAGE %s", program.getInfoLog());

		std::vector<unsigned int> spirv;
		std::string warningsErrors;
		spv::SpvBuildLogger logger;
		glslang::GlslangToSpv(*program.getIntermediate(EShLangVertex), spirv, &logger);


		int a = 0;
		a++;
		glslang::FinalizeProcess();
	}

} // video

} // sh