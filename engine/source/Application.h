#ifndef SHADOW_APPLICATION_INCLUDE
#define SHADOW_APPLICATION_INCLUDE

#include "Globals.h"
#include "CreationParameters.h"

namespace sh
{

	class Application
	{
	public:
		explicit Application(const CreationParameters& params);
		Application() = delete;
		Application(const Application& other) = delete;
		virtual ~Application() { }
		
		virtual void Init();
		virtual void Update(u64 delta) { }
		virtual void Destroy();

		void Run();
	};

} // sh

#endif