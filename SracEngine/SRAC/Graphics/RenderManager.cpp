#include "pch.h"
#include "Graphics/RenderManager.h"

#include "Texture.h"
#include "Renderable.h"
#include "Renderer.h"

#include "Debug/ImGui/ImGuiMenu.h"


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

	auto order_by_layer = [](const RenderPack& a, const RenderPack& b) { return a.layer < b.layer; };
	std::sort(mRenderPackets.begin(), mRenderPackets.end(), order_by_layer);

	const u32 render_count = mRenderPackets.size();

	int starting_indexes[RenderPack::Layer::Count];
	memset(starting_indexes, 0, sizeof(int) * RenderPack::Layer::Count);
	for (u32 layer = 0; layer < RenderPack::Layer::Count; layer++)
	{
		for (int i = 0; i < mRenderPackets.size(); )
		{
			// if the render layer is > pack layer then we've got out next starting point
			if (mRenderPackets[i].layer >= layer)
			{
				starting_indexes[layer] = i;
				layer++;
				continue;
			}

			i++;
		}
	}

	renderPacketRange(0, starting_indexes[1]);

	for (u32 layer = 1; layer < RenderPack::Layer::Count - 1; layer++)
	{
		u32 start = starting_indexes[layer];
		u32 end = starting_indexes[layer + 1];

		renderPacketRange(start, end);
	}

	renderPacketRange(starting_indexes[RenderPack::Layer::Highest], mRenderPackets.size());

	mRenderPackets.clear();

#if IMGUI
	DebugMenu::Draw();
#endif

	// update window surface
	SDL_RenderPresent(renderer);
}

void RenderManager::renderPacketRange(int start, int end)
{
	for (u32 i = start; i < end; i++)
	{
		if(mRenderPackets[i].subRect.isValid())
			mRenderPackets[i].texture->renderSubTexture(mRenderPackets[i].rect, mRenderPackets[i].subRect, mRenderPackets[i].flip);
		else
			mRenderPackets[i].texture->render(mRenderPackets[i].rect, mRenderPackets[i].flip);
	}
}


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