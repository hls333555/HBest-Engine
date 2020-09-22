#pragma once

#include "Panels/EditorPanel.h"

namespace ZeoEngine {

	class SceneOutlinePanel : public ScenePanel
	{
	public:
		using ScenePanel::ScenePanel;

		virtual void OnImGuiRender() override;

	private:
		void DrawEntityNode(Entity entity);

	};

}
