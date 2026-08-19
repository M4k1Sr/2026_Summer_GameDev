#include "Stage2.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/SoundManager.h"
#include "../../../Manager/ServiceLocator.h"

void Stage2::InitLoad(void)
{
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::SECOND_STAGE));
	ServiceLocator::GetSound().LoadBank(BANK_ID::STAGE_2);
}

void Stage2::InitTransform(void)
{
	transform_.scl = SECOND_STAGE_SCALE;
	transform_.pos = NEXT_STAGE_POS;
	transform_.Update();
}

void Stage2::InitCollider(void)
{

	// 基底クラスのヘルパー関数を呼ぶ
	SetupModelCollider(ColliderBase::TAG::STAGE, { "Mush", "Grass" }, {/* "Ground", "Cube" */});

}

void Stage2::InitPost(void)
{
	ServiceLocator::GetSound().PlayEvent(SOUND_ID::BGM_STAGE1, true);

}
