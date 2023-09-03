#include "pch.h"
#include "GameState.h"

#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include "Game/Camera/Camera.h"


void GameState::init()
{
	initCamera();

	// Start Audio
	AudioManager* audio = AudioManager::Get();

	audio->push(AudioEvent(AudioEvent::FadeInMusic, "Game", nullptr, 1500));
}


void GameState::handleInput()
{
	//mGameData->environment->handleInput(mGameData->inputManager);

	//int movement = mGameData->network->handleNetworkInput();
	//if (movement > -1)
	//{
	//	PlayerManager* player = mGameData->environment->actors()->player();

	//	VectorF position = player->position();

	//	float x = 0.0f;
	//	float y = 0.0f;
	//	float speed = 1.0f;

	//	if (movement == 1)
	//	{
	//		x = -speed;
	//	}

	//	if (movement == 2)
	//	{
	//		x = speed;
	//	}

	//	if (movement == 3)
	//	{
	//		y = -speed;
	//	}

	//	if (movement == 4)
	//	{
	//		y = speed;
	//	}

	//	position += VectorF(x, y);

	//	player->setPosition(position);
	//}



}


void GameState::fastUpdate(float dt)
{
	Camera::Get()->fastUpdate(dt);
	//mGameData->environment->fastUpdate(dt);
}


void GameState::slowUpdate(float dt)
{
	Camera::Get()->slowUpdate(dt);
	//mGameData->environment->slowUpdate(dt);
	//mGameData->scoreManager->slowUpdate();

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


//void GameState::initRendering()
//{
//	//mGameData->renderManager->Set(mGameData->environment);
//	//mGameData->renderManager->Set(mGameData->uiManager);
//}
