#ifndef SHADOW_UNIFORM_INCLUDE
#define SHADOW_UNIFORM_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		enum class GlobalUniformName
		{
			NONE = -1,

			// Model uniforms
			MODEL_WORLD_MATRIX = 0,
			MODEL_WORLD_VIEW_PROJECTION_MATRIX,
			MODEL_NORMAL_MATRIX,

			// Camera uniforms			
			CAMERA_VIEW_MATRIX,
			CAMERA_VIEW_TRANSLATION_MATRIX,
			CAMERA_VIEW_ROTATION_MATRIX,
			CAMERA_PROJECTION_MATRIX,
			CAMERA_VIEW_PROJECTION_MATRIX,
			CAMERA_VIEW_ROTATION_PROJECTION_MATRIX,
			CAMERA_POSITION,

			// Light uniforms
			LIGHT_DIRECTIONAL_LIGHTS_COUNT,
			LIGHT_DIRECTIONAL_DIRECTION,
			LIGHT_DIRECTIONAL_COLOR,

			COUNT
		};

		static const String k_globalUniformsStringNames[] =
		{
			// Model uniforms
			String("model.worldMatrix"),
			String("model.worldViewProjectionMatrix"),
			String("model.normalMatrix"),

			// Camera uniforms
			String("camera.viewMatrix"),
			String("camera.viewTranslationMatrix"),
			String("camera.viewRotationMatrix"),
			String("camera.projectionMatrix"),
			String("camera.viewProjectionMatrix"),		
			String("camera.viewRotationProjectionMatrix"),
			String("camera.position"),

			// Light uniforms
			String("light.directional.count"),
			String("light.directional.direction"),
			String("light.directional.color")
		};

		class Uniform
		{
			public:	
			/*
			Base implementation used just for holding values
			More specific type of Uniform that includes renderindg system
			information is defined in derived classes for each renser system
			*/
			enum class Type
			{
				FLOAT,
				INT,
				VEC2,
				VEC3,
				VEC4,
				MAT4,
				VEC3_ARRAY,
				UNKNOWN
			};
		
			enum class Usage
			{
				UNKNOWN
			};

			template<typename T>
			Uniform(const T& value);

			~Uniform() { delete m_placeHolder; }

			template<typename T>
			void Set(const T& value)
			{
				SH_ASSERT(typeid(T) == m_placeHolder->GetTypeInfo(), "Error types!");
				static_cast<UniformHolder<T>*>(m_placeHolder)->SetValue(value);
			}

			template<typename T>
			const T& Get() const
			{
				SH_ASSERT(typeid(T) == m_placeHolder->GetTypeInfo(), "Error types!");
				return static_cast<UniformHolder<T>*>(m_placeHolder)->GetValue();
			}

			void SetName(const std::string& name) { m_name = name; }
			const std::string& GetName() const { return m_name; }
			void SetUsage(Usage usage) { m_usage = usage; }
			Usage GetUsage() const { return m_usage; }
			void SetGlobalUniformName(GlobalUniformName name) { m_globalName = name; }
			GlobalUniformName GetGlobalUniformName() const { return m_globalName; }
			Type GetType() const { return m_type; }

			virtual void Upload() {}
			virtual void Init() {}
			virtual Uniform* Clone() { return nullptr; }

		private:
			class UniformPlaceHolder
			{
			public:
				virtual ~UniformPlaceHolder() {}
				virtual const std::type_info& GetTypeInfo() const = 0;
			};

			template <typename T>
			class UniformHolder : public UniformPlaceHolder
			{
			public:
				UniformHolder() {}
				UniformHolder(const T& value) : m_value(value) {}
				virtual const std::type_info& GetTypeInfo() const override { return typeid(m_value); }
				void SetValue(const T& value) { m_value = value; }
				const T& GetValue() const { return m_value; }
			public:
				T m_value;
			};

		protected:
			Type m_type;
			Usage m_usage;
			GlobalUniformName m_globalName = GlobalUniformName::NONE;
			std::string m_name;
			UniformPlaceHolder* m_placeHolder;
		};

		/////////////////////////////////////////////////////////////////////////////////
		// Constructors

		template<typename T>
		inline Uniform::Uniform(const T& value)
			: m_placeHolder(new UniformHolder<T>(value))
			, m_type(Type::UNKNOWN)
		{
		}

		template<>
		inline Uniform::Uniform(const float& value)
			: m_placeHolder(new UniformHolder<float>(value))
			, m_type(Type::FLOAT)
		{
		}

		template<>
		inline Uniform::Uniform(const int& value)
			: m_placeHolder(new UniformHolder<int>(value))
			, m_type(Type::INT)
		{
		}

		template<>
		inline Uniform::Uniform(const math::Vector2f& value)
			: m_placeHolder(new UniformHolder<math::Vector2f>(value))
			, m_type(Type::VEC2)
		{
		}

		template<>
		inline Uniform::Uniform(const math::Vector3f& value)
			: m_placeHolder(new UniformHolder<math::Vector3f>(value))
			, m_type(Type::VEC3)
		{
		}

		template<>
		inline Uniform::Uniform(const math::Vector4f& value)
			: m_placeHolder(new UniformHolder<math::Vector4f>(value))
			, m_type(Type::VEC4)
		{
		}

		template<>
		inline Uniform::Uniform(const math::Matrix4f& value)
			: m_placeHolder(new UniformHolder<math::Matrix4f>(value))
			, m_type(Type::MAT4)
		{
		}

		template<>
		inline Uniform::Uniform(const std::vector<math::Vector3f>& value)
			: m_placeHolder(new UniformHolder<std::vector<math::Vector3f> >(value))
			, m_type(Type::VEC3_ARRAY)
		{
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
	namespace video
	{

		struct mat43f
		{
			math::vec4f data[3];
			bool operator==(const mat43f& other) const
			{
				return data[0] == other.data[0] && data[1] == other.data[1] && data[2] == other.data[2];
			}
			bool operator!=(const mat43f& other) const
			{
				return !operator==(other);
			}
		};
		struct mat32f
		{
			math::vec3f data[2];
			bool operator==(const mat32f& other) const
			{
				return data[0] == other.data[0] && data[1] == other.data[1];
			}
			bool operator!=(const mat32f& other) const
			{
				return !operator==(other);
			}
		};


		class ShaderUniform
		{
			friend std::size_t hash_value(const ShaderUniform&);
		public:
			struct TypeDef
			{
				enum type
				{
					NONE,
					INT,
					FLOAT,
					MAT3,
					MAT4,
					VEC2,
					VEC3,
					VEC4,
					MAT43,
					MAT32
				};

				inline const char* name(type t)	const
				{
					ENUM_START(t)
						ENUM_CASE(NONE)
						ENUM_CASE(INT)
						ENUM_CASE(FLOAT)
						ENUM_CASE(MAT3)
						ENUM_CASE(MAT4)
						ENUM_CASE(VEC2)
						ENUM_CASE(VEC3)
						ENUM_CASE(VEC4)
						ENUM_CASE(MAT43)
						ENUM_CASE(MAT32)
						ENUM_END()
				}
			};
			typedef safe_enum<TypeDef, u8> Type;

			//type will be NONE
			ShaderUniform();
			~ShaderUniform();

			//this will use data as storage.
			ShaderUniform(Type type, u16 count, u8* data = 0);

			ShaderUniform(const ShaderUniform& other);

			//explictly construct a uniform with the corresponding type
			explicit ShaderUniform(int val);
			explicit ShaderUniform(float val);
			explicit ShaderUniform(const math::mat3f& mat);
			explicit ShaderUniform(const math::mat4f& mat);
			explicit ShaderUniform(const math::vec2f& vec);
			explicit ShaderUniform(const math::vec3f& vec);
			explicit ShaderUniform(const math::vec4f& vec);
			explicit ShaderUniform(const mat32f& mat);
			explicit ShaderUniform(const mat43f& mat);

			bool			operator==(const ShaderUniform& other) const;
			bool			operator!=(const ShaderUniform& other) const;

			//NOTE - this changes the type of the uniform to the specified type
			ShaderUniform& operator=(const ShaderUniform& other);
			ShaderUniform& operator=(int val);
			ShaderUniform& operator=(float val);
			ShaderUniform& operator=(const math::mat3f& mat);
			ShaderUniform& operator=(const math::mat4f& mat);
			ShaderUniform& operator=(const math::vec2f& vec);
			ShaderUniform& operator=(const math::vec3f& vec);
			ShaderUniform& operator=(const math::vec4f& vec);
			ShaderUniform& operator=(const mat32f& mat);
			ShaderUniform& operator=(const mat43f& mat);

			void					SetAndClamp(const ShaderUniform& val, const ShaderUniform& minVal, const ShaderUniform& maxVal);

			// a server can automatically put data in this uniform, and uses the data to identify the uniform
			void					SetServerData(u32 data);
			u32						GetServerData() const;
			bool					HasServerData() const;
			void					RemoveServerData();

			// a client can read data from this uniform, and uses the data to identify the uniform
			void					SetClientData(u32 data);
			u32						GetClientData() const;
			bool					HasClientData() const;
			void					RemoveClientData();

			u32						GetDataVersion() const;
			void					SetDataVersion(u32 ver);


			//the type can only be changed if no data was set
			Type					GetType() const;
			bool					IsValid() const;

			//string parsing
			void					ParseType(const String& type);
			void					ParseValue(const String& val, bool link);
			void					ParseArrayValues(const String& val);

			// Returns the number of elements in the array (1 for non-array uniforms)
			u32						GetCount() const;

		public:// non-array-uniforms getters
			int						GetInt() const;
			float					GetFloat() const;
			const math::mat3f&		GetMat3() const;
			const math::mat4f&		GetMat4() const;
			const math::vec2f&		GetVec2() const;
			const math::vec3f&		GetVec3() const;
			const math::vec4f&		GetVec4() const;
			const mat32f&			GetMat32() const;
			const mat43f&			GetMat43() const;

			bool					EqualsData(const u8* data) const;
			void					SetData(const u8* data);
			const u8*				GetData() const;
			u32						GetDataSize() const; //in bytes

		public:// array-uniforms getters/setters
			void Get(u32 idx, int* values, u32 count = 1) const;
			void Get(u32 idx, float* values, u32 count = 1) const;
			void Get(u32 idx, math::mat3f* values, u32 count = 1) const;
			void Get(u32 idx, math::mat4f* values, u32 count = 1) const;
			void Get(u32 idx, mat43f* values, u32 count = 1) const;
			void Get(u32 idx, mat32f* values, u32 count = 1) const;
			void Get(u32 idx, math::vec2f* values, u32 count = 1) const;
			void Get(u32 idx, math::vec3f* values, u32 count = 1) const;
			void Get(u32 idx, math::vec4f* values, u32 count = 1) const;

			void Set(u32 idx, const int* values, u32 count = 1);
			void Set(u32 idx, const float* values, u32 count = 1);
			void Set(u32 idx, const math::mat3f* values, u32 count = 1);
			void Set(u32 idx, const math::mat4f* values, u32 count = 1);
			void Set(u32 idx, const mat43f* values, u32 count = 1);
			void Set(u32 idx, const mat32f* values, u32 count = 1);
			void Set(u32 idx, const math::vec2f* values, u32 count = 1);
			void Set(u32 idx, const math::vec3f* values, u32 count = 1);
			void Set(u32 idx, const math::vec4f* values, u32 count = 1);

		protected:
			void					CommonConstructor();
			void					Allocate(Type type, u16 count);
			void					Touch();

			bool					ParseInt(const char* str, int& val);
			bool					ParseFloat(const char* str, float& val);
			bool					ParseVec2(const char* str, math::vec2f& val);
			bool					ParseVec3(const char* str, math::vec3f& val);
			bool					ParseVec4(const char* str, math::vec4f& val);

			u32						m_serverData;
			u32						m_clientData;

			u8						m_hasClientServerData;
			Type					m_type;
			u16						m_dataVersion;

			u32						m_count;
			boost::auto_buffer<float, boost::store_n_objects<16> > m_storage; //It's big enough to hold the biggest type = the matrix

			u8*						m_data;//we'll cast this all data types. 
		};

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform::ShaderUniform()
		{
			CommonConstructor();
			Allocate(Type::NONE, 1);
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform::ShaderUniform(Type type, u16 count, u8* data /* = 0 *///)
/*		{
			CommonConstructor();
			m_data = data;
			Allocate(type, count);
			PASSERT(!IsValid() || m_data);
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform::ShaderUniform(const ShaderUniform& other)
		{
			CommonConstructor();
			Allocate(other.m_type, other.m_count);
			u32 size = GetDataSize();
			if (size > 0)
			{
				PASSERT(m_data && other.m_data);
				memcpy(m_data, other.m_data, size);
			}

			m_hasClientServerData |= other.m_hasClientServerData;
			if (other.m_hasClientServerData & 0x1)
			{
				m_clientData = other.m_clientData;
			}

			if (other.m_hasClientServerData & 0x2)
			{
				m_serverData = other.m_serverData;
			}
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform::ShaderUniform(int val)
		{
			CommonConstructor();
			Allocate(Type::INT, 1);
			Set(0, &val);
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform::ShaderUniform(float val)
		{
			CommonConstructor();
			Allocate(Type::FLOAT, 1);
			Set(0, &val);
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform::ShaderUniform(const math::mat3f& val)
		{
			CommonConstructor();
			Allocate(Type::MAT3, 1);
			Set(0, &val);
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform::ShaderUniform(const math::mat4f& val)
		{
			CommonConstructor();
			Allocate(Type::MAT4, 1);
			Set(0, &val);
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform::ShaderUniform(const math::vec3f& val)
		{
			CommonConstructor();
			Allocate(Type::VEC3, 1);
			Set(0, &val);
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform::ShaderUniform(const math::vec4f& val)
		{
			CommonConstructor();
			Allocate(Type::VEC4, 1);
			Set(0, &val);
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform::ShaderUniform(const mat43f& val)
		{
			CommonConstructor();
			Allocate(Type::MAT43, 1);
			Set(0, &val);
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform::ShaderUniform(const mat32f& val)
		{
			CommonConstructor();
			Allocate(Type::MAT32, 1);
			Set(0, &val);
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform::ShaderUniform(const math::vec2f& val)
		{
			CommonConstructor();
			Allocate(Type::VEC2, 1);
			Set(0, &val);
		}

		//////////////////////////////////////////////////////////////////////////

		inline void ShaderUniform::CommonConstructor()
		{
			m_data = NULL;
			m_count = 0;
			m_type = Type::NONE;
			m_serverData = m_clientData = 0;
			m_hasClientServerData = 0;
			m_dataVersion = 0;
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform::~ShaderUniform()
		{
		}

		//////////////////////////////////////////////////////////////////////////

		inline bool	ShaderUniform::operator==(const ShaderUniform& other) const
		{
			PASSERT(m_type == other.m_type);
			if (m_type != other.m_type || m_count != other.m_count)
			{
				return false;
			}
			PASSERT(GetDataSize() == other.GetDataSize());
			return EqualsData(other.m_data);
		}

		//////////////////////////////////////////////////////////////////////////

		inline bool	ShaderUniform::operator!=(const ShaderUniform& other) const
		{
			return !operator==(other);
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform& ShaderUniform::operator=(const ShaderUniform& other)
		{
			if (other.m_hasClientServerData & 0x1)
			{
				m_hasClientServerData |= 0x1;
				m_clientData = other.m_clientData;
			}

			if (other.m_hasClientServerData & 0x2)
			{
				m_hasClientServerData |= 0x2;
				m_serverData = other.m_serverData;
			}

			if (m_type != other.m_type || m_count != other.m_count)
			{
				Allocate(other.m_type, other.m_count);
			}
			u32 size = GetDataSize();
			if (size > 0)
			{
				PASSERT(m_data && other.m_data);
				memcpy(m_data, other.m_data, size);
			}
			Touch();

			return *this;
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform& ShaderUniform::operator=(int val)
		{
			PASSERT(m_type == Type::INT);
			PASSERT(GetCount() == 1);
			Set(0, &val);
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform& ShaderUniform::operator=(float val)
		{
			PASSERT(m_type == Type::FLOAT);
			PASSERT(GetCount() == 1);
			Set(0, &val);
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform& ShaderUniform::operator=(const math::mat3f& val)
		{
			PASSERT(m_type == Type::MAT3);
			PASSERT(GetCount() == 1);
			Set(0, &val);
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform& ShaderUniform::operator=(const math::mat4f& val)
		{
			PASSERT(m_type == Type::MAT4);
			PASSERT(GetCount() == 1);
			Set(0, &val);
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform& ShaderUniform::operator=(const math::vec2f& val)
		{
			PASSERT(m_type == Type::VEC2);
			PASSERT(GetCount() == 1);
			Set(0, &val);
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform& ShaderUniform::operator=(const math::vec3f& val)
		{
			PASSERT(m_type == Type::VEC3);
			PASSERT(GetCount() == 1);
			Set(0, &val);
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform& ShaderUniform::operator=(const math::vec4f& val)
		{
			PASSERT(m_type == Type::VEC4);
			PASSERT(GetCount() == 1);
			Set(0, &val);
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform& ShaderUniform::operator=(const mat32f& val)
		{
			PASSERT(m_type == Type::MAT32);
			PASSERT(GetCount() == 1);
			Set(0, &val);
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform& ShaderUniform::operator=(const mat43f& val)
		{
			PASSERT(m_type == Type::MAT43);
			PASSERT(GetCount() == 1);
			Set(0, &val);
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////

		inline void ShaderUniform::Allocate(Type type, u16 count)
		{
			if (type == m_type && count <= m_count)
			{
				m_count = count;
				return;
			}
			PASSERT(count > 0);

			m_type = type;
			m_count = count;

			u32 size = GetDataSize();
			if (size > 0)
			{
				if (!m_data || !m_storage.empty())
				{
					//own data
					m_storage.uninitialized_resize(size);
					m_data = (u8*)m_storage.data();
				}
				PASSERT(m_data);
			}
			else
			{
				m_storage.uninitialized_resize(0);
				m_data = NULL;
			}
		}

		//////////////////////////////////////////////////////////////////////////

		inline void ShaderUniform::SetServerData(u32 data)
		{
			m_serverData = data;
			m_hasClientServerData |= 0x2;
		}

		//////////////////////////////////////////////////////////////////////////

		inline u32 ShaderUniform::GetServerData() const
		{
			return m_serverData;
		}

		//////////////////////////////////////////////////////////////////////////

		inline bool ShaderUniform::HasServerData() const
		{
			return (m_hasClientServerData & 0x2) != 0;
		}

		//////////////////////////////////////////////////////////////////////////

		inline void ShaderUniform::RemoveServerData()
		{
			m_hasClientServerData &= ~0x2;
		}

		//////////////////////////////////////////////////////////////////////////

		inline void ShaderUniform::SetClientData(u32 data)
		{
			m_clientData = data;
			m_hasClientServerData |= 0x1;
		}

		//////////////////////////////////////////////////////////////////////////

		inline u32 ShaderUniform::GetClientData() const
		{
			return m_clientData;
		}

		//////////////////////////////////////////////////////////////////////////

		inline bool ShaderUniform::HasClientData() const
		{
			return (m_hasClientServerData & 0x1) != 0;
		}

		//////////////////////////////////////////////////////////////////////////

		inline void ShaderUniform::RemoveClientData()
		{
			m_hasClientServerData &= ~0x1;
		}

		//////////////////////////////////////////////////////////////////////////

		inline int ShaderUniform::GetInt() const
		{
			PASSERT(m_type == Type::INT);
			PASSERT(GetCount() == 1);
			return *(int*)m_data;
		}

		//////////////////////////////////////////////////////////////////////////

		inline float ShaderUniform::GetFloat() const
		{
			PASSERT(m_type == Type::FLOAT);
			PASSERT(GetCount() == 1);
			return *(float*)m_data;
		}

		//////////////////////////////////////////////////////////////////////////

		inline const math::mat3f& ShaderUniform::GetMat3() const
		{
			PASSERT(m_type == Type::MAT3);
			PASSERT(GetCount() == 1);
			return *(math::mat3f*)m_data;
		}

		//////////////////////////////////////////////////////////////////////////

		inline const math::mat4f&	ShaderUniform::GetMat4() const
		{
			PASSERT(m_type == Type::MAT4);
			PASSERT(GetCount() == 1);
			return *(math::mat4f*)m_data;
		}

		//////////////////////////////////////////////////////////////////////////

		inline const math::vec2f& ShaderUniform::GetVec2() const
		{
			PASSERT(m_type == Type::VEC2);
			PASSERT(GetCount() == 1);
			return *(math::vec2f*)m_data;
		}

		//////////////////////////////////////////////////////////////////////////

		inline const math::vec3f& ShaderUniform::GetVec3() const
		{
			PASSERT(m_type == Type::VEC3);
			PASSERT(GetCount() == 1);
			return *(math::vec3f*)m_data;
		}

		//////////////////////////////////////////////////////////////////////////

		inline const math::vec4f& ShaderUniform::GetVec4() const
		{
			PASSERT(m_type == Type::VEC4);
			PASSERT(GetCount() == 1);
			return *(math::vec4f*)m_data;
		}

		//////////////////////////////////////////////////////////////////////////

		inline const mat43f& ShaderUniform::GetMat43() const
		{
			PASSERT(m_type == Type::MAT43);
			PASSERT(GetCount() == 1);
			return *(mat43f*)m_data;
		}

		//////////////////////////////////////////////////////////////////////////

		inline const mat32f& ShaderUniform::GetMat32() const
		{
			PASSERT(m_type == Type::MAT32);
			PASSERT(GetCount() == 1);
			return *(mat32f*)m_data;
		}

		//////////////////////////////////////////////////////////////////////////

		inline ShaderUniform::Type ShaderUniform::GetType() const
		{
			return m_type;
		}

		//////////////////////////////////////////////////////////////////////////

		inline u32 ShaderUniform::GetDataVersion() const
		{
			return m_dataVersion;
		}

		//////////////////////////////////////////////////////////////////////////

		inline void ShaderUniform::SetDataVersion(u32 ver)
		{
			m_dataVersion = ver;
		}

		//////////////////////////////////////////////////////////////////////////

		inline bool ShaderUniform::IsValid() const
		{
			return GetType() != Type::NONE;
		}

		//////////////////////////////////////////////////////////////////////////

		inline void ShaderUniform::Touch()
		{
			m_dataVersion++;
		}

		//////////////////////////////////////////////////////////////////////////

		inline u32 ShaderUniform::GetCount() const
		{
			return m_count;
		}

		//////////////////////////////////////////////////////////////////////////

		inline const u8* ShaderUniform::GetData() const
		{
			return m_data;
		}

		//////////////////////////////////////////////////////////////////////////

		inline bool ShaderUniform::EqualsData(const u8* data) const
		{
			u32 dataSize = GetDataSize();
			if (dataSize == 0)
			{
				//when the type is NONE, the data has to be NULL to be equal
				return data == NULL;
			}

			PASSERT(m_data && data && dataSize > 0);
			PASSERT((((size_t)m_data | (size_t)data) & 0x3) == 0);

			if (m_count == 1)
			{
				switch (m_type.value())
				{
				case Type::INT:
				case Type::FLOAT:
					return *(s32*)m_data == *(const s32*)data;
				case Type::VEC2:
					return *(math::vec2i*)m_data == *(const math::vec2i*)data;
				case Type::VEC3:
					return *(math::vec3i*)m_data == *(const math::vec3i*)data;
				case Type::VEC4:
					return *(math::vec4i*)m_data == *(const math::vec4i*)data;
				default:
					return memcmp(m_data, data, dataSize) == 0;
				}
			}
			else
			{
				return memcmp(m_data, data, dataSize) == 0;
			}
		}

		//////////////////////////////////////////////////////////////////////////

		inline void ShaderUniform::SetData(const u8* data)
		{
			u32 dataSize = GetDataSize();
			PASSERT(m_data && data && dataSize > 0);
			memcpy(m_data, data, dataSize);
			Touch();
		}

		//////////////////////////////////////////////////////////////////////////

		inline u32 ShaderUniform::GetDataSize() const
		{
			const u32 size[] =
			{
				0,
				sizeof(int),
				sizeof(float),
				sizeof(math::mat3f),
				sizeof(math::mat4f),
				sizeof(math::vec2f),
				sizeof(math::vec3f),
				sizeof(math::vec4f),
				sizeof(mat43f),
				sizeof(mat32f)
			};
			PASSERT(m_type.value() < sizeof(size) / sizeof(u32));

			return size[m_type.value()] * m_count;
		}

		//////////////////////////////////////////////////////////////////////////

		inline std::size_t hash_value(const ShaderUniform& su)
		{
			std::size_t hash = 0;
			switch (su.m_type.value())
			{
			case ShaderUniform::Type::INT:
			{
				const int* data = (const int*)su.GetData();
				for (u32 i = 0; i < su.GetCount(); i++)
				{
					boost::hash_combine(hash, data[i]);
				}
				break;
			}
			default:
			{
				u32 count = su.GetDataSize() / sizeof(s32);
				//treat the floats like ints - faster hashing
				const s32* data = (const s32*)su.GetData();
				for (u32 i = 0; i < count; i++)
				{
					boost::hash_combine(hash, data[i]);
				}
			}
			break;
			}

			return hash;
		}

	}
}

*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
