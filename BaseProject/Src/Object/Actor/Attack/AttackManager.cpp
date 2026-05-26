#include "AttackManager.h"

AttackManager::AttackManager(ObjectManager* objMng)
	: objMng_(objMng)
{
}

AttackManager::~AttackManager()
{
}

void AttackManager::Update(void)
{
	// XV	
	for (auto& attack : attacks_)
	{
		attack->Update();
	}
}

void AttackManager::Draw(void)
{
}

void AttackManager::SpawnFireBall(const VECTOR& startPos, const VECTOR& dir)
{
}
