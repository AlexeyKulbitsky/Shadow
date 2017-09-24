#ifndef SHADOW_COMMON_TYPES_INCLUDE
#define SHADOW_COMMON_TYPES_INCLUDE

#include <map>

namespace sh
{
	
	enum BlendFactor
	{
		BF_ZERO = 0,
		BF_ONE,
		BF_SRC_COLOR,
		BF_INV_SRC_COLOR,
		BF_DST_COLOR,
		BF_INV_DST_COLOR,
		BF_SRC_ALPHA,
		BF_INV_SRC_ALPHA,
		BF_DST_ALPHA,
		BF_INV_DST_ALPHA,
		BF_SRC_ALPHA_SATURATED,
		BF_COUNT
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

	enum BlendOperation
	{
		BO_ADD = 0,
		BO_SUBSTRACT,
		BO_REVERSE_SUBSTRACT,
		BO_MIN,
		BO_MAX,
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

	enum CompareFunction
	{
		COMP_FUNC_LESS = 0,
		COMP_FUNC_LESS_OR_EQUAL,
		COMP_FUNC_EQUAL,
		COMP_FUNC_GREATER_OR_EQUAL,
		COMP_FUNC_GREATER,
		COMP_FUNC_NOT_EQUAL,
		COMP_FUNC_ALWAYS,
		COMP_FUNC_NEVER,
		COMP_FUNC_COUNT
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

	enum WriteMask
	{
		WM_ZERO = 0,
		WM_ALL,
	};

	static const char* const g_writeMaskMap[] =
	{
		"Zero",
		"All"
	};

	////////////////////////////////////////////////////////////

	enum StencilOperation
	{
		STENCIL_OP_KEEP = 0,
		STENCIL_OP_CLEAR,
		STENCIL_OP_REPLACE,
		STENCIL_OP_INVERT,
		STENCIL_OP_INCREMENT,
		STENCIL_OP_DECREMENT,
		STENCIL_OP_INCREMENT_WRAP,
		STENCIL_OP_DECREMENT_WRAP,
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

	enum FrontFace
	{
		FF_CLOCKWISE = 0,
		FF_COUNTER_CLOCKWISE,
	};

	static const char* const g_frontFaceMap[] =
	{
		"ClockWise",
		"CounterClockWise"
	};

	////////////////////////////////////////////////////////////

	enum CullFace
	{
		CF_FRONT = 0,
		CF_BACK,
		CF_NONE,
	};

	static const char* const g_cullFaceMap[] =
	{
		"Front",
		"Back",
		"None"
	};

	////////////////////////////////////////////////////////////

	enum FillMode
	{
		FM_SOLID = 0,
		FM_WIREFRAME,
	};

	static const char* const g_fillModeMap[] =
	{
		"Solid",
		"WireFrame"
	};

	enum IndexType
	{
		IT_16_BIT,
		IT_32_BIT
	};

	enum Usage
	{
		USAGE_STATIC = 0,
		USAGE_DYNAMIC,
		USAGE_STREAM
	};

	enum Topology
	{
		TOP_LINE_LIST = 0,
		TOP_LINE_LOOP,
		TOP_LINE_STRIP,
		TOP_TRIANGLE_LIST,
		TOP_TRIANGLE_STRIP,
		TOP_TRIANGLE_FAN
	};

	enum ShaderType
	{
		ST_VERTEX = 0,
		ST_FRAGMENT,
		ST_GEOMETRY,
		ST_TESSELATION_EVALUATION,
		ST_TESSELATION_CONTROL,
		ST_COMPUTE,
		ST_NONE
	};

	enum TextureType
	{
		TEX_TYPE_TEXTURE_1D,
		TEX_TYPE_TEXTURE_2D,
		TEX_TYPE_TEXTURE_3D,
		TEX_TYPE_TEXTURE_CUBE
	};

	enum TextureTiling
	{
		TEX_TILING_REPEAT,
		TEX_TILING_MIRRORED_REPEAT,
		TEX_TILING_CLAMP_TO_EDGE,
		TEX_TILING_NONE
	};

	enum TextureFiltering
	{
		TEX_FILT_NEAREST,
		TEX_FILT_LINEAR,
		TEX_FILT_NONE
	};

	enum TextureFace
	{
		TEX_FRONT_FACE,
		TEX_BACK_FACE,
		TEX_RIGHT_FACE,
		TEX_LEFT_FACE,
		TEX_TOP_FACE,
		TEX_BOTTOM_FACE,

		TEX_COUNT = 6
	};

	enum class TextureFormat : int
	{
		UNDEFINED = -1,
		RGB = 0,
		RGBA,
		DXT1_RGB,
		DXT1_RGBA,
		ALPHA
	};

	enum class AttributeSemantic
	{
		UNDEFINED = -1,
		POSITION = 0,
		COLOR,
		UV,
		NORMAL,
		BINORMAL,
		TANGENT,

