#pragma once

#include <string>
#include <unordered_map>

#include "Engine/Core/Log.h"
#include "Engine/GameFramework/Entity.h"
#include "Engine/GameFramework/Scene.h"
#include "Engine/Renderer/Buffer.h"
#include "Menus/EditorMenu.h"
#include "Panels/EditorPanel.h"
#include "Engine/Events/Event.h"
#include "Engine/ImGui/MyImGui.h"
#include "Utils/EditorUtils.h"
#include "Engine/Utils/PlatformUtils.h"

namespace ZeoEngine {

	class EditorLayer;

	class EditorDockspace
	{
		friend class DockspaceManager;
		friend class ScenePanel;
		friend class MainDockspace;

	public:
		EditorDockspace() = delete;
		EditorDockspace(EditorWindowType dockspaceType, EditorLayer* context, bool bDefaultShow = false,
			const glm::vec2& dockspacePadding = glm::vec2(0.0f, 0.0f),
			ImGuiWindowFlags dockspaceWindowFlags = ImGuiWindowFlags_MenuBar,
			ImVec2Data initialSize = ImVec2Data::DefaultSize, ImVec2Data initialPos = ImVec2Data::DefaultPos);

	public:
		virtual void OnAttach();
		virtual void OnDetach() {}
		void OnUpdate(DeltaTime dt);
		void OnImGuiRender();
		void OnEvent(Event& e);

		const std::string& GetDockspaceName() const { return m_DockspaceName; }
		bool* GetShowPtr() { return &m_bShow; }
		Entity GetContextEntity() const { return m_ContextEntity; }
		void SetContextEntity(Entity newEntity) { m_ContextEntity = newEntity; }
		const Ref<Scene>& GetScene() const { return m_Scene; }
		const Ref<FrameBuffer>& GetFrameBuffer() const { return m_FBO; }

	protected:
		void PushDockspace(EditorDockspace* dockspace);
	public:
		void PushMenu(EditorMenu* menu);
		void PushPanel(EditorPanel* panel);
		EditorPanel* GetPanelByType(EditorWindowType panelType);

		EditorDockspace* OpenEditor(EditorWindowType dockspaceType);

		/** Create an empty scene and init camera. */
		virtual void CreateNewScene();
		void OpenScene();
		void SaveScene();
		void SaveSceneAs();

	private:
		void CreateScene();

		void CreateFrameBuffer();
		void BeginFrameBuffer();
		void EndFrameBuffer();

		void RenderDockspace();
		virtual void BuildDockWindows(ImGuiID dockspaceID) = 0;

	protected:
		bool m_bIsMainDockspace{ false };
		AssetType m_SerializeAssetType{ AssetType::NONE };
		Entity m_ContextEntity;
	private:
		entt::sigh<void()> m_OnSceneCreateDel;
	protected:
		entt::sink<void()> m_OnSceneCreate{ m_OnSceneCreateDel };

	private:
		std::string m_DockspaceName;
		ImVec2Data m_InitialPos, m_InitialSize;
		glm::vec2 m_DockspacePadding;
		ImGuiWindowFlags m_DockspaceWindowFlags;
		EditorLayer* m_EditorContext;
		bool m_bShow;

		Ref<Scene> m_Scene;
		Ref<FrameBuffer> m_FBO;
		MenuManager m_MenuManager;
		PanelManager m_PanelManager;

		bool m_bShouldRebuildDockLayout{ false };
	};

	class DockspaceManager
	{
	public:
		DockspaceManager() = default;
		~DockspaceManager();

		void OnUpdate(DeltaTime dt);
		void OnImGuiRender();
		void OnEvent(Event& e);

		void PushDockspace(EditorDockspace* dockspace);
		EditorDockspace* GetDockspaceByName(const std::string& dockspaceName);

		void RebuildDockLayout(const std::string& dockspaceName);

	private:
		std::unordered_map<std::string, EditorDockspace*> m_Dockspaces;
	};

}
