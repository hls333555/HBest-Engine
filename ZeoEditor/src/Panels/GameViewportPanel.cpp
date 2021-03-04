#include "Panels/GameViewportPanel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ImGuizmo.h>

#include "Engine/GameFramework/Components.h"
#include "Dockspaces/EditorDockspace.h"
#include "Dockspaces/MainDockspace.h"
#include "Engine/Math/Math.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"
#include "Engine/Renderer/Renderer2D.h"

namespace ZeoEngine {

	void GameViewportPanel::OnAttach()
	{
		SceneViewportPanel::OnAttach();

		m_StopTexture = Texture2D::Create("assets/textures/Stop.png");
		m_ToolbarTextures[0] = m_PlayTexture->GetTexture();
		m_ToolbarTextures[1] = m_PauseTexture->GetTexture();
	}

	void GameViewportPanel::OnEvent(Event& e)
	{
		SceneViewportPanel::OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(ZE_BIND_EVENT_FUNC(GameViewportPanel::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(ZE_BIND_EVENT_FUNC(GameViewportPanel::OnMouseButtonPressed));
	}

	bool GameViewportPanel::OnKeyPressed(KeyPressedEvent& e)
	{
		if ((!IsPanelFocused() && !IsPanelHovered()) || e.GetRepeatCount() > 0) return false;

		bool bCanSwitchGizmo = !ImGuizmo::IsUsing();
		switch (e.GetKeyCode())
		{
			case Key::W:
			{
				if (bCanSwitchGizmo) m_GizmoType = ImGuizmo::TRANSLATE;
				break;
			}
			case Key::E:
			{
				if (bCanSwitchGizmo) m_GizmoType = ImGuizmo::ROTATE;
				break;
			}
			case Key::R:
			{
				if (bCanSwitchGizmo) m_GizmoType = ImGuizmo::SCALE;
				break;
			}
			case Key::Space:
			{
				if (bCanSwitchGizmo) m_GizmoType = m_GizmoType + 1 > ImGuizmo::SCALE ? ImGuizmo::TRANSLATE : m_GizmoType + 1;
				break;
			}
			default:
				return false;
		}

		return true;
	}

	bool GameViewportPanel::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (IsPanelHovered() && e.GetMouseButton() == Mouse::ButtonLeft && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
		{
			GetContext()->SetContextEntity(m_HoveredEntity);
		}

		return false;
	}

	void GameViewportPanel::ReadPixelDataFromIDBuffer(const Ref<FrameBuffer>& frameBuffer)
	{
		auto [mx, my] = GetMouseViewportPosition();
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		// Y coordinate for framebuffer texture is from bottom to up
		my = viewportSize.y - my;
		if (mx >= 0.0f && my >= 0.0f && mx < viewportSize.x && my < viewportSize.y)
		{
			int32_t pixel = frameBuffer->ReadPixel(1, static_cast<int32_t>(mx), static_cast<int32_t>(my));
			m_HoveredEntity = pixel == -1 ? Entity{} : Entity(static_cast<entt::entity>(pixel), GetScene().get());

			auto& Stats = Renderer2D::GetStats();
			Stats.HoveredEntity = m_HoveredEntity;
		}
	}

	void GameViewportPanel::RenderPanel()
	{
		SceneViewportPanel::RenderPanel();

		RenderGizmo();
	}

	void GameViewportPanel::RenderToolbar()
	{
		const float buttonSize = 32.0f;
		// Place buttons at window center
		ImGui::Indent(ImGui::GetContentRegionAvail().x * 0.5f - buttonSize - GImGui->Style.FramePadding.x * 3.0f/* Both two sides of button and SameLine() have spacing. */);
		// Toggle play / stop
		if (ImGui::ImageButton(m_ToolbarTextures[0], { buttonSize, buttonSize }, { 0.0f, 1.0f }, { 1.0f, 0.0f }))
		{
			ToggleStopTexture();
			//if (pieState == PIEState::None)
			//{
			//	StartPIE();
			//}
			//else
			//{
			//	StopPIE();
			//}
		}
		ImGui::SameLine();
		// Toggle pause / resume
		if (ImGui::ImageButton(m_ToolbarTextures[1], { buttonSize, buttonSize }, { 0.0f, 1.0f }, { 1.0f, 0.0f }))
		{
			ToggleResumeTexture();
			//if (pieState == PIEState::Running)
			//{
			//	PausePIE();
			//}
			//else if (pieState == PIEState::Paused)
			//{
			//	ResumePIE();
			//}
		}

	}

	void GameViewportPanel::ToggleStopTexture()
	{
		m_ToolbarTextures[0] = m_ToolbarTextures[0] == m_PlayTexture->GetTexture() ? m_StopTexture->GetTexture() : m_PlayTexture->GetTexture();
	}

	void GameViewportPanel::ToggleResumeTexture()
	{
		if (m_ToolbarTextures[0] == m_StopTexture->GetTexture())
		{
			m_ToolbarTextures[1] = m_ToolbarTextures[1] == m_PauseTexture->GetTexture() ? m_PlayTexture->GetTexture() : m_PauseTexture->GetTexture();
		}
	}

	void GameViewportPanel::RenderGizmo()
	{
		Entity selectedEntity = GetContext()->GetContextEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			const ImRect InnerRect = ImGui::GetCurrentWindow()->InnerRect;
			ImGuizmo::SetRect(InnerRect.Min.x, InnerRect.Min.y, InnerRect.GetSize().x, InnerRect.GetSize().y);

			// Editor camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Selected entity
			auto& transformComp = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 entityTransform = transformComp.GetTransform();

			// Snapping
			bool bSnap = Input::IsKeyPressed(Key::LeftControl);
			// Snap to 0.5m for translation/scale
			float snapValue = 0.5f;
			// Snap to 45 degrees for rotation
			if (m_GizmoType == ImGuizmo::ROTATE)
			{
				snapValue = 45.0f;
			}
			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				static_cast<ImGuizmo::OPERATION>(m_GizmoType), ImGuizmo::LOCAL,
				glm::value_ptr(entityTransform),
				nullptr, bSnap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 outTranslation, outRotation, outScale;
				Math::DecomposeTransform(entityTransform, outTranslation, outRotation, outScale);

				// This delta rotation prevents gimbal lock situation
				glm::vec3 deltaRotation = outRotation - transformComp.Rotation;
				transformComp.Translation = outTranslation;
				transformComp.Rotation += deltaRotation;
				transformComp.Scale = outScale;
			}
		}
	}

}