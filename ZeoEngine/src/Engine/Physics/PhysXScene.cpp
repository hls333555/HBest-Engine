#include "ZEpch.h"
#include "Engine/Physics/PhysXScene.h"

#include "Engine/GameFramework/Components.h"
#include "Engine/Physics/PhysicsEngine.h"
#include "Engine/Physics/PhysXEngine.h"
#include "Engine/Physics/PhysXUtils.h"
#include "Engine/Physics/PhysXActor.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Utils/SceneUtils.h"

namespace ZeoEngine {

	class PhysXBroadPhaseCallback : public physx::PxBroadPhaseCallback
	{
	public:
		void onObjectOutOfBounds(physx::PxShape& shape, physx::PxActor& actor) override;
		void onObjectOutOfBounds(physx::PxAggregate& aggregate) override;
	};

	// https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxguide/Manual/RigidBodyCollision.html#broad-phase-callback
	void PhysXBroadPhaseCallback::onObjectOutOfBounds(physx::PxShape& shape, physx::PxActor& actor)
	{
		if (const auto* rigidActor = actor.is<physx::PxRigidActor>())
		{
#ifdef ZE_DEBUG
			const char* name = rigidActor->getName();
#else
			const auto* physXActor = static_cast<PhysXActor*>(rigidActor->userData);
			const std::string& name = physXActor->GetEntity().GetName();
#endif
			ZE_CORE_WARN("Collision detection is disabled for physics actor \"{}\" due to being out of world bounds", name);
		}
	}

	void PhysXBroadPhaseCallback::onObjectOutOfBounds(physx::PxAggregate& aggregate)
	{
	}

	class PhysXSimulationEventCallback : public physx::PxSimulationEventCallback
	{
	public:
		virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
		virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override;
		virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;
	};

	void PhysXSimulationEventCallback::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
	{
	}

	void PhysXSimulationEventCallback::onWake(physx::PxActor** actors, physx::PxU32 count)
	{
	}

	void PhysXSimulationEventCallback::onSleep(physx::PxActor** actors, physx::PxU32 count)
	{
	}

	void PhysXSimulationEventCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{
		if (!SceneUtils::IsLevelPlaying()) return;

		auto removedActor0 = pairHeader.flags & physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_0;
		auto removedActor1 = pairHeader.flags & physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_1;
		if (removedActor0 || removedActor1) return;

		const auto* actor0 = static_cast<PhysXActor*>(pairHeader.actors[0]->userData);
		const auto* actor1 = static_cast<PhysXActor*>(pairHeader.actors[1]->userData);
		if (!actor0 || !actor1) return;

		bool bActor0ScriptClassValid = ScriptEngine::IsEntityScriptClassValid(actor0->GetEntity());
		bool bActor1ScriptClassValid = ScriptEngine::IsEntityScriptClassValid(actor1->GetEntity());
		if (!bActor0ScriptClassValid && !bActor1ScriptClassValid) return;

		for (U32 i = 0; i < nbPairs; ++i)
		{
			const auto& contactPair = pairs[i];

			auto removedShape0 = contactPair.flags & physx::PxContactPairFlag::eREMOVED_SHAPE_0;
			auto removedShape1 = contactPair.flags & physx::PxContactPairFlag::eREMOVED_SHAPE_1;
			if (removedShape0 || removedShape1) continue;

			const auto* shape0 = contactPair.shapes[0];
			auto* collider0 = static_cast<PhysXColliderShapeBase*>(shape0->userData);
			const auto* shape1 = contactPair.shapes[1];
			auto* collider1 = static_cast<PhysXColliderShapeBase*>(shape1->userData);

			physx::PxContactPairPoint contacts[PHYSICS_MAX_CONTACT_POINTS];
			const U32 numContacts = contactPair.extractContacts(contacts, PHYSICS_MAX_CONTACT_POINTS);
			CollisionInfo collisionInfo;
			collisionInfo.NumContacts = numContacts;
			for (U32 j = 0; j < numContacts; ++j)
			{
				const auto& contact = contacts[j];

				auto* physxMaterial0 = shape0->getMaterialFromInternalFaceIndex(contact.internalFaceIndex0);
				const auto physicsMaterialAsset0 = static_cast<PhysicsMaterial*>(physxMaterial0->userData)->GetHandle();
				auto* physxMaterial1 = shape1->getMaterialFromInternalFaceIndex(contact.internalFaceIndex1);
				const auto physicsMaterialAsset1 = static_cast<PhysicsMaterial*>(physxMaterial1->userData)->GetHandle();
				collisionInfo.Contacts[j] = { PhysXUtils::FromPhysXVector(contact.position), PhysXUtils::FromPhysXVector(contact.normal), contact.separation, { physicsMaterialAsset0, physicsMaterialAsset1 } };
			}
			if (contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				if (bActor0ScriptClassValid)
				{
					collisionInfo.OtherCollider = collider1;
					ScriptEngine::OnCollisionBegin(actor0->GetEntity(), actor1->GetEntity(), collisionInfo);
				}
				if (bActor1ScriptClassValid)
				{
					collisionInfo.OtherCollider = collider0;
					ScriptEngine::OnCollisionBegin(actor1->GetEntity(), actor0->GetEntity(), collisionInfo);
				}
			}
			else if (contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				if (bActor0ScriptClassValid)
				{
					collisionInfo.OtherCollider = collider1;
					ScriptEngine::OnCollisionEnd(actor0->GetEntity(), actor1->GetEntity(), collisionInfo);
				}
				if (bActor1ScriptClassValid)
				{
					collisionInfo.OtherCollider = collider0;
					ScriptEngine::OnCollisionEnd(actor1->GetEntity(), actor0->GetEntity(), collisionInfo);
				}
			}
		}
	}

