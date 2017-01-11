#ifndef SHADOW_SCALE_GIZMO_INCLUDE
#define SHADOW_SCALE_GIZMO_INCLUDE

#include "Gizmo.h"

class ScaleGizmo : public Gizmo
{
public:
	ScaleGizmo();
	virtual ~ScaleGizmo();
	virtual void Render();
};

#endif