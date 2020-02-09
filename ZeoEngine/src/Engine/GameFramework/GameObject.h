#pragma once

#include <glm/glm.hpp>

#include <rttr/type>
#include <rttr/registration>
#include <rttr/registration_friend>

#include "Engine/Core/DeltaTime.h"

namespace ZeoEngine {

#define WORLD_UP_VECTOR glm::vec2{ 0.0f, 1.0f }
#define WORLD_RIGHT_VECTOR glm::vec2{ 1.0f, 0.0f }

	struct Transform
	{
		Transform() = default;
		Transform(const glm::vec3& _position)
			: position(_position)
		{}

		glm::vec3 position{ 0.0f };
		float rotation = 0.0f;
		glm::vec2 scale{ 1.0f };

		RTTR_ENABLE()
	};

	enum class ObjectCollisionType
	{
		None,
		Box,
		Sphere,
	};

	// TODO: Support collision primitives which can be added to objects arbitrarily
	struct CollisionData
	{
		friend class GameObject;

	public:
		virtual void UpdateData() = 0;

		glm::vec2 Center{ 0.0f };
		GameObject* OwnerObject;

	protected:
		glm::vec2 CenterOffset;

		RTTR_ENABLE()
		RTTR_REGISTRATION_FRIEND
	};

	struct BoxCollisionData : public CollisionData
	{
		BoxCollisionData(GameObject* ownerObject, const glm::vec2& centerOffset = { 0.0f, 0.0f }, const glm::vec2& extents = { 0.0f, 0.0f })
			: Extents(extents)
		{
			OwnerObject = ownerObject;
			CenterOffset = centerOffset;
			UpdateData();
		}

		inline virtual void UpdateData() override;

		glm::vec2 Extents;

		RTTR_ENABLE(CollisionData)
	};

	struct SphereCollisionData : public CollisionData
	{
		SphereCollisionData(GameObject* ownerObject, const glm::vec2& centerOffset = { 0.0f, 0.0f }, float radius = 0.0f)
			: Radius(radius)
		{
			OwnerObject = ownerObject;
			CenterOffset = centerOffset;
			UpdateData();
		}

		inline virtual void UpdateData() override;

		float Radius;

		RTTR_ENABLE(CollisionData)
	};

#define Super __super

#define RTTR_SPAWN(className) \
public: static className* SpawnGameObject(const glm::vec3& position)\
		{\
			return ZeoEngine::Level::Get().SpawnGameObject<className>(position);\
		}
#define RTTR_REGISTER(className, ...) RTTR_ENABLE(__VA_ARGS__) RTTR_SPAWN(className)

	enum class ClassMeta
	{
		/** This class is abstract and cannot be instanced to the level */
		Abstract,
		/** Pop up a tooltip text on mouse hovering this class */
		Tooltip,
	};

	enum class PropertyMeta
	{
		/** This property will display under this category in the object property window */
		Category,
		/** Pop up a tooltip text on mouse hovering this property */
		Tooltip,
		/** Defines the min value of numerical property slider */
		Min,
		/** Defines the max value of numerical property slider */
		Max,
		/** Controls how fast it changes when you are dragging a property value, default is 1.0f */
		DragSensitivity,
		/** This property is transient and will not get serialized */
		Transient,
		/**
		 * Used to filter out GameObject instances not derived from this specified class, which will not appear in the dropdown list.
		 * The value (class name) should be a string literal.
		 * NOTE: We ONLY support register GameObject* pointer for now!
		 * e.g.
		 * GameObject* m_Player;
		 *
		 * RTTR_REGISTRATION
		 * {
		 *     rttr::registration::class_<my_struct>("my_struct")
		 *         .property("Player", &TestObject::m_Player)
		 *         (
		 *             rttr::metadata(META_PROP_SUBCLASSOF, "Player")
		 *         );
		 * }
		 */
		SubclassOf,
	};

