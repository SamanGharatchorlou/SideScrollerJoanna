#include "pch.h"
#include "RenderSystem.h"

#include "GameSource/ECS/Components.h"
#include "ECS/EntityCoordinator.h"
#include "Graphics/RenderManager.h"

#include "Debugging/ImGui/Components/ComponentDebugMenu.h"

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

			if(transform.flip != SDL_FLIP_NONE)
			{
				//int a = 4;
				//pack.rotation = 90;
			}

			pack.subRect = sprite.subRect;
			pack.flip = transform.flip;

			VectorF rect_center = transform.rect.Center();
			pack.flipPoint = rect_center - renderRect.TopLeft();
			GameData::Get().renderManager->AddRenderPacket(pack);

			if( DebugMenu::DisplayRenderRect(entity) )
			{
				DebugDraw::RectOutline(transform.rect, Colour::Red);
				DebugDraw::RectOutline(pack.rect, Colour::Green);
				DebugDraw::Point(pack.rect.TopLeft() + pack.flipPoint, 3.0f, Colour::Blue);
			}
		}
	}
}