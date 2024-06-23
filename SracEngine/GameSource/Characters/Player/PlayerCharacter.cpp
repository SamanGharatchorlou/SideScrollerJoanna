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

ECS::Entity s_playerEntity = ECS::EntityInvalid;

ECS::Entity Player::Get()
{
	return s_playerEntity;
}

ECS::Entity Player::Create()
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;

	s_playerEntity = ecs->CreateEntity("Player");

	// Transform
	ECS::Transform& transform = ecs->AddComponent(Transform, s_playerEntity);
	
	// MovementPhysics
	ECS::Physics& physics = ecs->AddComponent(Physics, s_playerEntity);
	physics.applyGravity = false;	
	physics.acceleration = VectorF(100.0f, 100.0f);
	physics.maxSpeed = VectorF(5.0f, 5.0f);

	// Animation
	ECS::Animation& animation = ecs->AddComponent(Animation, s_playerEntity);
	AnimationReader::Parse( "BloodHeroAnimations", animation.animator );
	animation.animator.start();

	// Sprite
	ECS::Sprite& sprite = ecs->AddComponent(Sprite, s_playerEntity);
	sprite.renderLayer = 9;
	
	// Collider
	ECS::Collider& collider = ecs->AddComponent(Collider, s_playerEntity);
	SpriteSheet& ss = animation.animator.mSpriteSheets.front();
	collider.SetRect(RectF(VectorF::zero(), ss.colliderSize));
	
	// PlayerController
	ECS::PlayerController& player_controller = ecs->AddComponent(PlayerController, s_playerEntity);

	std::vector<ActionState> actions;
	for( u32 i = 0; i < (u32)ActionState::Count; i++ )
	{
		actions.push_back((ActionState)i);
	}
	player_controller.statePool.load(actions, 4);
	
	// CharacterState
	ECS::CharacterState& character_state = ecs->AddComponent(CharacterState, s_playerEntity);
	character_state.facingDirection = VectorI(0,1); // facing down
	
	// Health
	ECS::Health& health = ecs->AddComponent(Health, s_playerEntity);
	health.maxHealth = 100;
	health.currentHealth = 100;

	return s_playerEntity;
}