#ifndef GAME_MODULE_LAUNCHER_INCLUDE
#define GAME_MODULE_LAUNCHER_INCLUDE

#include <Shadow.h>

enum class GameModuleState
{
	Playing,
	Paused,
	Stopped
};

class GameModuleLauncher : public sh::Singleton<GameModuleLauncher>
{
public:
	GameModuleLauncher();

	void SetState(GameModuleState state);
	GameModuleState GetState() const;

private:
	GameModuleState m_state;
};

#endif
