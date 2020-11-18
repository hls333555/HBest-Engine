#pragma once

#include "Dockspaces/EditorDockspace.h"

namespace ZeoEngine {

	class ParticleEditorDockspace : public EditorDockspace
	{
	public:
		using EditorDockspace::EditorDockspace;

		virtual void OnAttach() override;

		virtual EditorWindowType GetViewportPanelType() const override { return EditorWindowType::Particle_View; }

	private:
		virtual AssetType GetAssetType() const override { return AssetType::ParticleTemplate; }
		virtual void Serialize(const std::string& filePath) override;
		virtual void Deserialize(const std::string& filePath) override;

		virtual void BuildDockWindows(ImGuiID dockspaceID) override;

	};

}
