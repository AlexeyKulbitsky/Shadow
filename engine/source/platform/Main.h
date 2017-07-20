#ifndef SHADOW_MAIN_INCLUDE
#define SHADOW_MAIN_INCLUDE

#if defined (SHADOW_WINDOWS)

#define SHADOW_MAIN_FUNCTION(ApplicationFunction)												\
int main()																						\
{																								\
	return ApplicationFunction;																	\
} 

#elif defined (SHADOW_ANDROID)

#define SHADOW_MAIN_FUNCTION(ApplicationFunction)													\
void AndroidMainThreadFunction()																\
{																								\
	ApplicationFunction;																		\
}



#endif

///////////////////////////////////////////////////////////////////////////////

#define SHADOW_APPLICATION_DECLARATION(ApplicationType)											\
int ApplicationFunction()																		\
{																								\
	sh::Device *device = sh::CreateDevice();													\
	ApplicationType* application = new ApplicationType();										\
	device->SetApplication(application);														\
	device->Init();																				\
	device->Run();																				\
	device->Destroy();																			\
	delete application;																			\
	return 0;																					\
}																								\
SHADOW_MAIN_FUNCTION(ApplicationFunction())

#endif
