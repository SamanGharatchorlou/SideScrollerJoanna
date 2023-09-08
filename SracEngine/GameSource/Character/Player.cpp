#include "pch.h"
#include "Player.h"

#include "ECS/Components.h"
#include "ECS/EntityCoordinator.h"

#include "Graphics/TextureManager.h"
#include "Configs.h"
#include "System/Files/ConfigManager.h"
#include "Graphics/RenderManager.h"

void Player::Init()
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;

	ECS::Transform transform;
	transform.baseRect = RectF(VectorF(100, 300), VectorF(100, 100));

	ECS::Sprite sprite;
	sprite.texture = TextureManager::Get()->getTexture("PlayerMovementSheet", FileManager::Image_Animations);
	sprite.renderLayer = 5;

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

	ECS::Entity entity = ecs->CreateEntity("Player");

	ecs->AddComponent(Transform, entity, transform);
	ecs->AddComponent(Sprite, entity, sprite);
	ecs->AddComponent(PlayerInput, entity, ECS::PlayerInput());
	ecs->AddComponent(CharacterState, entity, ECS::CharacterState());
	ecs->AddComponent(Velocity, entity, velocity);
	ecs->AddComponent(MovementPhysics, entity, physics);
	ecs->AddComponent(Animation, entity, animation);

}