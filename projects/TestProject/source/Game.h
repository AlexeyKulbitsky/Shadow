#ifndef GAME_INCLUDE
#define GAME_INCLUDE

#include <Shadow.h>

class Game : public sh::Application
{
public:
	Game();
	virtual ~Game();

	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update(sh::u64 delta) override;
};

#endif
