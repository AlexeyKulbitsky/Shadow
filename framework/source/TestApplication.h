#ifndef TEST_APP
#define TEST_APP

#include <Shadow.h>

class TestApp : public sh::Application
{
public:
	TestApp();
	TestApp(const sh::CreationParameters& params);
	virtual ~TestApp();
		
	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update(sh::u64 delta) override;
};

#endif