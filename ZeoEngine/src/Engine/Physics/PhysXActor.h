#pragma once

#include <PxRigidActor.h>

#include "Engine/GameFramework/Entity.h"
#include "Engine/Physics/PhysicsTypes.h"

namespace ZeoEngine {

	class PhysXColliderShapeBase;

	class PhysXActor
	{
		friend class PhysXScene;

	public:
		PhysXActor(Entity entity);
		~PhysXActor();

		Entity GetEntity() const { return m_Entity; }

		physx::PxRigidActor& GetRigidActor() const { return *m_RigidActor; }

		void SetTranslation(const Vec3& translation, bool bAutoWake = true) const;
		void SetRotation(const Vec3& rotation, bool bAutoWake = true) const;
		void SetTransform(const Mat4& transform, bool bAutoWake = true) const;

		bool IsDynamic() const;
		bool IsKinematic() const;
		void SetKinematic(bool bIsKinematic) const;

		bool IsSleeping() const;

		bool IsGravityEnabled() const;
		void SetGravityEnabled(bool bEnable) const;

		float GetMass() const;
		void SetMass(float mass) const;

		float GetLinearDamping() const;
		void SetLinearDamping(float damping) const;
		float GetAngularDamping() const;
		void SetAngularDamping(float damping) const;

		Vec3 GetLinearVelocity() const;
		void SetLinearVelocity(const Vec3& velocity) const;
		Vec3 GetAngularVelocity() const;
		void SetAngularVelocity(const Vec3& velocity) const;
		float GetMaxLinearVelocity() const;
		void SetMaxLinearVelocity(float maxVelocity) const;
		float GetMaxAngularVelocity() const;
		void SetMaxAngularVelocity(float maxVelocity) const;

		void AddForce(const Vec3& force, ForceMode forceMode) const;

		void AddCollider(ColliderType type);

	private:
		void CreateRigidActor();
		void SynchronizeTransform();

	private:
		Entity m_Entity;

		physx::PxRigidActor* m_RigidActor = nullptr;
		std::vector<Scope<PhysXColliderShapeBase>> m_Colliders;
	};

}
