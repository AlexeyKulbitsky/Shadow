#ifndef SHADOW_GEOMETRY_GENERATOR_INCLUDE
#define SHADOW_GEOMETRY_GENERATOR_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace scene
	{
		class SHADOW_API GeometryGenerator
		{
		public:
			static ModelPtr GetLineModel(const math::Vector3f& start, const math::Vector3f& end);
			static ModelPtr GetCircleModel(const math::Vector3f& centre, const f32 radius, const math::Vector3f& vtx, const math::Vector3f& vty);
			static ModelPtr GetTorusModel(const math::Vector3f& start, const f32 radius, const f32 ringRadius, u32 sides, u32 rings, const math::Vector3f& vtx, const math::Vector3f& vty);
			static ModelPtr GetTorusModel(const f32 radius, const f32 ringRadius, u32 sides, u32 rings, math::Matrix4f transform = math::Matrix4f::Identity());
			static ModelPtr GetHalfTorusModel(const f32 radius, const f32 ringRadius, u32 sides, u32 rings, math::Matrix4f transform = math::Matrix4f::Identity());
			static ModelPtr GetHalfTorusModel(const math::Vector3f& start, const f32 radius, const f32 ringRadius, u32 sides, u32 rings, const math::Vector3f& vtx, const math::Vector3f& vty);
			static ModelPtr GetCylinderModel(f32 height, f32 radius, u32 numberOfSides, math::Matrix4f transform = math::Matrix4f::Identity());
			static ModelPtr GetConeModel(f32 radius, f32 height, math::Matrix4f transform = math::Matrix4f::Identity());
			static ModelPtr GetPlaneModel(f32 width, f32 height, math::Matrix4f transform = math::Matrix4f::Identity());
		private:
			static ModelPtr CreateModel(const std::vector<float>& vertexArray, const std::vector<u32>& indexArray, 
				const video::VertexDeclarationPtr& vertexDeclaration, Topology topology);
		};
	}
}

#endif