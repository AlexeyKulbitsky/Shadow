#ifndef SHADOW_VERTEX_INCLUDE
#define SHADOW_VERTEX_INCLUDE

#include "../math/vec3.h"

namespace sh
{
	namespace video
	{
		struct Vertex
		{
			Vertex(){}
			Vertex(f32 x, f32 y, f32 z, f32 r, f32 g, f32 b)
				: pos(x, y, z)
				, color(r, g, b)
			{}
			Vertex(const math::vec3f& p, const math::vec3f& c)
				: pos(p)
				, color(c)
			{}
			math::vec3f pos;
			math::vec3f color;
		};
	}
}
#endif