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
	bool isHitKeyNew = ins.IsTrgDown(KEY_INPUT_E)
		|| ins.IsPadBtnPress(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT);

	// Eキーが押されたら投擲開始
	if (isHitKeyNew) {
		// 1. 手の現在座標にワープ＆飛翔状態を開始する
		ThrowStart();
	}

	// 状態ごとの移動・追従処理
	if (isThrow_) {
		// 2. 投げられている間：毎フレーム位置計算＆モデル行列の更新を行う
		Throw();
	}
	else {
		// 3. まだ投げていない間：プレイヤーの手のボーンに張り付く
		ThrowSet();
	}

	transform_.Update();
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

void Bomb::ThrowStart(void)
{
	if (data_.ownerModelId == -1) return;

	isThrow_ = true;

	// ① 手の現在位置（ワールド座標）を取得して爆弾の位置にする
	MATRIX handMat = MV1GetFrameLocalWorldMatrix(data_.ownerModelId, data_.ownerFrameIndex);
	data_.pos = VGet(handMat.m[3][0], handMat.m[3][1], handMat.m[3][2]);

	// ② 重力蓄積をリセット
	gravityPow_ = 0.0f;

	// ③ プレイヤー本体モデルの行列から正面方向を取得
	MATRIX ownerMat = MV1GetMatrix(data_.ownerModelId);
	VECTOR forward = VGet(ownerMat.m[2][0], ownerMat.m[2][1], ownerMat.m[2][2]);

	// 向きを矯正
	forward = VScale(forward, -1.0f);

	// 水平方向に調整して正規化
	forward.y = 0.0f;
	forward = VNorm(forward);

	// ④ 初速ベクトル（jumpPow_）の計算
	float throwSpeed = 400.0f; // 前方への飛翔スピード
	float upPower = 700.0f; // 山なりに上げるための上方向の力

	// jumpPow_ に「前方の力」と「上方向の力」を合成してセット
	jumpPow_ = VAdd(VScale(forward, throwSpeed), VGet(0.0f, upPower, 0.0f));
}
void Bomb::Throw(void)
{
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// ① 初速（移動ベクトル）による座標更新
	data_.pos = VAdd(data_.pos, VScale(jumpPow_, deltaTime));

	// ② 重力の計算（加速度的に下にひっぱる）
	gravityPow_ += 980.0f * deltaTime; // 重力の強さ
	data_.pos.y -= gravityPow_ * deltaTime;

	// ③ ★重要：移動後の座標でモデルの行列を書き換える
	MATRIX scaleMat = MGetScale(data_.scl);
	MATRIX transMat = MGetTranslate(data_.pos);
	MV1SetMatrix(data_.modelId_, MMult(scaleMat, transMat));
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

