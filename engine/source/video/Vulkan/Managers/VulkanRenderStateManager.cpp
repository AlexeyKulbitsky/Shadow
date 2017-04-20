#include "VulkanRenderStateManager.h"

#include "../VulkanRenderPipeline.h"
#include "../VulkanShader.h"
#include "../VulkanVertexDeclaration.h"
#include "../VulkanSampler.h"

namespace sh
{

namespace video
{

	RenderPipelinePtr VulkanRenderStateManager::CreateRenderPipeline(const RenderPipelineDescription& description)
	{
		RenderPipelinePtr result = nullptr;
		result.reset(new VulkanRenderPipeline(description));
		return result;
	}

	ShaderPtr VulkanRenderStateManager::CreateShader(const ShaderDescription& description)
	{
		ShaderPtr result = nullptr;
		result.reset(new VulkanShader(description));
		return result;
	}

	SamplerPtr VulkanRenderStateManager::CreateSampler(const SamplerDescription& description)
	{
		SamplerPtr result;
		result.reset(new VulkanSampler(description));
		return result;
	}

	VertexInputDeclarationPtr VulkanRenderStateManager::CreateVertexDeclaration()
	{
		VertexInputDeclarationPtr result;
		result.reset(new VulkanVertexDeclaration());
		return result;
	}

} // video

} // sh