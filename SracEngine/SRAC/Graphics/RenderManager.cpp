#include "pch.h"
#include "Graphics/RenderManager.h"

#include "Texture.h"
#include "Renderable.h"
#include "Renderer.h"


#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "System/Window.h"

//#include "ThirdParty/imgui-master/imgui.h"
#include "ThirdParty/imgui_sdl-master/imgui_sdl.h"

#include "Game/Data/LoadingManager.h"

RenderManager::RenderManager() { }

void RenderManager::Init()
{
	SDL_Renderer* renderer = Renderer::Get()->sdlRenderer();
	

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGui::SetCurrentContext(ctx);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(GameData::Get().window->get(), renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);

	//ImGui::CreateContext();
	//ImGuiContext* ctx = ImGui::CreateContext();

	// Initialize ImGuiSDL by calling Initialize with your SDL_Renderer, and with window size. This will also take care of setting up the ImGui viewport.
	ImGuiSDL::Initialize(renderer, 800, 600);
}

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
	//if (LoadingManager::Get() && LoadingManager::Get()->isLoadingAssets())
		//return;
	//if(GameData::Get().l)

	SDL_Renderer* renderer = Renderer::Get()->sdlRenderer();

	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	//bool show_demo_window = true;
	//bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	//if (show_demo_window)
	//	ImGui::ShowDemoWindow(&show_demo_window);


	//// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	//{
	//	static float f = 0.0f;
	//	static int counter = 0;

	//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	//	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	//	ImGui::Checkbox("Another Window", &show_another_window);

	//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	//		counter++;
	//	ImGui::SameLine();
	//	ImGui::Text("counter = %d", counter);

	//	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	//	ImGui::End();
	//}








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

	//ImGui::ShowDemoWindow();

	ImGui::Begin("Image");
	bool anything = true;
	ImGui::Checkbox("my check box", &anything);
	ImGui::End();

	// Rendering
	ImGui::Render();
	//SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
	//SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
	//SDL_RenderClear(renderer);
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(renderer);

	//ImGui::Render();
	//ImGuiSDL::Render(ImGui::GetDrawData());

	//// update window surface
	//SDL_RenderPresent(renderer);
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