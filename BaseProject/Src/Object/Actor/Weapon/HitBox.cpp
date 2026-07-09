#include "HitBox.h"
#include "../../../Manager/ResourceManager.h"

// コンストラクタ
HitBox::HitBox(const WeaponData& data)
	: data_(data)
	, isAttackAct_(false)
	, isHit_(false)
	, isAct_(false)
	, actTimer_(0.0f)
	, resMng_(ResourceManager::GetInstance())
{
}

// リソースロード
void HitBox::Load(void)
{
	// こん棒リソースロード
	if (data_.kind == WeaponKind::Club) {
		data_.modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::CLUB);
	}
}

// 更新
void HitBox::Update(void)
{
}

// 描画
void HitBox::Draw(void)
{
	MV1DrawModel(data_.modelId_);
}

// 解放
void HitBox::Release(void)
{
	MV1DeleteModel(data_.modelId_);
}

