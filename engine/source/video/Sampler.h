#ifndef SHADOW_SAMPLER_INCLUDE
#define SHADOW_SAMPLER_INCLUDE

#include "../Globals.h"
#include "Texture.h"

namespace sh
{
	namespace video
	{
		class Sampler
		{
		public:
			enum class Usage
			{
				DIFFUSE_MAP,
				NORMAL_MAP,
				SPECULAR_MAP,
				DISPLACMENT_MAP,

				UNKNOWN
			};

			Sampler(){}
			Sampler(Texture* texture){}
			virtual ~Sampler(){}

			virtual void Set(TexturePtr texture){}
			virtual void Load(int textureChannel){}
			virtual void Init(){}
			virtual Sampler* Clone(){ return nullptr; }

			virtual void SetFiltering(TextureFiltering filtering);
			virtual void SetTiling(TextureTiling tilingU, TextureTiling tilingV, TextureTiling tilingW = TextureTiling::REPEAT);
			virtual void SetType(TextureType type);
			virtual void SetName(const std::string& name);
			void SetUsage(Usage usage) { m_usage = usage; }
			void SetTexureName(const String& name) { m_textureName = name; }

			TextureFiltering GetFiltering() const { return m_filtering; }
			TextureTiling GetTilingU() const { return m_tilingU; }
			TextureTiling GetTilingV() const { return m_tilingV; }
			TextureType GetType() const { return m_type; }
			const std::string& GetName() const { return m_name; }
			Usage GetUsage() const { return m_usage; }
			const String& GetTextureName() const { return m_textureName; }

		protected:
			TextureFiltering m_filtering;
			TextureTiling m_tilingU;
			TextureTiling m_tilingV;
			TextureTiling m_tilingW;
			TextureType m_type;
			Usage m_usage;
			String m_textureName;
			std::string m_name;
			bool m_hasMipMaps;
		};

		inline void Sampler::SetFiltering(TextureFiltering filtering)
		{
			m_filtering = filtering;
		}

		inline void Sampler::SetTiling(TextureTiling tilingU, TextureTiling tilingV, TextureTiling tilingW)
		{
			m_tilingU = tilingU;
			m_tilingV = tilingV;
			m_tilingW = tilingW;
		}

		inline void Sampler::SetType(TextureType type)
		{
			m_type = type;
		}

		inline void Sampler::SetName(const std::string& name)
		{
			m_name = name;
		}
	}
}

#endif
