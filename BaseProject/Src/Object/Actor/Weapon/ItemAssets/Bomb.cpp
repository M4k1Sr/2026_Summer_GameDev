#include "Bomb.h"
#include "../../../../Manager/InputManager.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Utility/MatrixUtility.h"
#include "../../../../Application.h"
#include "../../../../Manager/SceneManager.h"
#include "../../../../Object/Actor/Charactor/Player.h"


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
	auto& ins = InputManager::GetInstance();

	// 投擲キー判定 (Eキー)
	bool isHitKeyNew = ins.IsNew(KEY_INPUT_E)
		|| ins.IsPadBtnPress(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT);

	// Eキーが押されたら、手に持っていようが既に飛んでいようが Throw() を呼ぶ！
	if (isHitKeyNew) {
		Throw();
	}

	// 投擲中の処理
	if (isThrow_) {
		float deltaTime = SceneManager::GetInstance().GetDeltaTime();

		// 重力計算・移動処理
		Gravity();
		data_.pos = VAdd(data_.pos, VScale(jumpPow_, deltaTime));
		VECTOR gravityVec = VScale(AsoUtility::DIR_U, gravityPow_);
		data_.pos = VAdd(data_.pos, gravityVec);

		// モデル位置更新
		MATRIX scaleMat = MGetScale(data_.scl);
		MATRIX transMat = MGetTranslate(data_.pos);
		MV1SetMatrix(data_.modelId_, MMult(scaleMat, transMat));
	}
	else {
		// 初期状態（手に持っている間）
		ThrowSet();
	}
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

void Bomb::Gravity(void)
{
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// 重力加速度計算
	gravityPow_ += Application::GetInstance().GetGravityPow() * deltaTime;

	// 落下速度制限 (MAX_FALL_SPEED)
	if (gravityPow_ > -MAX_FALL_SPEED)
	{
		gravityPow_ = -MAX_FALL_SPEED;
	}
}

void Bomb::Throw(void)
{
	if (data_.ownerModelId == -1) return;

	isThrow_ = true;

	// ① 手の位置を取得
	MATRIX handMat = MV1GetFrameLocalWorldMatrix(data_.ownerModelId, data_.ownerFrameIndex);
	data_.pos = VGet(handMat.m[3][0], handMat.m[3][1], handMat.m[3][2]);

	// ② 重力のリセット
	gravityPow_ = 0.0f;

	// ③ プレイヤーのモデル行列を取得
	MATRIX ownerMat = MV1GetMatrix(data_.ownerModelId);
	VECTOR ownerForward = VGet(ownerMat.m[2][0], ownerMat.m[2][1], ownerMat.m[2][2]);

	// ★ 後ろを向いているため、反転させる（ -1.0f を掛ける）
	ownerForward = VScale(ownerForward, -1.0f);

	// 水平にして正規化
	ownerForward.y = 0.0f;
	ownerForward = VNorm(ownerForward);

	// ④ 初速の計算
	float throwPower = 600.0f; // 前方への速度
	float upPower = 300.0f; // 上方向の力

	jumpPow_ = VAdd(VScale(ownerForward, throwPower), VGet(0.0f, upPower, 0.0f));
}

void Bomb::ThrowSet(void)
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

