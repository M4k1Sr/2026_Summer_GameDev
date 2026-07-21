#include "Bomb.h"
#include "../../../../Manager/InputManager.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Utility/MatrixUtility.h"

// コンストラクタ
Bomb::Bomb(const WeaponData& data)
	: data_(data)
	, isThrow_(false)
	, isHit_(false)
	, isAct_(false)
	, actTimer_(0.0f)
	, resMng_(ResourceManager::GetInstance())
{
}

// リソースロード
void Bomb::Load(void)
{
	// 爆弾リソースロード
	if (data_.item == ItemKind::BOMB) {
		data_.modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::BOMB);
	}
}

// 更新
void Bomb::Update(void)
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

	//Throw();
}

// 描画
void Bomb::Draw(void)
{
	MV1DrawModel(data_.modelId_);

}

// 解放
void Bomb::Release(void)
{
	MV1DeleteModel(data_.modelId_);
}

void Bomb::Throw(void)
{
	auto& ins = InputManager::GetInstance();

	// ギミック処理
	bool isHitKeyNew = ins.IsPress(KEY_INPUT_E)
		|| ins.IsPadBtnPress(
			InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT);

	if (isHitKeyNew) {

	}

}

