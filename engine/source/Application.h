#ifndef SHADOW_APPLICATION_INCLUDE
#define SHADOW_APPLICATION_INCLUDE

#include "Globals.h"
#include "CreationParameters.h"

namespace sh
{

	class SHADOW_API Application
	{
	public:
		Application() {}
		Application(const Application& other) = delete;
		virtual ~Application() { }
		
		virtual void Init();
		virtual void Update(u64 delta) { }
		virtual void Destroy();

		void Run();
		void SetNeedsToBeClosed(bool needsToBeClosed) { m_needsToBeClosed = needsToBeClosed; }
		bool NeedsToBeClosed() const { return m_needsToBeClosed; }

	protected:
		bool m_needsToBeClosed = false;
	};

} // sh

#endif