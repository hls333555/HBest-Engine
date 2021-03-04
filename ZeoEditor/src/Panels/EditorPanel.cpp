#include "Panels/EditorPanel.h"

#include "Dockspaces/EditorDockspace.h"

namespace ZeoEngine {

	EditorPanel::EditorPanel(EditorPanelType panelType, bool bDefaultShow, ImGuiWindowFlags panelWindowFlags, const ImVec2Data& initialSize, const ImVec2Data& initialPos)
		: m_PanelType(panelType), m_bShow(bDefaultShow), m_PanelWindowFlags(panelWindowFlags)
		, m_InitialSize(initialSize), m_InitialPos(initialPos)
	{
	}

	void EditorPanel::OnImGuiRender()
	{
		if (!IsShow()) return;

		if (m_InitialPos == ImVec2Data::DefaultPos)
		{
			ImGuiViewport* mainViewport = ImGui::GetMainViewport();
			ImVec2 CenterPos{ mainViewport->Size.x / 2.0f, mainViewport->Size.y / 2.0f };
			ImGui::SetNextWindowPos(CenterPos, m_InitialPos.Condition, ImVec2(0.5f, 0.5f));
		}
		else
		{
			ImGui::SetNextWindowPos(m_InitialPos.Data, m_InitialPos.Condition);
		}
		ImGui::SetNextWindowSize(m_InitialSize.Data, m_InitialSize.Condition);

		if (ImGui::Begin(GetPanelName().c_str(), &m_bShow, m_PanelWindowFlags))
		{
			m_bIsPanelFocused = ImGui::IsWindowFocused();
			m_bIsPanelHovered = ImGui::IsWindowHovered();

			RenderPanel();
		}

		ImGui::End();
	}

	ScenePanel::ScenePanel(EditorPanelType panelType, EditorDockspace* context, bool bDefaultShow, ImGuiWindowFlags panelWindowFlags, const ImVec2Data& initialSize, const ImVec2Data& initialPos)
		: EditorPanel(panelType, bDefaultShow, panelWindowFlags, initialSize, initialPos)
		, m_Context(context)
	{
	}

	bool ScenePanel::IsShow() const
	{
		return m_Context->m_bShow && EditorPanel::IsShow();
	}

	const Ref<Scene>& ScenePanel::GetScene() const
	{
		return m_Context->GetScene();
	}

	const Ref<FrameBuffer>& ScenePanel::GetFrameBuffer() const
	{
		return m_Context->GetFrameBuffer();
	}

	PanelManager::~PanelManager()
	{
		for (auto& [name, panel] : m_Panels)
		{
			panel->OnDetach();
			delete panel;
		}
	}

	void PanelManager::OnUpdate(DeltaTime dt)
	{
		for (auto& [name, panel] : m_Panels)
		{
			panel->OnUpdate(dt);
		}
	}

	void PanelManager::OnImGuiRender()
	{
		for (auto& [name, panel] : m_Panels)
		{
			panel->OnImGuiRender();
		}
	}

	void PanelManager::OnEvent(Event& e)
	{
		for (auto& [name, panel] : m_Panels)
		{
			panel->OnEvent(e);
		}
	}

	void PanelManager::PushPanel(EditorPanel* panel)
	{
		m_Panels.emplace(panel->GetPanelName(), panel);
		panel->OnAttach();
	}

	EditorPanel* PanelManager::GetPanelByName(const std::string& panelName)
	{
		auto result = m_Panels.find(panelName);
		return result == m_Panels.end() ? nullptr : result->second;
	}

}