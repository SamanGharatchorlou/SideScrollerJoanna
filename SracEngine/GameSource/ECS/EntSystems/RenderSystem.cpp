#include "pch.h"
#include "RenderSystem.h"

#include "GameSource/ECS/Components.h"
#include "ECS/EntityCoordinator.h"
#include "Graphics/RenderManager.h"

namespace ECS
{
	void RenderSystem::Update(float dt)
	{
		EntityCoordinator* ecs = GameData::Get().ecs;

 		for (Entity entity : entities)
		{
			Transform& transform = ecs->GetComponentRef(Transform, entity);
			Sprite& sprite = ecs->GetComponentRef(Sprite, entity);

			VectorF size = transform.rect.Size() * sprite.relativeRenderRect.Size();
			VectorF pos = transform.rect.TopLeft() - sprite.relativeRenderRect.TopLeft() * size;
			RectF renderRect(pos, size);

			RenderPack pack(sprite.texture, renderRect, sprite.renderLayer);
			pack.subRect = sprite.subRect;
			pack.flip = transform.flip;
			GameData::Get().renderManager->AddRenderPacket(pack);
		}
	}
}