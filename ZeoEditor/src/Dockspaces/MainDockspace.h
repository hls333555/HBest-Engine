#pragma once

#include "Dockspaces/EditorDockspace.h"

#include "Engine/GameFramework/Entity.h"

namespace ZeoEngine {

	class MainDockspace : public EditorDockspace
	{
		friend class SceneOutlinePanel;
		friend class EntityInspectorPanel;

	public:
		using EditorDockspace::EditorDockspace;

		virtual void OnAttach() override;

	private:
		virtual void BuildDockWindows(ImGuiID dockspaceID) override;

	private:
		Entity m_SelectedEntity;

	};

}
