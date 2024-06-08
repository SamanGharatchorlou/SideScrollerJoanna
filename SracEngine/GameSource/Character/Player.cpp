#include "pch.h"
#include "Player.h"

#include "ECS/Components.h"
#include "ECS/Components/Collider.h"
#include "ECS/EntityCoordinator.h"

#include "Graphics/TextureManager.h"
#include "Configs.h"
#include "System/Files/ConfigManager.h"
#include "PlayerStates/PlayerStates.h"
#include "ECS/Components/PlayerController.h"
#include "ECS/Components/Physics.h"

void Player::Init()
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;

	ECS::Entity entity = ecs->CreateEntity("Player");

	// Transform
	ECS::Transform& transform = ecs->AddComponent(Transform, entity);
	transform.rect = RectF(VectorF(300, 300), VectorF(64, 64));
	transform.targetCenterPosition = transform.rect.Center();
	
	// MovementPhysics
	ECS::Physics& physics = ecs->AddComponent(Physics, entity);
	physics.applyGravity = false;	
	physics.acceleration = VectorF(100.0f, 100.0f);
	physics.maxSpeed = VectorF(5.0f, 5.0f);

	// Animation
	ECS::Animation& animation = ecs->AddComponent(Animation, entity);
	AnimationConfig* up = ConfigManager::Get()->addAndLoad<AnimationConfig>("BloodHeroUpAnimations");
	AnimationConfig* down = ConfigManager::Get()->addAndLoad<AnimationConfig>("BloodHeroDownAnimations");
	AnimationConfig* rl = ConfigManager::Get()->addAndLoad<AnimationConfig>("BloodHeroRLAnimations");
	
	animation.animator.AddAnimations(up);
	animation.animator.AddAnimations(down);
	animation.animator.AddAnimations(rl);
	animation.animator.start();

	// Sprite
	ECS::Sprite& sprite = ecs->AddComponent(Sprite, entity);
	sprite.texture = TextureManager::Get()->getTexture("blood_hero", FileManager::Image_Animations);
	sprite.renderLayer = 9;

	const SpriteSheet& ss = up->animations.front().spriteSheet;
	VectorF pos = ss.objectPos / ss.frameSize;
	VectorF size = ss.frameSize / ss.objectSize;
	sprite.relativeRenderRect = RectF(pos, size);
	
	// Collider
	ECS::Collider& collider = ecs->AddComponent(Collider, entity);
	collider.mRect = transform.rect;
	
	// PlayerController
	ECS::PlayerController& player_controller = ecs->AddComponent(PlayerController, entity);
	player_controller.entity = entity;

	std::vector<ActionState> actions;
	for( u32 i = 0; i < (u32)ActionState::Count; i++ )
	{
		actions.push_back((ActionState)i);
	}
	player_controller.statePool.load(actions, 4);
	
	// CharacterState
	ECS::CharacterState& character_state = ecs->AddComponent(CharacterState, entity);
	character_state.facingDirection = VectorI(0,1); // facing down
}