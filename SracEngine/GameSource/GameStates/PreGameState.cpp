 #include "pch.h"
#include "PreGameState.h"

#include "Audio/AudioManager.h"
#include "UI/UIManager.h"
#include "Graphics/TextureManager.h"
#include "Graphics/Renderer.h"
#include "UI/Screens/MainMenuScreen.h"

#include "GameSource/ECS/Components.h"
#include "ECS/EntityCoordinator.h"
#include "ECS/ComponentManager.h"
#include "ECS/SystemManager.h"
#include "GameSource/EntSystems/RenderSystem.h"
#include "GameSource/EntSystems/PlayerInputSystem.h"
#include "GameSource/EntSystems/MovementSystem.h"
#include "GameSource/EntSystems/AnimationSystem.h"
#include "ECS/EntityManager.h"
#include "Graphics/RenderManager.h"

#include "GameSource/Configs.h"
#include "System/Files/ConfigManager.h"

void PreGameState::init()
{	
	UIManager* UI = GameData::Get().uiManager;

	UI->controller()->clearScreenStack();
	UI->controller()->addScreen(UIScreen::Type::MainMenu);

	if(Texture* cursor_texture = TextureManager::Get()->getTexture("UICursor", FileManager::Image_UI))
		UI->setCursorTexture(cursor_texture);

	AudioManager* audio = AudioManager::Get();
	if(!audio->isPlaying("Menu", nullptr))
		audio->push(AudioEvent(AudioEvent::FadeInMusic, "Menu", nullptr, 1000));


	// ECS STUFF
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;

	ecs->RegisterComponent(Transform);
	ecs->RegisterComponent(Sprite);
	ecs->RegisterComponent(PlayerInput);
	ecs->RegisterComponent(CharacterState);
	ecs->RegisterComponent(Velocity);
	ecs->RegisterComponent(MovementPhysics);
	ecs->RegisterComponent(Animation);

	ECS::Archetype renderArchetype = ArcheBit(Transform) | ArcheBit(Sprite);
	ecs->RegisterSystem<ECS::RenderSystem>(renderArchetype);

	ECS::Archetype inputArchetype = ArcheBit(PlayerInput) | ArcheBit(CharacterState) | ArcheBit(Velocity);
	ecs->RegisterSystem<ECS::PlayerInputSystem>(inputArchetype);

	ECS::Archetype movementArchetype = ArcheBit(Transform) | ArcheBit(Velocity) | ArcheBit(CharacterState) | ArcheBit(MovementPhysics);
	ecs->RegisterSystem<ECS::MovementSystem>(movementArchetype);

	ECS::Archetype animationArchetype = ArcheBit(Sprite) | ArcheBit(Animation) | ArcheBit(CharacterState) | ArcheBit(Transform);
	ecs->RegisterSystem<ECS::AnimationSystem>(animationArchetype);


	ECS::Transform transform;
	transform.baseRect = RectF(VectorF(100,300), VectorF(100,100));

	ECS::Sprite sprite;
	sprite.texture = TextureManager::Get()->getTexture("PlayerMovementSheet", FileManager::Image_Animations);
	sprite.renderLayer = RenderPack::Layer::Highest;

	ECS::Velocity velocity;
	velocity.acceleration = VectorF(40.0f, 40.0f);
	velocity.maxSpeed = VectorF(2.0f, 2.0f);

	ECS::MovementPhysics physics;
	physics.physics.mDragFactor = 0.35f;

	ECS::Animation animation;
	AnimationConfig* movement = ConfigManager::Get()->addAndLoad<AnimationConfig>("PlayerMovementAnimations");
	AnimationConfig* attacks = ConfigManager::Get()->addAndLoad<AnimationConfig>("PlayerAttackAnimations");
	animation.animator = Animator(movement);
	animation.animator.AddAnimations(attacks);

	ECS::Entity entity = ecs->CreateEntity();

	ecs->AddComponent(Transform, entity, transform);
	ecs->AddComponent(Sprite, entity, sprite);
	ecs->AddComponent(PlayerInput, entity, ECS::PlayerInput());
	ecs->AddComponent(CharacterState, entity, ECS::CharacterState());
	ecs->AddComponent(Velocity, entity, velocity);
	ecs->AddComponent(MovementPhysics, entity, physics);
	ecs->AddComponent(Animation, entity, animation);



	int a = 4;
}

void PreGameState::slowUpdate(float dt) 
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ecs->UpdateSystems(dt);
}

void PreGameState::exit()
{
	AudioManager::Get()->push(AudioEvent(AudioEvent::FadeOut, "Menu", nullptr, 150));

	Screen* screen = GameData::Get().uiManager->getActiveScreen();
	MainMenuScreen* mainMenu = dynamic_cast<MainMenuScreen*>(screen);
	if (!mainMenu)
		DebugPrint(Error, "Selection screen is no active, cannot select a character");
}

