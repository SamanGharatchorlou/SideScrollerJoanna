#include "pch.h"
#include "Enemies.h"

#include "ECS/Components/Components.h"
#include "ECS/Components/Collider.h"
#include "ECS/EntityCoordinator.h"

#include "Graphics/TextureManager.h"
#include "Configs.h"
#include "System/Files/ConfigManager.h"
#include "CharacterStates/PlayerStates.h"
#include "ECS/Components/Physics.h"

#include "Animations/AnimationReader.h"

ECS::Entity Enemy::Create()
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;

	ECS::Entity entity = ecs->CreateEntity("Enemy");

	// Transform
	ECS::Transform& transform = ecs->AddComponent(Transform, entity);
	transform.rect.SetSize(VectorF(17, 34));
	
	// MovementPhysics
	ECS::Physics& physics = ecs->AddComponent(Physics, entity);
	physics.applyGravity = false;	
	physics.acceleration = VectorF(100.0f, 100.0f);
	physics.maxSpeed = VectorF(5.0f, 5.0f);
	physics.speed = VectorF(0,2);

	// Animation
	ECS::Animation& animation = ecs->AddComponent(Animation, entity);
	AnimationReader::Parse( "TribeWarriorAnimations", animation.animator );
	animation.animator.start();

	// Sprite
	ECS::Sprite& sprite = ecs->AddComponent(Sprite, entity);
	sprite.renderLayer = 9;
	
	// Collider
	ECS::Collider& collider = ecs->AddComponent(Collider, entity);
	collider.mRect = transform.rect;
	
	// PlayerController
	//ECS::PlayerController& player_controller = ecs->AddComponent(PlayerController, entity);
	//player_controller.entity = entity;

	//std::vector<ActionState> actions;
	//for( u32 i = 0; i < (u32)ActionState::Count; i++ )
	//{
	//	actions.push_back((ActionState)i);
	//}
	//player_controller.statePool.load(actions, 4);

	// AI Controller
	ECS::AIController& ai_controller = ecs->AddComponent(AIController, entity);
	ai_controller.entity = entity;
	
	// CharacterState
	ECS::CharacterState& character_state = ecs->AddComponent(CharacterState, entity);
	character_state.facingDirection = VectorI(0,1); // facing down

	ecs->AddComponent(Pathing, entity);

	return entity;
}
