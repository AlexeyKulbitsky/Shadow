#include "GameModuleLauncher.h"

#include "selection/SelectionManager.h"

GameModuleLauncher::GameModuleLauncher()
	: m_state(GameModuleState::Stopped)
{
	m_buttonGroup.reset(new sh::gui::ButtonGroup());
	m_buttonGroup->buttonToggled.Connect(std::bind(&GameModuleLauncher::OnButtonToggled, this, std::placeholders::_1));
}

void GameModuleLauncher::SetState(GameModuleState state)
{

	switch (state)
	{
	case GameModuleState::Playing:
	{
		auto sceneManager = sh::Device::GetInstance()->GetSceneManager();
		m_cachedSelectedEntity = SelectionManager::GetInstance()->GetSelectedEntity();
		SelectionManager::GetInstance()->SetSelectedEntity(nullptr);

		if (m_state == GameModuleState::Stopped)
		{
			// Cache scene
			if (m_cachedScene)
			{
				m_cachedScene->Clear();
				delete m_cachedScene;
			}
			m_cachedScene = sceneManager->GetCurrentScene();
			sh::scene::Scene* scene = m_cachedScene->Clone();
			sceneManager->SetCurrentScene(scene);
		}

		sceneManager->SetSystemsState(sh::SystemState::Running);
	}
		break;
	case GameModuleState::Paused:
	{
		sh::Device::GetInstance()->GetSceneManager()->SetSystemsState(sh::SystemState::Paused);
	}
		break;
	case GameModuleState::Stopped:
	{
		auto sceneManager = sh::Device::GetInstance()->GetSceneManager();

		if (m_state == GameModuleState::Playing)
		{
			// Restore cache
			auto scene = sceneManager->GetCurrentScene();
			
			if (scene)
			{
				scene->Clear();
				delete scene;
			}

			sceneManager->SetCurrentScene(m_cachedScene);
			m_cachedScene = nullptr;
		}
		sceneManager->SetSystemsState(sh::SystemState::Stopped);
		SelectionManager::GetInstance()->SetSelectedEntity(m_cachedSelectedEntity);
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

void GameModuleLauncher::SetPlayButton(const sh::gui::ButtonPtr& button)
{
	m_playButton = button;
	m_buttonGroup->AddButton(button);
}

void GameModuleLauncher::SetPauseButton(const sh::gui::ButtonPtr& button)
{
	m_pauseButton = button;
	m_buttonGroup->AddButton(button);
}

void GameModuleLauncher::SetStopButton(const sh::gui::ButtonPtr& button)
{
	m_stopButton = button;
	m_buttonGroup->AddButton(button);
}

void GameModuleLauncher::OnButtonToggled(const sh::gui::ButtonPtr& sender)
{
	if (sender == m_playButton)
	{
		SetState(GameModuleState::Playing);
	}
	else if (sender == m_pauseButton)
	{
		SetState(GameModuleState::Paused);
	}
	else if (sender == m_stopButton)
	{
		SetState(GameModuleState::Stopped);
	}
}

