#ifndef SHADOW_OBJECT_INCLUDE
#define SHADOW_OBJECT_INCLUDE

#define SHADOW_OBJECT(TypeName)					                    \
public:																\
using ClassType = TypeName;											\
static const char* GetTypeNameStatic() { return #TypeName; }		\
virtual const char* GetTypeName() override { return GetTypeNameStatic(); }

#endif
