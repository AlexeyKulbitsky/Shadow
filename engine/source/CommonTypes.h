#ifndef SHADOW_COMMON_TYPES_INCLUDE
#define SHADOW_COMMON_TYPES_INCLUDE

namespace sh
{
	enum class BlendFactor
	{
		ZERO = 0,
		ONE,
		SRC_COLOR,
		INV_SRC_COLOR,
		DST_COLOR,
		INV_DST_COLOR,
		SRC_ALPHA,
		INV_SRC_ALPHA,
		DST_ALPHA,
		INV_DST_ALPHA,
		SRC_ALPHA_SATURATED,

		COUNT
	};

	static const char* const g_blendFactorMap[] =
	{
		"Zero",
		"One",
		"SrcColor",
		"InvSrcColor",
		"DstColor",
		"InvDstColor",
		"SrcAlpha",
		"InvSrcAlpha",
		"DstAlpha",
		"InvDstAlpha",
		"SrcAlphaSturated"
	};

	////////////////////////////////////////////////////////////

	enum class BlendOperation
	{
		ADD = 0,
		SUBSTRACT,
		REVERSE_SUBSTRACT,
		MIN,
		MAX,

		COUNT
	};

	static const char* const g_blendOperationMap[] =
	{
		"Add",
		"Substract",
		"ReverseSubstract",
		"Min",
		"Max"
	};

	////////////////////////////////////////////////////////////
	enum class BlendCombination
	{
		SOLID = 0,
		ALPHA,
		ADDITIVE,
		MULTIPLY,
		BURN,
		SCREEN,
		SUBSTRACTIVE,

		COUNT
	};

	static const char* const g_blendCombinationMap[] =
	{
		"Solid",
		"Alpha",
		"Additive",
		"Multiply",
		"Burn",
		"Screen",
		"Substractive"
	};

	enum class CompareFunction
	{
		LESS = 0,
		LESS_OR_EQUAL,
		EQUAL,
		GREATER_OR_EQUAL,
		GREATER,
		NOT_EQUAL,
		ALWAYS,
		NEVER,

		COUNT
	};

	static const char* const g_compareFunctionMap[] =
	{
		"Less",
		"LessOrEqual",
		"Equal",
		"GreaterOrEqual",
		"Greater",
		"NotEqual",
		"Always",
		"Never"
	};

	////////////////////////////////////////////////////////////

	enum class WriteMask
	{
		ZERO = 0,
		ALL,

		COUNT
	};

	static const char* const g_writeMaskMap[] =
	{
		"Zero",
		"All"
	};

	////////////////////////////////////////////////////////////

	enum class StencilOperation
	{
		KEEP = 0,
		CLEAR,
		REPLACE,
		INVERT,
		INCREMENT,
		DECREMENT,
		INCREMENT_WRAP,
		DECREMENT_WRAP,

		COUNT
	};

	static const char* const g_stencilOperationMap[] =
	{
		"Keep",
		"Clear",
		"Replace",
		"Invert",
		"Increment",
		"Decrement",
		"IncrementWrap",
		"DecrementWrap"
	};

	enum class FrontFace
	{
		CLOCKWISE = 0,
		COUNTER_CLOCKWISE,

		COUNT
	};

	static const char* const g_frontFaceMap[] =
	{
		"ClockWise",
		"CounterClockWise"
	};

	////////////////////////////////////////////////////////////

	enum class CullFace
	{
		FRONT = 0,
		BACK,
		NONE,

		COUNT
	};

	static const char* const g_cullFaceMap[] =
	{
		"Front",
		"Back",
		"None"
	};

	////////////////////////////////////////////////////////////

	enum class FillMode
	{
		SOLID = 0,
		WIREFRAME,

		COUNT
	};

	static const char* const g_fillModeMap[] =
	{
		"Solid",
		"WireFrame"
	};

	enum class IndexType
	{
		UNSIGNED_16_BIT,
		UNSIGNED_32_BIT
	};

	enum Usage
	{
		STATIC = 0,
		DYNAMIC
	};

	enum class Topology
	{
		LINE_LIST = 0,
		LINE_LOOP,
		LINE_STRIP,
		TRIANGLE_LIST,
		TRIANGLE_STRIP,
		TRIANGLE_FAN
	};

	enum class ShaderType
	{
		VERTEX = 0,
		FRAGMENT,
		//GEOMETRY,
		//DOMAIN,
		//HULL,
		//COMPUTE,
		NONE
	};

	enum class TextureType
	{
		TEXTURE_1D,
		TEXTURE_2D,
		TEXTURE_3D,
		TEXTURE_CUBE
	};

	enum class TextureTiling
	{
		REPEAT,
		MIRRORED_REPEAT,
		CLAMP_TO_EDGE
	};

	enum class TextureFiltering
	{
		NEAREST,
		LINEAR,
		BILINEAR,
		TRILINEAR,
		ANISOTROPIC
	};

	enum class TextureFace
	{
		FRONT_FACE,
		BACK_FACE,
		RIGHT_FACE,
		LEFT_FACE,
		TOP_FACE,
		BOTTOM_FACE,

		COUNT = 6
	};

	enum class TextureFormat
	{
		UNDEFINED,
		DXT1_RGB,
		DXT1_RGBA
	};

	enum class AttributeSemantic
	{
		POSITION = 0,
		COLOR,
		UV,
		NORMAL,
		BINORMAL,
		TANGENT,

		COUNT
	};

	enum class AttributeType
	{
		FLOAT = 4,
		INT = 4,
		UNSIGNED_INT = 4,
		BYTE = 1,
		UNSIGNED_BYTE = 1
	};
}

#endif
