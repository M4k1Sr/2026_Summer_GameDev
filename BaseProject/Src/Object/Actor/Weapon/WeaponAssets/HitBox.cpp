#include "HitBox.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Utility/MatrixUtility.h"

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
	if (data_.weapon == WeaponKind::CLUB) {
		data_.modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::CLUB);
	}
}

// 更新
void HitBox::Update(void)
{
	// オーナー未設定の場合何もしない
	if (data_.ownerModelId == -1) return;
	if (data_.modelId_ == -1) return;

	// 武器のスケール行列
	MATRIX scaleMat = MGetScale(data_.scl);

	// 武器のローカル回転行列(向きの補正)
	MATRIX localRotMat = MatrixUtility::GetMatrixRotateXYZ(data_.localRot);

	// 武器のローカル位置の変換行列(位置の補正)
	MATRIX transMatPos = MGetTranslate(data_.localPos);

	// 手のボーンのワールド行列(現在のポーズでの位置・向き)
	MATRIX handMat = MV1GetFrameLocalWorldMatrix(
		data_.ownerModelId, data_.ownerFrameIndex);

	// 合成: スケール → ローカル回転 → ローカル位置オフセット → 手のワールド行列
	MATRIX localMat = MMult(scaleMat, localRotMat);
	localMat = MMult(localMat, transMatPos);
	MATRIX resultMat = MMult(localMat, handMat);

	MV1SetMatrix(data_.modelId_, resultMat);
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

