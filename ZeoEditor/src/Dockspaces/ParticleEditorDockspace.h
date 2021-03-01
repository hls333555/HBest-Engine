#pragma once

#include "Dockspaces/EditorDockspace.h"

namespace ZeoEngine {

	class ParticleEditorDockspace : public EditorDockspace
	{
	public:
		using EditorDockspace::EditorDockspace;

		virtual void OnAttach() override;

		virtual EditorPanelType GetViewportPanelType() const override { return EditorPanelType::Particle_View; }

		virtual void CreateNewScene(bool bIsFromOpenScene = false) override;

	private:
		virtual AssetType GetAssetType() const override { return AssetType::ParticleTemplate; }
		virtual void Serialize(const std::string& filePath) override;
		virtual void Deserialize(const std::string& filePath) override;

		virtual void BuildDockWindows(ImGuiID dockspaceID) override;

	};

}
