#include "Panels/AboutPanel.h"

#include "Engine/ImGui/MyImGui.h"

namespace ZeoEngine {

	void AboutPanel::OnAttach()
	{
		PanelBase::OnAttach();

		SetFlags(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize);
		SetInitialSize({ 300.0f, 200.0f });

		m_LogoTexture = Texture2D::Create("resources/textures/Logo_96x.png");
	}

	void AboutPanel::ProcessRender()
	{
		ImGui::TextCentered("ZeoEngine 0.1");
		ImGui::TextCentered("Created by SanSan");
		ImGui::TextCentered("https://github.com/hls333555/");
		const float logoSize = ImGui::GetStyle().Alpha * 96.0f;
		// Center the logo
		ImGui::Indent((ImGui::GetContentRegionAvail().x - logoSize) * 0.5f);
		ImGui::Image(m_LogoTexture->GetTextureID(),
			{ logoSize, logoSize },
			{ 0.0f, 1.0f }, { 1.0f, 0.0f });
	}

}
