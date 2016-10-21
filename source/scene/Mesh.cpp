#include "Mesh.h"

using namespace sh;
using namespace scene;

void Mesh::Init()
{
}

void Mesh::SetVerticesData(const void* data, u32 size)
{
	m_vertices.resize(size);
	std::copy(static_cast<const f32*>(data), static_cast<const f32*>(data) + size, m_vertices.begin());
}

void Mesh::SetIndicesData(const void* data, u32 size)
{
	m_indices.resize(size);
	std::copy(static_cast<const u16*>(data), static_cast<const u16*>(data) + size, m_indices.begin());
}