	/**
	 * Base class for spawnable objects.
	 *
	 * IMPORTANT NOTES:
	 * You should always call ZeoEngine::Level::Get().SpawnGameObject<>(); to spawn one to the level.
	 * You should add the macro RTTR_REGISTER() to the end of every class derived from GameObject class, which you want to instantiate. Also make sure you have included level.h.
	 * You should add the macro RTTR_REGISTRATION_FRIEND to the end of the class derived from GameObject class, in which you want to register protected or private variables directly instead of getters and setters.
	 * If you come across a compile error: fatal error C1128: number of sections exceeded object file format limit: compile with /bigobj, add /bigobj to the CommandLine of project properties.
	 */
	class GameObject
	{
		friend class EditorLayer;
		friend class GameLayer;
		friend class Level;

	protected:
		GameObject() = default;

		virtual ~GameObject();

		const std::string& GetUniqueName() const { return m_UniqueName; }
		const std::string& GetName() const { return m_Name; }
	private:
		void SetUniqueName(const std::string& uniqueName) { m_UniqueName = uniqueName; }
		void SetName(const std::string& name) { m_Name = name; }
	public:
		GameObject* GetOwner() const { return m_OwnerObject; }
		void SetOwner(GameObject* owner) { m_OwnerObject = owner; }
		bool IsActive() const { return m_bIsActive; }
		void SetActive(bool bIsActive) { m_bIsActive = bIsActive; }
		bool IsPendingDestroy() const { return m_bPendingDestroy; }
		const Transform& GetTransform() const { return m_Transform; }
		void SetTransform(const Transform& transform) { m_Transform = transform; }
		const glm::vec2 GetPosition2D() const { return { m_Transform.position.x, m_Transform.position.y }; }
		const glm::vec3& GetPosition() const { return  m_Transform.position; }
		void SetPosition2D(const glm::vec2& position)
		{
			m_Transform.position.x = position.x;
			m_Transform.position.y = position.y;
			if (m_CollisionData)
			{
				m_CollisionData->UpdateData();
			}
		}
		/** For most cases, you should use SetPosition2D() for 2D rendering instead. */
		void SetPosition(const glm::vec3& position)
		{
			m_Transform.position = position;
			if (m_CollisionData)
			{
				m_CollisionData->UpdateData();
			}
		}
		/** For draw command, you should use radians as rotation. */
		const float GetRotation(bool bInRadians) const { return bInRadians ? glm::radians(m_Transform.rotation) : m_Transform.rotation; }
		void SetRotation(float rotation) { m_Transform.rotation = rotation; }
		const glm::vec2& GetScale() const { return m_Transform.scale; }
		void SetScale(const glm::vec2& scale)
		{
			m_Transform.scale = scale;
			if (m_CollisionData)
			{
				m_CollisionData->UpdateData();
			}
		}
		void SetScale(float uniformScale) { m_Transform.scale = { uniformScale, uniformScale }; }
		const glm::mat4& GetTransformMatrix() const { return m_TransformMatrix; }
		ObjectCollisionType GetCollisionType() const { return m_CollisionType; }
		void SetCollisionType(ObjectCollisionType newType) { m_CollisionType = newType; }
		bool ShouldGenerateOverlapEvents() const { return m_bGenerateOverlapEvents; }
		void SetGenerateOverlapEvents(bool bGenerate) { m_bGenerateOverlapEvents = bGenerate; }
		float GetSpeed() const { return m_Speed; }
		void SetSpeed(float speed) { m_Speed = speed; }
		const glm::vec2& GetVelocity() const { return m_Velocity; }

		virtual bool IsTranslucent() const { return false; }

		virtual void Init();
		virtual void BeginPlay() {}
		virtual void OnUpdate(DeltaTime dt);
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

		// TODO: Currently only implements for enum
		virtual void OnPropertyValueChange(const rttr::property& prop);

