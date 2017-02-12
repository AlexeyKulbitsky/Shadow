#ifndef SHADOW_GEOMETRY_GENERATOR_INCLUDE
#define SHADOW_GEOMETRY_GENERATOR_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace scene
	{
		class GeometryGenerator
		{
		public:
			static ModelPtr GetLineModel(const math::Vector3f& start, const math::Vector3f& end);
			static ModelPtr GetConeModel(const math::Vector3f& start, const math::Vector3f& axis, const math::Vector3f& vtx, const math::Vector3f& vty);
			static ModelPtr GetCircleModel(const math::Vector3f& centre, const f32 radius, const math::Vector3f& vtx, const math::Vector3f& vty);
			static ModelPtr GetCylinderModel(const math::Vector3f& start, const f32 radius, const math::Vector3f& direction, const math::Vector3f& vtx, const math::Vector3f& vty);
			static ModelPtr GetTorusModel(const math::Vector3f& start, const f32 radius, const f32 ringRadius, u32 sides, u32 rings, const math::Vector3f& vtx, const math::Vector3f& vty);
			static ModelPtr GetHalfTorusModel(const math::Vector3f& start, const f32 radius, const f32 ringRadius, u32 sides, u32 rings, const math::Vector3f& vtx, const math::Vector3f& vty);
			static ModelPtr GetCylinder(f32 height, f32 radius, u32 numberOfSides);
		};
	}
}

#endif