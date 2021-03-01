#pragma once

#include "Panels/EditorPanel.h"

namespace ZeoEngine {

	class PreferencesPanel : public EditorPanel
	{
	public:
		using EditorPanel::EditorPanel;

	private:
		virtual void RenderPanel() override;

	};

}
