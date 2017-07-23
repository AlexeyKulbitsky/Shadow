#ifndef SHADOW_SCENE_NODE_INCLUDE
#define SHADOW_SCENE_NODE_INCLUDE

#include "../Globals.h"

namespace sh
{

namespace scene
{

	class Node
	{
	public:
		Node() {}
		virtual ~Node() {}
	protected:
		std::vector<Node*> m_children;
	};

} // scene

} // sh

#endif
