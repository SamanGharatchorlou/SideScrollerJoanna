#include "pch.h"
#include "GameState.h"

#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include "Game/Camera/Camera.h"
#include "UI/UIManager.h"

#include "ECS/EntityCoordinator.h"

#include "GameSource/Character/Player.h"
#include "GameSource/ECS/ComponentsSetup.h"

#include "Scene/SceneParsing/SceneBuilder.h"

#include "Configs.h"

void GameState::init()
{
	ECS::RegisterAllComponents();
	ECS::RegisterAllSystems();

	player = new Player;
	player->Init();


	UIManager* ui = GameData::Get().uiManager;
	ui->controller()->replaceScreen(UIScreen::Type::Game);

	SceneBuilder::ReadScene("firstexport");


	initCamera();

	// Start Audio
	AudioManager* audio = AudioManager::Get();
	audio->push(AudioEvent(AudioEvent::FadeInMusic, "Game", nullptr, 1500));
}


void GameState::handleInput()
{

}


void GameState::fastUpdate(float dt)
{
	Camera::Get()->fastUpdate(dt);
}


void GameState::slowUpdate(float dt)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ecs->UpdateSystems(dt);

	Camera::Get()->slowUpdate(dt);

	Cursor* cursor = GameData::Get().inputManager->getCursor();
	cursor->mode();
}

void GameState::resume() 
{
	//mGameData->environment->resume();
	AudioManager::Get()->push(AudioEvent(AudioEvent::FadeInMusic, "Game", nullptr, 750));
}

void GameState::pause()
{
	//mGameData->environment->pause();
	AudioManager::Get()->push(AudioEvent(AudioEvent::FadeOut, "Game", nullptr, 150));
}


void GameState::exit()
{
	//mGameData->environment->clear();
	//mGameData->scoreManager->reset();
	AudioManager::Get()->push(AudioEvent(AudioEvent::FadeOut, "Game", nullptr, 150));

	delete player;
}


// --- Private Functions --- //

void GameState::initCamera()
{
	Camera* camera = Camera::Get();

	VectorF cameraPosition = VectorF(0.0f, 0.0f);
	camera->setPosition(cameraPosition);

	// TODO: fix these values
	camera->initShakeyCam(100.0f, 80.0f);

	//RectF* playerRect = &mGameData->environment->actors()->player()->get()->rectRef();
	//camera->follow(playerRect);
}