		COUNT
	};

	static const std::map<std::string, AttributeSemantic> attributeSemanticMap =
	{
		{ "position", AttributeSemantic::POSITION },
		{ "color", AttributeSemantic::COLOR },
		{ "uv", AttributeSemantic::UV },
		{ "normal", AttributeSemantic::NORMAL },
		{ "binormal", AttributeSemantic::BINORMAL },
		{ "tangent", AttributeSemantic::TANGENT }
	};

	enum class AttributeType
	{
		FLOAT = 4,
		INT = 4,
		UNSIGNED_INT = 4,
		BYTE = 1,
		UNSIGNED_BYTE = 1
	};

	enum GpuParamDataType
	{
		GPDT_FLOAT1,
		GPDT_FLOAT2,
		GPDT_FLOAT3,
		GPDT_FLOAT4,
		GPDT_INT1,
		GPDT_INT2,
		GPDT_INT3,
		GPDT_INT4,
		GPDT_BOOL1,
		GPDT_BOOL2,
		GPDT_BOOL3,
		GPDT_BOOL4,
		GPDT_MATRIX4,
		GPDT_MATRIX3,
		GPDT_MATRIX2
	};

	enum GpuParamObjectType
	{
		GPOT_SAMPLER_1D,
		GPOT_SAMPLER_2D,
		GPOT_SAMPLER_3D,
		GPOT_SAMPLER_CUBE
	};

	enum CommandBufferType
	{
		COMMAND_BUFFER_TYPE_PRIMARY,
		COMMAND_BUFFER_TYPE_SECONDARY
	};

	enum class MaterialParamUsage : int
	{
		Undefined = -1,
		MatrixWorldViewProjection,
		MatrixWorld,
		MatrixView,
		MatrixViewRotation,
		MatrixViewRotationProjection,
		MatrixProjection,
		MatrixViewProjection,
		DirectionalLightDirection,
		DirectionalLightColor,
		CameraPosition
	};

	enum class MaterialParamType
	{
		Matrix4,
		Matrix3,
		Float4,
		Float3,
		Float2,
		Float,
		Int4,
		Int3,
		Int2,
		Int,
		Sampler2D,
		SamplerCube
	};

	static std::string const materialParamTypeStringArray[] =
	{
		"Matrix4",
		"Matrix3",
		"Float4",
		"Float3",
		"Float2",
		"Float",
		"Int4",
		"Int3",
		"Int2",
		"Int",
		"Sampler2D",
		"SamplerCube"
	};
	
	static const std::map<std::string, MaterialParamUsage> materialAutoParamsMap =
	{
		{ "matWVP", MaterialParamUsage::MatrixWorldViewProjection },
		{ "matWorld", MaterialParamUsage::MatrixWorld },
		{ "matView", MaterialParamUsage::MatrixView },
		{ "matViewRotation", MaterialParamUsage::MatrixViewRotation },
		{ "matViewRotationProjection", MaterialParamUsage::MatrixViewRotationProjection },
		{ "matProjection", MaterialParamUsage::MatrixProjection },
		{ "matViewProjection", MaterialParamUsage::MatrixViewProjection }
	};

	static const std::map<std::string, MaterialParamUsage> materialCommonParamsMap =
	{
		{ "dLightDirection", MaterialParamUsage::DirectionalLightDirection },
		{ "dLightColor", MaterialParamUsage::DirectionalLightColor },
		{ "cameraPosition", MaterialParamUsage::CameraPosition }
	};

	static const std::map<std::string, GpuParamObjectType> samplerTypeMap =
	{
		{ "1D", GPOT_SAMPLER_1D },
		{ "2D", GPOT_SAMPLER_2D },
		{ "3D", GPOT_SAMPLER_3D },
		{ "Cube", GPOT_SAMPLER_CUBE }
	};

	static std::string const samplerTypeStringArray[] = 
	{
		"1D",
		"2D",
		"3D",
		"Cube"
	};
	
	static const std::map<std::string, TextureFiltering> textureFilteringMap =
	{
		{ "Nearest", TEX_FILT_NEAREST },
		{ "Linear", TEX_FILT_LINEAR },
		{ "None", TEX_FILT_NONE }
	};

	static std::string const textureFilteringStringArray[] = 
	{
		"Nearest",
		"Linear",
		"None"
	};

	static const std::map<std::string, TextureTiling> textureTilingMap =
	{
		{ "Repeat", TEX_TILING_REPEAT },
		{ "MirroredRepeat", TEX_TILING_MIRRORED_REPEAT },
		{ "ClampToEdge", TEX_TILING_CLAMP_TO_EDGE }
	};

	static std::string const textureTilingStringArray[] = 
	{
		"Repeat",
		"MirroredRepeat",
		"ClampToEdge"
	};
}

#endif
