#include "GameModuleLauncher.h"

GameModuleLauncher::GameModuleLauncher()
	: m_state(GameModuleState::Stopped)
{

}

void GameModuleLauncher::SetState(GameModuleState state)
{

	switch (state)
	{
	case GameModuleState::Playing:
	{
		if (m_state == GameModuleState::Stopped)
		{
			// Cache scene
		}
	}
		break;
	case GameModuleState::Paused:
	{
	}
		break;
	case GameModuleState::Stopped:
	{
	}
		break;
	default:
		break;
	}

	m_state = state;
}

GameModuleState GameModuleLauncher::GetState() const
{
	return m_state;
}
