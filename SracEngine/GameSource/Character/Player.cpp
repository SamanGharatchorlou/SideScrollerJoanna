#include "pch.h"
#include "Player.h"

#include "ECS/Components.h"
#include "ECS/Components/Collider.h"
#include "ECS/EntityCoordinator.h"

#include "Graphics/TextureManager.h"
#include "Configs.h"
#include "System/Files/ConfigManager.h"
#include "PlayerStates/PlayerStates.h"

void Player::Init()
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;

	ECS::Entity entity = ecs->CreateEntity("Player");

	// Transform
	ECS::Transform& transform = ecs->AddComponent(Transform, entity);
	transform.baseRect = RectF(VectorF(200, 300), VectorF(100, 100));
	transform.targetCenterPosition = transform.baseRect.Center();
	
	// Sprite
	ECS::Sprite& sprite = ecs->AddComponent(Sprite, entity);
	sprite.texture = TextureManager::Get()->getTexture("PlayerMovementSheet", FileManager::Image_Animations);
	sprite.renderLayer = 5;
	
	// MovementPhysics
	ECS::Physics& physics = ecs->AddComponent(Physics, entity);
	physics.applyGravity = true;

	// Animation
	ECS::Animation& animation = ecs->AddComponent(Animation, entity);
	AnimationConfig* movement = ConfigManager::Get()->addAndLoad<AnimationConfig>("PlayerMovementAnimations");
	AnimationConfig* attacks = ConfigManager::Get()->addAndLoad<AnimationConfig>("PlayerAttackAnimations");
	animation.animator = Animator(movement);
	animation.animator.AddAnimations(attacks);
	
	// Collider
	ECS::Collider& collider = ecs->AddComponent(Collider, entity);
	collider.mRect = transform.baseRect;
	
	// PlayerController
	ECS::PlayerController& player_controller = ecs->AddComponent(PlayerController, entity);
	player_controller.entity = entity;

	std::vector<ActionState> actions;
	for( u32 i = 0; i < (u32)ActionState::Count; i++ )
	{
		actions.push_back((ActionState)i);
	}
	player_controller.statePool.load(actions, 4);

	PlayerState* idle_state = player_controller.statePool.getObject(ActionState::Idle);
	idle_state->SetBaseParameters(&player_controller, ActionState::Idle);
	player_controller.actions.Push(idle_state);

	// Velocity
	ECS::Velocity& velocity = ecs->AddComponent(Velocity, entity);
	velocity.acceleration = VectorF(40.0f, 40.0f);
	velocity.maxSpeed = VectorF(20.0f, 20.0f);
	velocity.maxAcceleration = VectorF(200.0f, 200.0f);
	
	// CharacterState
	ECS::CharacterState character_state = ecs->AddComponent(CharacterState, entity);
}