#pragma once

#include "Engine/GameFramework/Entity.h"

#include "Engine/Events/Event.h"

namespace ZeoEngine {

	class ScriptableEntity
	{
		friend class Scene;

	public:
		virtual ~ScriptableEntity() = default;

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(DeltaTime dt) {}
		virtual void OnEvent(Event& e) {}

	private:
		Entity m_Entity;

	};

}
