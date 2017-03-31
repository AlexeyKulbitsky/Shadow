#ifndef SHADOW_GLES20_COMMON_INCLUDE
#define SHADOW_GLES20_COMMON_INCLUDE

#include "../../Globals.h"

namespace sh
{

namespace video
{

	static GLenum const s_glWriteMask[] =
	{
		GL_FALSE,
		GL_TRUE
	};


	static GLenum const s_glFrontFace[] =
	{
		GL_CW,
		GL_CCW
	};

	static GLenum const s_glCullFace[] =
	{
		GL_FRONT,
		GL_BACK,
		GL_NONE
	};

	static GLenum const s_glCompareFunction[] =
	{
		GL_LESS,
		GL_LEQUAL,
		GL_EQUAL,
		GL_GEQUAL,
		GL_GREATER,
		GL_NOTEQUAL,
		GL_ALWAYS,
		GL_NEVER
	};

	static GLenum const s_glStencilOperation[] =
	{
		GL_KEEP,
		GL_ZERO,
		GL_REPLACE,
		GL_INVERT,
		GL_INCR,
		GL_DECR,
		GL_INCR_WRAP,
		GL_DECR_WRAP
	};

	static GLenum const s_glBlendFactor[] =
	{
		GL_ZERO,
		GL_ONE,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_SRC_ALPHA_SATURATE
	};
		
	static GLenum const s_glBlendOperation[] =
	{
		GL_FUNC_ADD,
		GL_FUNC_SUBTRACT,
		GL_FUNC_REVERSE_SUBTRACT,
		GL_MIN,
		GL_MAX
	};

	static GLenum const s_glTopology[] =
	{
		GL_LINES,
		GL_LINE_LOOP,
		GL_LINE_STRIP,
		GL_TRIANGLES,
		GL_TRIANGLE_STRIP,
		GL_TRIANGLE_FAN
	};

	static GLenum const s_glIndexType[] =
	{
		GL_UNSIGNED_SHORT,
		GL_UNSIGNED_INT
	};

} // video

} // sh


#endif