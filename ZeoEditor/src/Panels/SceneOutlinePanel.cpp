#include "Panels/SceneOutlinePanel.h"

#include <imgui.h>
#include <IconsFontAwesome5.h>

#include "Engine/GameFramework/Components.h"
#include "Editors/EditorBase.h"
#include "Engine/Core/KeyCodes.h"

namespace ZeoEngine {

	void SceneOutlinePanel::ProcessRender()
	{
		m_Filter.Draw("##SceneOutlineAssetFilter", "Search entities", -1.0f);
		
		ImGui::Separator();

		EditorBase* owningEditor = GetOwningEditor();

		// Display entities in creation order, the order is updated when a new entity is created or destroyed
		owningEditor->GetScene()->m_Registry.view<CoreComponent>().each([this](auto entityId, auto& cc)
		{
			Entity entity{ entityId, GetOwningEditor()->GetScene().get() };
			DrawEntityNode(entity);
		});

		// Deselect entity when blank space is clicked
		// NOTE: We cannot use IsPanelHovered() here or entities can never be selected on mouse click
		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
		{
			owningEditor->SetContextEntity({});
		}

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindowWithPadding(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem(ICON_FA_BULLSEYE "  Create Empty Entity"))
			{
				auto selectedEntity = owningEditor->GetScene()->CreateEntity("New Entity");
				owningEditor->SetContextEntity(selectedEntity);
			}

			ImGui::EndPopup();
		}
	}

	void SceneOutlinePanel::DrawEntityNode(Entity entity)
	{
		auto& coreComp = entity.GetComponent<CoreComponent>();
		if (coreComp.bIsInternal) return;

		const char* entityName = coreComp.Name.c_str();
		if (!m_Filter.IsActive() || m_Filter.IsActive() && m_Filter.PassFilter(entityName))
		{
			EditorBase* owningEditor = GetOwningEditor();
			auto selectedEntity = owningEditor->GetContextEntity();
			ImGuiTreeNodeFlags flags = (selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
			bool bIsTreeExpanded = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, entityName);
			if (ImGui::IsItemClicked())
			{
				owningEditor->SetContextEntity(entity);
			}
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
			{
				owningEditor->FocusContextEntity();
			}

			bool bIsCurrentEntitySelected = selectedEntity == entity;
			bool bWillDestroyEntity = false;
			// NOTE: We cannot use Input::IsKeyReleased() here as it will return true even if key has not been pressed yet
			if (IsPanelFocused() && bIsCurrentEntitySelected && ImGui::IsKeyReleased(Key::Delete))
			{
				bWillDestroyEntity = true;
			}

			if (bIsTreeExpanded)
			{
				// TODO: Display child entities

				ImGui::TreePop();
			}

			// Defer entity destruction
			if (bWillDestroyEntity)
			{
				owningEditor->GetScene()->DestroyEntity(entity);
				if (bIsCurrentEntitySelected)
				{
					owningEditor->SetContextEntity({});
				}
			}
		}
		
	}

}
