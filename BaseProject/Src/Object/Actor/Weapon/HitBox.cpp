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
		transform_.SetModel
		(resMng_.LoadModelDuplicate(ResourceManager::SRC::CLUB));
	}
}

// 更新
void HitBox::Update(void)
{
	MV1SetPosition(transform_.modelId, data_.pos);
	MV1SetRotationXYZ(transform_.modelId, data_.rot);
	MV1SetScale(transform_.modelId, data_.scl);
}

// 描画
void HitBox::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

// 解放
void HitBox::Release(void)
{
	MV1DeleteModel(transform_.modelId);
}

