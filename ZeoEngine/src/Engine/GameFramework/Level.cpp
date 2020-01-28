#include "ZEpch.h"
#include "Engine/GameFramework/Level.h"

#include <imgui.h>

#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/Renderer2D.h"

namespace ZeoEngine {

	Level::~Level()
	{
		for (auto* object : m_GameObjects)
		{
			delete object;
		}
	}

	void Level::Init()
	{
		// Default background
		m_backgroundTexture = Texture2D::Create("assets/textures/Checkerboard_Alpha.png");

	}

	void Level::OnUpdate(DeltaTime dt)
	{
		for (uint32_t i = 0; i < m_GameObjects.size(); ++i)
		{
			if (!m_GameObjects[i]->IsActive())
				continue;

			// Collision detection
			if (m_GameObjects[i]->IsCollisionEnabled() && m_GameObjects[i]->ShouldGenerateOverlapEvent())
			{
				m_GameObjects[i]->DoCollisionTest(m_GameObjects);
			}

			// Out of range error may occur
			if (i < m_GameObjects.size())
			{
				m_GameObjects[i]->OnUpdate(dt);
			}
		}

		m_ParticleManager.OnUpdate(dt);

	}

	void Level::OnRender()
	{
		Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 100.0f, 100.0f }, m_backgroundTexture, { 50.0f, 50.0f });

		for (uint32_t i = 0; i < m_GameObjects.size(); ++i)
		{
			if (!m_GameObjects[i]->IsActive())
				continue;

			if (!m_GameObjects[i]->IsTranslucent())
			{
				// Render opaque objects first
				m_GameObjects[i]->OnRender();
			}
		}

		RenderCommand::EnableDepthWriting(false);
		// Then render translucent objects
		// NOTE: m_TranslucentObjects is not updated every frame
		// so you must make sure all translucent object's zPositions will never change at runtime!
		for (const auto& objectPair : m_TranslucentObjects)
		{
			if (!objectPair.second->IsActive())
				continue;

			objectPair.second->OnRender();
		}
		m_ParticleManager.OnRender();
		RenderCommand::EnableDepthWriting(true);

	}

	void Level::OnImGuiRender()
	{
		for (uint32_t i = 0; i < m_GameObjects.size(); ++i)
		{
			if (m_GameObjects[i]->IsActive())
			{
				m_GameObjects[i]->OnImGuiRender();
			}
		}

		// TODO: This will eventually be in EditorLayer
		ImGui::Begin("Level Outline");
		ImGui::Text("(%d objects total)", m_GameObjects.size());
		for (uint32_t i = 0; i < m_GameObjects.size(); ++i)
		{
			ImVec4 color;
			if (m_GameObjects[i]->IsActive())
			{
				// Translucent objects are marked yellow instead of white
				color = m_GameObjects[i]->IsTranslucent() ? ImVec4(1.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{
				// Inactive objects are marked darker
				color = m_GameObjects[i]->IsTranslucent() ? ImVec4(0.75f, 0.75f, 0.0f, 1.0f) : ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
			}
			ImGui::TextColored(color, "%s", m_GameObjects[i]->GetName().c_str());
		}
		ImGui::End();

		ImGui::Begin("Engine Stats");
		ImGui::Text("%.f FPS (%.2f ms)", ImGui::GetIO().Framerate, 1000.f / ImGui::GetIO().Framerate);
		ImGui::End();

	}

	void Level::DestroyGameObject(GameObject* object)
	{
		auto it = std::find(m_GameObjects.begin(), m_GameObjects.end(), object);
		if (it != m_GameObjects.end())
		{
			m_GameObjects.erase(it);
		}

		auto it2 = std::find_if(m_TranslucentObjects.begin(), m_TranslucentObjects.end(), [&object](const std::pair<TranslucentObjectData, GameObject*>& objectPair) {
			return objectPair.second == object;
			});
		if (it2 != m_TranslucentObjects.end())
		{
			m_TranslucentObjects.erase(it2);
		}

		delete object;
	}

	ParticleSystem* Level::SpawnParticleSystem(const ParticleTemplate& particleTemplate, GameObject* attachToParent, bool bAutoDestroy)
	{
		ParticleSystem* ps = new ParticleSystem(particleTemplate, attachToParent, bAutoDestroy);
		m_ParticleManager.AddParticleSystem(ps);
		return ps;
	}

}