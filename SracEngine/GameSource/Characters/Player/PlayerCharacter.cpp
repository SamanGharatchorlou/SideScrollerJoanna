#include "pch.h"
#include "PlayerCharacter.h"

#include "ECS/Components/Components.h"
#include "ECS/Components/Collider.h"
#include "ECS/EntityCoordinator.h"

#include "Graphics/TextureManager.h"
#include "Configs.h"
#include "System/Files/ConfigManager.h"
#include "CharacterStates/PlayerStates.h"
#include "ECS/Components/PlayerController.h"
#include "ECS/Components/Physics.h"

#include "Animations/AnimationReader.h"

ECS::Entity Player::Create()
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;

	ECS::Entity entity = ecs->CreateEntity("Player");

	// Transform
	ECS::Transform& transform = ecs->AddComponent(Transform, entity);
	transform.rect.SetSize(VectorF(64, 64));
	
	// MovementPhysics
	ECS::Physics& physics = ecs->AddComponent(Physics, entity);
	physics.applyGravity = false;	
	physics.acceleration = VectorF(100.0f, 100.0f);
	physics.maxSpeed = VectorF(5.0f, 5.0f);

	// Animation
	ECS::Animation& animation = ecs->AddComponent(Animation, entity);
	AnimationReader::Parse( "BloodHeroAnimations", animation.animator );
	animation.animator.start();

	// Sprite
	ECS::Sprite& sprite = ecs->AddComponent(Sprite, entity);
	sprite.renderLayer = 9;
	
	// Collider
	ECS::Collider& collider = ecs->AddComponent(Collider, entity);
	collider.mRect = transform.rect;
	
	// PlayerController
	ECS::PlayerController& player_controller = ecs->AddComponent(PlayerController, entity);

	std::vector<ActionState> actions;
	for( u32 i = 0; i < (u32)ActionState::Count; i++ )
	{
		actions.push_back((ActionState)i);
	}
	player_controller.statePool.load(actions, 4);
	
	// CharacterState
	ECS::CharacterState& character_state = ecs->AddComponent(CharacterState, entity);
	character_state.facingDirection = VectorI(0,1); // facing down
	
	// Health
	ECS::Health& health = ecs->AddComponent(Health, entity);
	health.maxHealth = 100;
	health.currentHealth = 100;

	return entity;
}