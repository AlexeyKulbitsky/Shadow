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

	void SetPlayButton(const sh::gui::ButtonPtr& button);
	void SetPauseButton(const sh::gui::ButtonPtr& button);
	void SetStopButton(const sh::gui::ButtonPtr& button);

private:
	void OnButtonToggled(const sh::gui::ButtonPtr& sender);

private:
	GameModuleState m_state;

	sh::gui::ButtonPtr m_playButton;
	sh::gui::ButtonPtr m_pauseButton;
	sh::gui::ButtonPtr m_stopButton;
	sh::gui::ButtonGroupPtr m_buttonGroup;

	sh::Entity* m_cachedSelectedEntity = nullptr;
	sh::scene::Scene* m_cachedScene = nullptr;
};

#endif
