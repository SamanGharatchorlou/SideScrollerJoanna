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
			Transform& transform = ecs->GetComponent(Transform, entity);
			Sprite& sprite = ecs->GetComponent(Sprite, entity);

			RectF renderRect = transform.baseRect;
			if (!transform.sizeFactor.isZero())
			{
				VectorF center = renderRect.Center();
				renderRect.SetSize(renderRect.Size() * transform.sizeFactor);
				renderRect.SetCenter(center);
			}

			RenderPack pack(sprite.texture, renderRect, sprite.renderLayer);
			pack.subRect = sprite.subRect;
			pack.flip = transform.flip;
			GameData::Get().renderManager->AddRenderPacket(pack);
		}
	}
}