		/**
		 * Get forward vector based on this object's rotation.
		 * @see WORLD_UP_VECTOR
		 */
		const glm::vec2 GetForwardVector2D() const;
		/**
		 * Get right vector based on this object's rotation.
		 * @see WORLD_RIGHT_VECTOR
		 */
		const glm::vec2 GetRightVector2D() const;

		float FindLookAtRotation2D(const glm::vec2& sourcePosition, const glm::vec2& targetPosition);

		void TranslateTo2D(const glm::vec2& targetPosition);

		glm::vec2 GetRandomPositionInRange2D(const glm::vec2& center, const glm::vec2& extents);

		/** Reset necessary data, mostly used by ObjectPooler. */
		virtual void Reset();

		bool IsCollisionEnabled() const { return m_CollisionType > ObjectCollisionType::None; }

		/**
		 * Update box collision data.
		 * You SHOULD call it after specifying collision type and DO NOT call it in the constructor!
		 */
		void FillBoxCollisionData(const glm::vec2& extents = { 0.0f, 0.0f }, const glm::vec2& centerOffset = { 0.0f, 0.0f });
		/**
		 * Update sphere collision data.
		 * You SHOULD call it after specifying sphere collision type and DO NOT call it in the constructor!
		 */
		void FillSphereCollisionData(float radius = 0.0f, const glm::vec2& centerOffset = { 0.0f, 0.0f });

		void DoCollisionTest(const std::vector<GameObject*>& objects);

		virtual void OnOverlap(GameObject* other) {}

		virtual void ApplyDamage(float damage, GameObject* target, GameObject* causer, GameObject* instigator);
		virtual void TakeDamage(float damage, GameObject* causer, GameObject* instigator) {}

		void Destroy();
		virtual void OnDestroyed() {}

	private:
		/** Generate a new collision data based on current collision type. */
		void GenerateCollisionData();

		bool CheckCollision(GameObject* other);
		bool CheckCollision_BB(GameObject* other);
		bool CheckCollision_BS(GameObject* boxObject, GameObject* sphereObject);
		bool CheckCollision_SS(GameObject* other);

		/** Called after gizmo manipulation to update the transform property */
		void DecomposeTransformMatrix();
		/** Called after modifying the transform property to update the transform matrix used by gizmo */
		void ReComposeTransformMatrix();

		// TODO: Component system
	private:
		/** The unique name with suffix index number, e.g. "Player_2" */
		std::string m_UniqueName;
		/** The name displayed in the level outline, which can be modified by the user. By default, this is the same as m_UniqueName */
		std::string m_Name;
		bool m_bIsActive = true;
		/** GameObject that owns this one, which means if owner is destroyed, this one either */
		GameObject* m_OwnerObject;
		bool m_bPendingDestroy = false;

		//// Transform component
		Transform m_Transform;
		/**
		 * Matrix used by gizmo manipulation internally
		 * TODO: Do not pass it directly to OpenGL draw call as rotation inside is calculated from degrees while OpenGL prefers radians
		 */
		glm::mat4 m_TransformMatrix;

		//// Collision component
		ObjectCollisionType m_CollisionType = ObjectCollisionType::None;
		CollisionData* m_CollisionData = nullptr;
		bool m_bGenerateOverlapEvents = false;

		GameObject* OverlappedObject = nullptr;

		//// Movement component
		/** A scalar indicating how fast this object moves */
		float m_Speed = 0.0f;
		/** A vector representing current velocity of this object  */
		glm::vec2 m_Velocity{ 0.0f };

		bool m_bIsMoving = false;
		glm::vec2 m_MoveSourcePosition{ 0.0f }, m_MoveTargetPosition{ 0.0f };
		float m_MoveDistance = 0.0f;
		float m_MoveAlpha = 0.0f;
		/** Used for calculating current velocity */
		glm::vec2 m_LastPosition{ 0.0f };

#if WITH_EDITOR
		bool m_bIsSelectedInEditor = false;
#endif

		RTTR_ENABLE()
		RTTR_REGISTRATION_FRIEND
	};

}
