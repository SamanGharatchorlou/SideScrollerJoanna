#include "pch.h"
#include "ImGuiMenu.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "ThirdParty/imgui_sdl-master/imgui_sdl.h"

#include "Graphics/Renderer.h"
#include "System/Window.h"

#include "ImguiMainWindows.h"

#if IMGUI
namespace DebugMenu
{
	void Init()
	{
		SDL_Renderer* renderer = Renderer::Get()->sdlRenderer();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		ImGui_ImplSDL2_InitForSDLRenderer(GameData::Get().window->get(), renderer);
		ImGui_ImplSDLRenderer2_Init(renderer);

		// Initialize ImGuiSDL by calling Initialize with your SDL_Renderer, and with window size. This will also take care of setting up the ImGui viewport.
		ImGuiSDL::Initialize(renderer, 800, 600);
	}


	bool HandleInput(SDL_Event& event)
	{
		ImGui_ImplSDL2_ProcessEvent(&event);

		// consume game inputs when over gui window
		bool is_window_hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
		return is_window_hovered;
	}

	static bool s_entitySystemWindow = false;
	static bool s_componentWindow = false;
	static bool s_inputWindow = false;

	void Draw()
	{
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();


		ImGui::Begin("MainWindow", 0, ImGuiWindowFlags_MenuBar);
		ImGui::Checkbox("Entity System", &s_entitySystemWindow);
		ImGui::SameLine();
		ImGui::Checkbox("Components", &s_componentWindow);
		ImGui::SameLine();
		ImGui::Checkbox("Input", &s_inputWindow);
		ImGui::End();

		if (s_entitySystemWindow)
		{
			DoEntitySystemWindow();
		}

		if (s_componentWindow)
		{
			DoComponentWindow();
		}

		if (s_inputWindow)
		{
			DoInputWindow();
		}

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
	}
}
#endif