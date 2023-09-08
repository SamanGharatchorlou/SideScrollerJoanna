#include "pch.h"
#include "Graphics/RenderManager.h"

#include "Texture.h"
#include "Renderable.h"
#include "Renderer.h"

#include "Debugging/ImGui/ImGuiMenu.h"


RenderManager::RenderManager() { }

void RenderManager::addRenderable(Renderable* renderable)
{
	mRenderables.push_back(renderable);
}

void RenderManager::removeRenderable(Renderable* renderable)
{
	for (auto iter = mRenderables.begin(); iter != mRenderables.end(); iter++)
	{
		if (*iter == renderable)
		{
			mRenderables.erase(iter);
			return;
		}
	}
}

void RenderManager::AddDebugRenderPacker(const DebugRenderPack& renderPack)
{
	mDebugRenders.push_back(renderPack);
}

void RenderManager::render()
{
	SDL_Renderer* renderer = Renderer::Get()->sdlRenderer();

	// clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	// todo: remove
	for (uint32_t i = 0; i < mRenderables.size(); i++)
	{
		mRenderables[i]->render();
	}

	//auto order_by_layer = [](const RenderPack& a, const RenderPack& b) { return a.layer < b.layer; };
	//std::sort(mRenderPackets.begin(), mRenderPackets.end(), order_by_layer);

	//const u32 render_count = mRenderPackets.size();

	//int starting_indexes[RenderPack::Layer::Count];
	//memset(starting_indexes, 0, sizeof(int) * RenderPack::Layer::Count);
	//for (u32 layer = 0; layer < RenderPack::Layer::Count; layer++)
	//{
	//	for (int i = 0; i < mRenderPackets.size(); )
	//	{
	//		// if the render layer is > pack layer then we've got out next starting point
	//		if (mRenderPackets[i].layer >= layer)
	//		{
	//			starting_indexes[layer] = i;
	//			layer++;
	//			continue;
	//		}

	//		i++;
	//	}
	//}

	//renderPacketRange(0, starting_indexes[1]);

	//for (u32 layer = 1; layer < RenderPack::Layer::Count - 1; layer++)
	//{
	//	u32 start = starting_indexes[layer];
	//	u32 end = starting_indexes[layer + 1];

	//	renderPacketRange(start, end);
	//}

	//renderPacketRange(starting_indexes[RenderPack::Layer::Highest], mRenderPackets.size());

	for (u32 i = 0; i < c_RenderLayers; i++)
	{
		std::vector<RenderPack>& render_packs = mRenderPackets[i];
		for (u32 i = 0; i < render_packs.size(); i++)
		{
			if (render_packs[i].subRect.isValid())
				render_packs[i].texture->renderSubTexture(render_packs[i].rect, render_packs[i].subRect, render_packs[i].flip);
			else
				render_packs[i].texture->render(render_packs[i].rect, render_packs[i].flip);
		}

		render_packs.clear();
	}

	//mRenderPackets.clear();
	// 
	// always debug draw last
	for (u32 i = 0; i < mDebugRenders.size(); i++)
	{
		switch (mDebugRenders[i].type)
		{
		case DebugRenderPack::Line:
		{
			Colour colour = mDebugRenders[i].colour;
			SDL_SetRenderDrawColor(Renderer::Get()->sdlRenderer(), colour.r, colour.g, colour.b, colour.a);

			//Vector2D<int> A = Camera::Get()->toCameraCoords(pointA).toInt();
			//Vector2D<int> B = Camera::Get()->toCameraCoords(pointB).toInt();
			RectF& rect = mDebugRenders[i].rect;

			Vector2D<int> A = rect.TopLeft().toInt();
			Vector2D<int> B = rect.BotRight().toInt();

			SDL_RenderDrawLine(Renderer::Get()->sdlRenderer(), A.x, A.y, B.x, B.y);
			break;
		}
		case DebugRenderPack::RectOutline:
		{
			Colour colour = mDebugRenders[i].colour;
			SDL_SetRenderDrawColor(Renderer::Get()->sdlRenderer(), colour.r, colour.g, colour.b, colour.a);

			RectF& rect = mDebugRenders[i].rect;
			SDL_Rect renderQuadb = { static_cast<int>(rect.x1),
				static_cast<int>(rect.y1),
				static_cast<int>(rect.Width()),
				static_cast<int>(rect.Height()) };

			SDL_RenderDrawRect(Renderer::Get()->sdlRenderer(), &renderQuadb);
			break;
		}
		case DebugRenderPack::Point:
		case DebugRenderPack::RectFill:
		{
			break;
		}
		default:
			break;
		}
	}

#if IMGUI
	DebugMenu::Draw();
#endif

	// update window surface
	SDL_RenderPresent(renderer);
}

//void RenderManager::renderPacketRange(int start, int end)
//{
//	for (u32 i = start; i < end; i++)
//	{
//		if(mRenderPackets[i].subRect.isValid())
//			mRenderPackets[i].texture->renderSubTexture(mRenderPackets[i].rect, mRenderPackets[i].subRect, mRenderPackets[i].flip);
//		else
//			mRenderPackets[i].texture->render(mRenderPackets[i].rect, mRenderPackets[i].flip);
//	}
//}


//void RenderManager::renderPackets(RenderLayer layer)
//{
//	for (int i = 0; i < mRenderPackets.size(); i++)
//	{
//		if(mRenderPackets[i].layer == layer)	
//		{
//			mRenderPackets[i].texture->render(mRenderPackets[i].rect);
//		}
//	}
//}


//void RenderManager::handleEvent(EventData& data)
//{
//	switch (data.eventType)
//	{
//		case Event::Render:
//		{
//			RenderEvent eventData = static_cast<RenderEvent&>(data);
//
//			RenderPack renderPacket(eventData.mTexture, eventData.mRect, static_cast<RenderLayer>(eventData.mRenderLayer));
//			AddRenderPacket(renderPacket);
//			break;
//		}
//	}
//}