#include "AttackManager.h"
#include "./AttackBase.h"
#include "../../Collider/ColliderBase.h"

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
	// •`‰æ
	for (auto& attack : attacks_)
	{
		attack->Draw();
	}
}

void AttackManager::Release(void)
{
	// ‰ð•ú
	for (auto& attack : attacks_)
	{
		attack->Release();
		delete attack;
	}
	attacks_.clear();
}

void AttackManager::AddHitCollider(const ColliderBase* hitCollider)
{
	for (auto& attack : attacks_)
	{
		attack->AddHitCollider(hitCollider);
	}
}
void AttackManager::SpawnFireBall(const VECTOR& startPos, const VECTOR& dir)
{
}
