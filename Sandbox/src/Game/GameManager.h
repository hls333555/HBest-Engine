#pragma once

#include "ZeoEngine.h"

#include "Engine/GameFramework/ObjectPooler.h"

#include "Obstacle.h"

class GameManager : public ZeoEngine::GameObject
{
	using ObstaclePool = ZeoEngine::ObjectPooler<Obstacle, 10>;

public:
	virtual void BeginPlay() override;
	virtual void OnUpdate(ZeoEngine::DeltaTime dt) override;

	void DelaySpawnEnemy(float delay);
	void DefaultDelaySpawnEnemy();

private:
	void SpawnObstacle();

private:
	ZeoEngine::Scope<ObstaclePool> m_ObstaclePool;

	bool m_bShouldSpawnObstacle = true;

	RTTR_REGISTER(GameManager, GameObject)
};
