#include "pch.h"
#include "GameScreen.h"

#include "UI/UIManager.h"
#include "Input/InputManager.h"
#include "UI/ScreenLayers.h"
#include "Audio/AudioManager.h"

#include "UI/ScreenController.h"
#include "Game/GameController.h"

#include "UI/Elements/UIButton.h"
#include "UI/Elements/UISlider.h"

#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"

#if SET_GAME_SCALE
#include "Graphics/Renderer.h"
#endif


GameScreen::GameScreen()
{ 

}

void GameScreen::init()
{
	ScreenLayer layer("HotKeys");
	add(layer);

	//getSlider("HealthSlider")->disableInput();

	//UISlider* expSlider = getSlider("ExpSlider");
	//expSlider->disableInput();
	//expSlider->setSliderValue(0.0f);

	GameData::Get().uiManager->setCursorTexture(TextureManager::Get()->getTexture("GameCursor", FileManager::Image_UI));
	//GameData::Get().uiManager->controller()->openPopup("Introduction");

#if SET_GAME_SCALE
	Renderer::Get()->setScale(game_scale);
#endif
}

void GameScreen::handleInput(const InputManager* input)
{
	if (input->isReleased(Button::Pause) || input->isReleased(Button::Esc))
	{
		GameData::Get().uiManager->controller()->addScreen(UIScreen::Type::Pause);
		GameData::Get().systemStateManager->addState(SystemStates::PauseState);
	}
}


void GameScreen::slowUpdate()
{
	//UISlider* healthSlider = getSlider("HealthSlider");
	//if (healthSlider && healthSlider->getValue() == 0.0f)
	//{
	//	UIButton* icon = healthSlider->handle();
	//	Texture* texture = TextureManager::Get()->getTexture("DeadIcon", FileManager::Image_UI);
	//	icon->setTextures(texture, nullptr, nullptr);

	//	const float maxDim = icon->rect().Height();
	//	const VectorF newSize = texture->originalDimentions.realiseSize(maxDim);
	//	RectF newRect = icon->rect();
	//	newRect.SetSize(newSize);
	//	icon->setRect(newRect);
	//}
}


void GameScreen::setCursorTexture(Texture* texture)
{
	GameData::Get().uiManager->setCursorTexture(texture);
}


void GameScreen::resume()
{
	UIManager* ui = GameData::Get().uiManager;
	ui->controller()->setPersistingScreen(nullptr);
	ui->setCursorTexture(TextureManager::Get()->getTexture("GameCursor", FileManager::Image_UI));
}


void GameScreen::pause()
{
	exit();
}

void GameScreen::exit()
{
	UIManager* ui = GameData::Get().uiManager;
	ui->controller()->setPersistingScreen(this);
	ui->setCursorTexture(TextureManager::Get()->getTexture("UICursor", FileManager::Image_UI));
}