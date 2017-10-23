#ifndef SHADOW_GL_COMMON_INCLUDE
#define SHADOW_GL_COMMON_INCLUDE

#include "../../Globals.h"

#if defined SHADOW_APPLE

#include <OpenGLES/ES3/gl.h>

#elif defined SHADOW_ANDROID

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#elif defined SHADOW_WINDOWS

//#include <gl/glew.h>
//#include <gl/wglew.h>

#include "../../../../libs/glew/include/GL/glew.h"
#include "../../../../libs/glew/include/GL/wglew.h"

#endif

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

	static GLenum const s_glUsage[] =
	{
		GL_STATIC_DRAW,
		GL_DYNAMIC_DRAW,
		GL_STREAM_DRAW
	};

	static GLenum const s_glTextureType[] =
	{
		GL_TEXTURE,
		GL_TEXTURE_2D,
		GL_TEXTURE_3D,
		GL_TEXTURE_CUBE_MAP
	};

	static GLenum const s_glTextureFace[] =
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	static GLenum const s_glTextureFormat[] =
	{
		GL_RGB,
		GL_RGBA,
		GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
		GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
		GL_LUMINANCE
	};

	static GLint const s_glTextureFiltering[] =
	{
		GL_NEAREST,
		GL_LINEAR
	};

	static GLint const s_glTextureTiling[] =
	{
		GL_REPEAT,
		GL_MIRRORED_REPEAT,
		GL_CLAMP_TO_EDGE
	};





#define GL_CALL(x)									\
{													\
	(x);											\
	GLDebug::CheckGLError(__FILE__, __LINE__);		\
}



	class GLDebug
	{
	public:
		static void CheckGLError(const char* file, unsigned line)
		{
			GLenum error = glGetError();
			while (error != GL_NO_ERROR)
			{
				std::ostringstream ss;
				switch (error)
				{
				case GL_INVALID_ENUM:
					ss << "ERROR: GL_INVALID_ENUM\n" << "Enumeration parameter(s) is(are) not a legal enumeration for this function";
					break;
				case GL_INVALID_VALUE:
					ss << "ERROR: GL_INVALID_VALUE\n" << "Value parameter(s) is(are) not a legal value(s) for this function";
					break;
				case GL_INVALID_OPERATION:
					ss << "ERROR: GL_INVALID_OPERATION\n" << "The set of state for a command is not legal for the parameters given to that command";
					break;
				case GL_STACK_OVERFLOW:
					ss << "ERROR: GL_STACK_OVERFLOW\n" << "Stack pushing operation cannot be done because it would overflow the limit of that stack's size";
					break;
				case GL_STACK_UNDERFLOW:
					ss << "ERROR: GL_STACK_UNDERFLOW\n" << "Stack popping operation cannot be done because the stack is already at its lowest point";
					break;
				case GL_OUT_OF_MEMORY:
					ss << "ERROR: GL_OUT_OF_MEMORY\n" << "Memory cannot be allocated";
					break;
				case GL_INVALID_FRAMEBUFFER_OPERATION:
					ss << "ERROR: GL_INVALID_FRAMEBUFFER_OPERATION\n" << "Doing anything that would attempt to read from or write/render to a framebuffer that is not complete";
					break;
				default:
					ss << "ERROR: unknown OpenGL error";
					break;
				}
				String message = ss.str();
				SH_ASSERT(0, message.c_str());
				error = glGetError();
			}
		}
	};



} // video

} // sh


#endif