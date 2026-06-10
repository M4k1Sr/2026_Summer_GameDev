#include "Stage2.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SoundManager.h"

void Stage2::InitLoad(void)
{
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::SUB_STAGE));
	SoundManager::GetInstance().LoadBank(BANK_ID::STAGE_1);
}

void Stage2::InitTransform(void)
{
	transform_.scl = VGet(0.1f, 0.1f, 0.1f);
	transform_.pos = VGet(0.0f, -100.0f, 0.0f);
	transform_.Update();
}

void Stage2::InitCollider(void)
{

	// 基底クラスのヘルパー関数を呼ぶ
	SetupModelCollider(ColliderBase::TAG::STAGE, { "Mush", "Grass" }, { "Ground", "Cube" });

}

void Stage2::InitPost(void)
{
	SoundManager::GetInstance().PlayEvent(SOUND_ID::BGM_STAGE1, true);
}
