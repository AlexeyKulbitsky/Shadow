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

			virtual void SetFiltering(Texture::Filtering filtering);
			virtual void SetTiling(Texture::Tiling tilingU, Texture::Tiling tilingV, Texture::Tiling tilingW = Texture::Tiling::REPEAT);
			virtual void SetType(Texture::Type type);
			virtual void SetName(const std::string& name);
			void SetUsage(Usage usage) { m_usage = usage; }
			void SetTexureName(const String& name) { m_textureName = name; }

			Texture::Filtering GetFiltering() const { return m_filtering; }
			Texture::Tiling GetTilingU() const { return m_tilingU; }
			Texture::Tiling GetTilingV() const { return m_tilingV; }
			Texture::Type GetType() const { return m_type; }
			const std::string& GetName() const { return m_name; }
			Usage GetUsage() const { return m_usage; }
			const String& GetTextureName() const { return m_textureName; }

		protected:
			Texture::Filtering m_filtering;
			Texture::Tiling m_tilingU;
			Texture::Tiling m_tilingV;
			Texture::Tiling m_tilingW;
			Texture::Type m_type;
			Usage m_usage;
			String m_textureName;
			std::string m_name;
			bool m_hasMipMaps;
		};

		inline void Sampler::SetFiltering(Texture::Filtering filtering)
		{
			m_filtering = filtering;
		}

		inline void Sampler::SetTiling(Texture::Tiling tilingU, Texture::Tiling tilingV, Texture::Tiling tilingW)
		{
			m_tilingU = tilingU;
			m_tilingV = tilingV;
			m_tilingW = tilingW;
		}

		inline void Sampler::SetType(Texture::Type type)
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
