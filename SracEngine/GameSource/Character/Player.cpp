#include "pch.h"
#include "Player.h"

#include "ECS/Components.h"
#include "ECS/Components/Collider.h"
#include "ECS/EntityCoordinator.h"

#include "Graphics/TextureManager.h"
#include "Configs.h"
#include "System/Files/ConfigManager.h"

void Player::Init()
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;

	ECS::Transform transform;
	transform.baseRect = RectF(VectorF(200, 300), VectorF(100, 100));
	transform.targetCenterPosition = transform.baseRect.Center();

	ECS::Sprite sprite;
	sprite.texture = TextureManager::Get()->getTexture("PlayerMovementSheet", FileManager::Image_Animations);
	sprite.renderLayer = 5;


	ECS::MovementPhysics physics;
	physics.physics.mDragFactor = 0.35f;
	physics.applyGravity = true;

	ECS::Animation animation;
	AnimationConfig* movement = ConfigManager::Get()->addAndLoad<AnimationConfig>("PlayerMovementAnimations");
	AnimationConfig* attacks = ConfigManager::Get()->addAndLoad<AnimationConfig>("PlayerAttackAnimations");
	animation.animator = Animator(movement);
	animation.animator.AddAnimations(attacks);

	ECS::Collider collider;
	collider.mRect = transform.baseRect;

	ECS::PlayerController player_controller;

	ECS::Velocity velocity;
	velocity.acceleration = VectorF(40.0f, 40.0f);
	velocity.maxSpeed = VectorF(20.0f, 20.0f);
	velocity.maxAcceleration = VectorF(200.0f, 200.0f);
	player_controller.velocity = velocity;


	ECS::Entity entity = ecs->CreateEntity("Player");

	ecs->AddComponent(Transform, entity, transform);
	ecs->AddComponent(Sprite, entity, sprite);
	ecs->AddComponent(PlayerController, entity, player_controller);
	ecs->AddComponent(MovementPhysics, entity, physics);
	ecs->AddComponent(Animation, entity, animation);
	ecs->AddComponent(Collider, entity, collider );
}