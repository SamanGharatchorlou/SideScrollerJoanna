#include "pch.h"
#include "CharacterAction.h"

#include "ECS/EntityCoordinator.h"
#include "ECS/Components/Components.h"
#include "ECS/Components/Collider.h"


ECS::Entity CreateAttackCollider(ECS::Entity entity, const RectF& collider_rect, float damage, const char* entity_name)
{
	ECS::EntityCoordinator* ecs = GameData::Get().ecs;
	ECS::Entity attack_collider = ecs->CreateEntity(entity_name);

	// Transform
	ECS::Transform& attack_transform = ecs->AddComponent(Transform, attack_collider);
	attack_transform.rect = collider_rect;
	attack_transform.targetCenterPosition = collider_rect.Center(); 

	// Collider
	ECS::Collider& collider = ecs->AddComponent(Collider, attack_collider);
	collider.mRect = collider_rect;

	// Damage
	ECS::Damage& damage_comp = ecs->AddComponent(Damage, attack_collider);
	damage_comp.value = damage;
		
	// dont damage our self
	ECS::Health& health = ecs->GetComponentRef(Health, entity);
	health.ignoredDamaged.push_back(attack_collider);

	return attack_collider;
}