#include "pch.h"
#include "GameState.h"

#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include "Game/Camera/Camera.h"
#include "UI/UIManager.h"

#include "ECS/Components/Components.h"
#include "ECS/EntityCoordinator.h"

#include "Characters/Spawner.h"

#include "ECS/Components/ComponentsSetup.h"

#include "Scene/SceneParsing/SceneBuilder.h"

#include "Configs.h"

void GameState::Init()
{
	ECS::RegisterAllComponents();
	ECS::RegisterAllSystems();

	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Entity entity = ecs->CreateEntity("Map");

	ECS::TileMap& tile_map = ecs->AddComponent(TileMap, entity);
	Map::SceneBuilder::BuildTileMap("blood_test_export.xml", tile_map.tileMap);

	PlayerSpawn::Spawn(tile_map);
	EnemySpawn::Spawn(tile_map);

	UIManager* ui = GameData::Get().uiManager;
	ui->controller()->replaceScreen(UIScreen::Type::Game);

	initCamera();

	// Start Audio
	AudioManager* audio = AudioManager::Get();
	audio->push(AudioEvent(AudioEvent::FadeInMusic, "Game", nullptr, 1500));
}


void GameState::HandleInput()
{

}


void GameState::FastUpdate(float dt)
{
	Camera::Get()->fastUpdate(dt);
}


void GameState::Update(float dt)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ecs->UpdateSystems(dt);

	Camera::Get()->Update(dt);

	Cursor* cursor = GameData::Get().inputManager->getCursor();
	cursor->mode();
}

void GameState::Resume() 
{
	//mGameData->environment->resume();
	AudioManager::Get()->push(AudioEvent(AudioEvent::FadeInMusic, "Game", nullptr, 750));
}

void GameState::Pause()
{
	//mGameData->environment->pause();
	AudioManager::Get()->push(AudioEvent(AudioEvent::FadeOut, "Game", nullptr, 150));
}


void GameState::Exit()
{
	//mGameData->environment->clear();
	//mGameData->scoreManager->reset();
	AudioManager::Get()->push(AudioEvent(AudioEvent::FadeOut, "Game", nullptr, 150));

	//delete player;
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