	void PhysXSimulationEventCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		if (!SceneUtils::IsLevelPlaying()) return;

		for (U32 i = 0; i < count; ++i)
		{
			const auto& pair = pairs[i];

			auto removedTrigger = pair.flags & physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER;
			auto removedOther = pair.flags & physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER;
			if (removedTrigger || removedOther) continue;

			const auto* triggerActor = static_cast<PhysXActor*>(pair.triggerActor->userData);
			const auto* otherActor = static_cast<PhysXActor*>(pair.otherActor->userData);
			if (!triggerActor || !otherActor) continue;

			bool bTriggerActorScriptClassValid = ScriptEngine::IsEntityScriptClassValid(triggerActor->GetEntity());
			bool bOtherActorScriptClassValid = ScriptEngine::IsEntityScriptClassValid(otherActor->GetEntity());
			if (!bTriggerActorScriptClassValid && !bOtherActorScriptClassValid) continue;

			if (pair.status == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				if (bTriggerActorScriptClassValid)
				{
					ScriptEngine::OnTriggerBegin(triggerActor->GetEntity(), otherActor->GetEntity());
				}
				if (bOtherActorScriptClassValid)
				{
					ScriptEngine::OnTriggerBegin(otherActor->GetEntity(), triggerActor->GetEntity());
				}
			}
			else if (pair.status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				if (bTriggerActorScriptClassValid)
				{
					ScriptEngine::OnTriggerEnd(triggerActor->GetEntity(), otherActor->GetEntity());
				}
				if (bOtherActorScriptClassValid)
				{
					ScriptEngine::OnTriggerEnd(otherActor->GetEntity(), triggerActor->GetEntity());
				}
			}
		}
	}

	void PhysXSimulationEventCallback::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
	{
	}

	static physx::PxFilterFlags FilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
	{
		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
			return physx::PxFilterFlag::eDEFAULT;
		}

		pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

		if (filterData0.word2 == static_cast<U32>(RigidBodyComponent::CollisionDetectionType::Continuous) || filterData1.word2 == static_cast<U32>(RigidBodyComponent::CollisionDetectionType::Continuous))
		{
			pairFlags |= physx::PxPairFlag::eDETECT_CCD_CONTACT;
		}

		if ((filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1))
		{
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
			pairFlags |= physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
			return physx::PxFilterFlag::eDEFAULT;
		}

		return physx::PxFilterFlag::eSUPPRESS;
	}

	static PhysXBroadPhaseCallback s_BroadPhaseCallback;
	static PhysXSimulationEventCallback s_SimulationEventCallback;

	PhysXScene::PhysXScene()
	{
		auto& physics = PhysXEngine::GetPhysics();
		const auto& settings = PhysicsEngine::GetSettings();
		m_SubStepTime = settings.FixedDeltaTime;

		physx::PxSceneDesc sceneDesc(physics.getTolerancesScale());
		sceneDesc.gravity = PhysXUtils::ToPhysXVector(settings.Gravity);
		sceneDesc.broadPhaseType = static_cast<physx::PxBroadPhaseType::Enum>(settings.BroadphaseAlgorithm);
		sceneDesc.broadPhaseCallback = &s_BroadPhaseCallback;
		sceneDesc.frictionType = static_cast<physx::PxFrictionType::Enum>(settings.FrictionModel);
		sceneDesc.solverType = static_cast<physx::PxSolverType::Enum>(settings.SolverModel);
		sceneDesc.filterShader = FilterShader;
		sceneDesc.simulationEventCallback = &s_SimulationEventCallback;
		sceneDesc.cpuDispatcher = PhysXEngine::GetCPUDispatcher();
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD | physx::PxSceneFlag::eENABLE_PCM;
		if (settings.bEnableEnhancedDeterminism)
		{
			sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ENHANCED_DETERMINISM;
		}

		ZE_CORE_ASSERT(sceneDesc.isValid());

		m_PhysicsScene = physics.createScene(sceneDesc);

		CreateRegions();
	}

	PhysXScene::~PhysXScene()
	{
		m_Actors.clear();

		m_PhysicsScene->release();
		m_PhysicsScene = nullptr;
	}

	void PhysXScene::Simulate(DeltaTime dt)
	{
		if (Advance(dt))
		{
			U32 numActiveActors;
			auto** activeActors = m_PhysicsScene->getActiveActors(numActiveActors);
			for (uint32_t i = 0; i < numActiveActors; ++i)
			{
				auto* actor = static_cast<PhysXActor*>(activeActors[i]->userData);
				if (actor && !actor->IsSleeping())
				{
					actor->SynchronizeTransform();
				}
			}
		}
	}

	PhysXActor* PhysXScene::GetActor(Entity entity) const
	{
		const auto it = m_Actors.find(entity.GetUUID());
		if (it == m_Actors.end()) return nullptr;
		return it->second.get();
	}

	PhysXActor* PhysXScene::CreateActor(Entity entity)
	{
		auto foundActor = GetActor(entity);
		if (foundActor) return foundActor;

		auto actor = CreateScope<PhysXActor>(entity);
		auto* actorPtr = actor.get();
		// TODO: Entity hierarchy
		if (entity.HasComponent<RigidBodyComponent>())
		{
			
		}
		m_Actors.emplace(entity.GetUUID(), std::move(actor));
		m_PhysicsScene->addActor(actorPtr->GetRigidActor());
		return actorPtr;
	}

	void PhysXScene::DestroyActor(Entity entity)
	{
		if (const auto* actor = GetActor(entity))
		{
			m_PhysicsScene->removeActor(actor->GetRigidActor());
		}
		m_Actors.erase(entity.GetUUID());
	}

	void PhysXScene::CreateRegions() const
	{
		physx::PxBroadPhaseCaps broadPhaseCaps;
		m_PhysicsScene->getBroadPhaseCaps(broadPhaseCaps);
		if (!broadPhaseCaps.needsPredefinedBounds) return;

		const auto& settings = PhysicsEngine::GetSettings();
		auto* regionBounds = new physx::PxBounds3[settings.WorldBoundsSubdivisions * settings.WorldBoundsSubdivisions];
		const physx::PxBounds3 globalBounds = physx::PxBounds3::centerExtents(PhysXUtils::ToPhysXVector(settings.WorldBoundsCenter), PhysXUtils::ToPhysXVector(settings.WorldBoundsExtent));
		const U32 regionCount = physx::PxBroadPhaseExt::createRegionsFromWorldBounds(regionBounds, globalBounds, settings.WorldBoundsSubdivisions);
		for (U32 i = 0; i < regionCount; i++)
		{
			physx::PxBroadPhaseRegion region;
			region.bounds = regionBounds[i];
			m_PhysicsScene->addBroadPhaseRegion(region);
		}
		
		delete[] regionBounds;
	}

	bool PhysXScene::Advance(DeltaTime dt)
	{
		EvaluateSubSteps(dt);

		for (U32 i = 0; i < m_NumSubSteps; ++i)
		{
			m_PhysicsScene->simulate(m_SubStepTime);
			m_PhysicsScene->fetchResults(true);
		}

		return m_NumSubSteps != 0;
	}

	void PhysXScene::EvaluateSubSteps(DeltaTime dt)
	{
		if (m_AccumulatedDeltaTime > m_SubStepTime)
		{
			m_AccumulatedDeltaTime = 0.0f;
		}

		m_AccumulatedDeltaTime += dt;
		if (m_AccumulatedDeltaTime < m_SubStepTime)
		{
			m_NumSubSteps = 0;
			return;
		}

		m_NumSubSteps = glm::min(static_cast<U32>(m_AccumulatedDeltaTime / m_SubStepTime), c_MaxSubSteps);
		m_AccumulatedDeltaTime -= static_cast<float>(m_NumSubSteps) * m_SubStepTime;
	}

}
