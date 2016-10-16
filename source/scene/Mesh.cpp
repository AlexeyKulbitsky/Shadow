#include "Mesh.h"

using namespace sh;
using namespace scene;

void Mesh::Init(const void* data, u32 size)
{
	m_vertices.resize(size);
	std::copy(static_cast<const float*>(data), static_cast<const float*>(data)+size, m_vertices.begin());


}