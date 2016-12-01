#ifndef SHADOW_SAMPLER_INCLUDE
#define SHADOW_SAMPLER_INCLUDE

#include "Texture.h"
#include <string>

namespace sh
{
	namespace video
	{
		class Sampler
		{
		public:
			Sampler(){}
			Sampler(Texture* texture){}
			virtual ~Sampler(){}

			virtual void Set(Texture* texture) = 0;
			virtual void Load(int textureChannel) = 0;
			virtual void Init() = 0;
			virtual Sampler* Clone() = 0;

			virtual void SetFiltering(Texture::Filtering filtering);
			virtual void SetTiling(Texture::Tiling tilingU, Texture::Tiling tilingV);
			virtual void SetType(Texture::Type type);
			virtual void SetName(const std::string& name);
			const std::string& GetName() const { return m_name; }

			Texture::Filtering GetFiltering() const { return m_filtering; }
			Texture::Tiling GetTilingU() const { return m_tilingU; }
			Texture::Tiling GetTilingV() const { return m_tilingV; }
			Texture::Type GetType() const { return m_type; }

		protected:
			Texture::Filtering m_filtering;
			Texture::Tiling m_tilingU;
			Texture::Tiling m_tilingV;
			Texture::Type m_type;
			std::string m_name;
		};

		inline void Sampler::SetFiltering(Texture::Filtering filtering)
		{
			m_filtering = filtering;
		}

		inline void Sampler::SetTiling(Texture::Tiling tilingU, Texture::Tiling tilingV)
		{
			m_tilingU = tilingU;
			m_tilingV = tilingV;
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
