#include "pch.h"
#include "ImGuiMainWindows.h"

#include "ThirdParty/imgui-master/imgui.h"
#include "ImGuiHelpers.h"

#include "ECS/EntityManager.h"
#include "ECS/EntityCoordinator.h"
#include "Graphics/RenderManager.h"
#include "Input/InputManager.h"

#include "Debugging/ImGui/Components/ComponentDebugMenu.h"

ECS::Entity s_selectedEntity = 0;
u32 DebugMenu::GetSelectedEntity() { return s_selectedEntity; }

// Entity Window
void DebugMenu::DoEntitySystemWindow()
{
    ImGui::Begin("Entity Window", nullptr, ImGuiWindowFlags_MenuBar);
    
    ECS::EntityCoordinator* ecs = GameData::Get().ecs;
    ECS::EntityManager& em = ecs->entities;
    std::unordered_map<ECS::Entity, StringBuffer32>& entityNames = em.entityNames;

    const char* selected = entityNames.count(s_selectedEntity) > 0 ? entityNames[s_selectedEntity].c_str() : "No selection";

    ImGui::Text("Selected Entity: %d", (int)s_selectedEntity);

    if (ImGui::BeginCombo("Entities", selected, 0))
    {
        for (auto iter = entityNames.begin(); iter != entityNames.end(); iter++)
        {
            const bool is_selected = iter->first == s_selectedEntity;

            if (ImGui::Selectable(iter->second.c_str(), is_selected))
                s_selectedEntity = iter->first;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    if (GameData::Get().ecs->IsAlive(s_selectedEntity)) 
    {
        if (ecs->IsAlive(s_selectedEntity))
        {
            ECS::Archetype type = 0;
            SetFlag<u64>(type, ECS::archetypeBit(DoTransformDebugMenu(s_selectedEntity)));
            SetFlag<u64>(type, ECS::archetypeBit(DoAnimationDebugMenu(s_selectedEntity)));
            SetFlag<u64>(type, ECS::archetypeBit(DoTileMapDebugMenu(s_selectedEntity)));
            SetFlag<u64>(type, ECS::archetypeBit(DoColliderDebugMenu(s_selectedEntity)));
            SetFlag<u64>(type, ECS::archetypeBit(DoCharacterStateDebugMenu(s_selectedEntity)));
            SetFlag<u64>(type, ECS::archetypeBit(DoPhysicsDebugMenu(s_selectedEntity)));
            SetFlag<u64>(type, ECS::archetypeBit(DoPlayerControllerDebugMenu(s_selectedEntity)));
            SetFlag<u64>(type, ECS::archetypeBit(DoSpriteDebugMenu(s_selectedEntity)));

            ECS::Archetype entity_type = ecs->entities.GetAchetype(s_selectedEntity);
            for (u32 i = 0; i < ECS::Component::Count; i++) 
            {
                if(entity_type & ECS::archetypeBit((ECS::Component::Type)i))
                {
                    if(type & ECS::archetypeBit((ECS::Component::Type)i))
                        continue;

		            ImGui::CollapsingHeader(ECS::ComponentNames[i]);
                }

            }
        }
    }

    ImGui::End();
}

void DebugMenu::DoInputWindow()
{
    ImGui::Begin("Input Window", nullptr, ImGuiWindowFlags_MenuBar);

    if (ImGui::CollapsingHeader("Inputs & Focus"))
    {
        ImGuiIO& io = ImGui::GetIO();

        // Display inputs submitted to ImGuiIO
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode("ImGui Inputs"))
        {
            if (ImGui::IsMousePosValid())
                ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
            else
                ImGui::Text("Mouse pos: <INVALID>");
            ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
            ImGui::Text("Mouse down:");
            for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDown(i)) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
            ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);

            // We iterate both legacy native range and named ImGuiKey ranges, which is a little odd but this allows displaying the data for old/new backends.
            // User code should never have to go through such hoops! You can generally iterate between ImGuiKey_NamedKey_BEGIN and ImGuiKey_NamedKey_END.
#ifdef IMGUI_DISABLE_OBSOLETE_KEYIO
            struct funcs { static bool IsLegacyNativeDupe(ImGuiKey) { return false; } };
            ImGuiKey start_key = ImGuiKey_NamedKey_BEGIN;
#else
            struct funcs { static bool IsLegacyNativeDupe(ImGuiKey key) { return key < 512 && ImGui::GetIO().KeyMap[key] != -1; } }; // Hide Native<>ImGuiKey duplicates when both exists in the array
            ImGuiKey start_key = (ImGuiKey)0;
#endif
            ImGui::Text("Keys down:");         for (ImGuiKey key = start_key; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1)) { if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyDown(key)) continue; ImGui::SameLine(); ImGui::Text((key < ImGuiKey_NamedKey_BEGIN) ? "\"%s\"" : "\"%s\" %d", ImGui::GetKeyName(key), key); }
            ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
            ImGui::Text("Chars queue:");       for (int i = 0; i < io.InputQueueCharacters.Size; i++) { ImWchar c = io.InputQueueCharacters[i]; ImGui::SameLine();  ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); } // FIXME: We should convert 'c' to UTF-8 here but the functions are not public.

            ImGui::TreePop();
        }
        if (ImGui::TreeNode("SDL Inputs"))
        {
            InputManager* im = InputManager::Get();

            StringBuffer32 cursorState = "None";
            if (im->mCursor.isPressed())
                cursorState = "Pressed";
            if (im->mCursor.isReleased())
                cursorState = "Released";
            else if (im->mCursor.isHeld())
                cursorState = "Held";

            ImGui::Text("Cursor %s Moving: ", im->mCursor.isMoving() ? "is" : "is not");
            ImGui::Text("Cursor: %s", cursorState.c_str());

            for (u32 i = 0; i < im->mButtons.size(); i++)
            {
                Button& button = im->mButtons[i];

                StringBuffer32 state;

                if (button.isPressed())
                    state = "Pressed, ";
                if (button.isHeld())
                    state = state + "Held, ";
                if (button.isReleased())
                    state = state + "Released";

                ImGui::PushID(i);
                if (!state.empty())
                {
                    ImGui::Text("Button %d: %s || Held Frames %d", i, state.c_str(), button.getHeldFrames());
                }
                ImGui::PopID();
            }

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

static bool s_displayStatics = true;
static bool s_displayDynamics = true;
static DebugDrawType s_drawType = DebugDrawType::RectOutline;

void DebugMenu::DoColliderWindow() 
{
    ECS::EntityCoordinator* ecs = GameData::Get().ecs;

	ECS::ComponentArray<ECS::Collider>& colliders =  ecs->GetComponents<ECS::Collider>(ECS::Component::Type::Collider);
	std::vector<ECS::Collider>& collider_list = colliders.components;

    ImGui::Checkbox("Display Statics", &s_displayStatics);
    ImGui::Checkbox("Display Dynamics", &s_displayDynamics);

    ImGui::DoDebugRenderTypeDropDown(s_drawType);

    // first we need to update the collider position with where the entity wants to be
    for(u32 i = 0; i < collider_list.size(); i++)
	{
        const ECS::Collider& collider = collider_list[i];

		// ignore static colliders, they dont move
        bool is_static = HasFlag(collider.mFlags, ECS::Collider::Flags::Static);
		if(!s_displayStatics && is_static)
            continue;
        if(!s_displayDynamics && !is_static)
            continue;

        DebugDraw::Shape(s_drawType, collider.mRect, Colour::Blue);
	}
}