#include "AttackBase.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Object/Common/Transform.h"
#include "../../../Application.h"

AttackBase::AttackBase(const AttackBase::AttackParam& data, const VECTOR& startPos, const VECTOR& dir)
	:
	ActorBase(),
	type_(data.type),
	stateBase_(0),
	startPos_(startPos),
	moveDir_(dir),
	gravityVelocity_(0.0f),
	modelHandle_(-1),
	isAttack_(false),
	isDead_(false),
	stateChanges_(),
	stateUpdate_(nullptr),
	param_(data)
{
	// モデル初期座標の設定
	transform_.pos = startPos_;

	// モデルのハンドルIDを設定
	// modelHandle_ = MV1DuplicateModel(ResourceManagerからマスターのハンドルを取得);
	// MV1SetScale(modelHandle_, VGet(data.scale, data.scale, data.scale));
	alpha_ = 1.0f;
	fadeTimer_ = 0.0f;
	isFading_ = false;
}

AttackBase::~AttackBase(void)
{
}

void AttackBase::Update()
{
	// 基底クラスの更新処理
	// 随時追加
	switch (type_)
	{
		case TYPE::FIRE_BALL:
			// 火の玉の動く処理
			ProcessFireBall();
			break;
		case TYPE::WAVE_ATTACK:
			// ウェーブ攻撃の動く処理
			ProcessWaveAttack();
			break;
		case TYPE::ARROW_ATTACK:
			ProcessArrowAttack();
			break;
		case TYPE::CHARGE_ATTACK:
			ProcessChargeAttack();
			break;
		case TYPE::AXE_THROW_ATTACK:
			ProcessAxeThrowAttack();
			break;
		default:
			break;


	}




	// 消滅処理
	// 生成場所(StartPos)から一定の距離(maxDistance)を離れたら消滅させる→isDead : true
	if (!isFading_) {
		if (VSize(VSub(transform_.pos, startPos_)) > param_.maxDistance)
		{
			isFading_ = true;
		}
	}
	else {
		// フェードタイマー更新
		fadeTimer_++;
		// 不透明度を1.0fから0.0fへ減らす
		alpha_ = 1.0f - (fadeTimer_ / 30.0f);

		// 完全に透明になったらマネージャに透明にしてもらうために死亡フラグを立てる
		if (alpha_ <= 0.0f)
		{
			isDead_ = true;
		}
	}

	// モデル制御更新
	transform_.Update();

}

void AttackBase::Draw(void)
{
	// 基底クラスの描画処理
	ActorBase::Draw();

	// 描画ブレンドモードを「αブレンド（透明対応）」にして、現在のalpha_を適用する
	// DxLibのブレンド値は 0〜255 なので、alpha_(0.0〜1.0) に 255 を掛け算します
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * alpha_));

	// 赤いデバッグ球体を描画
	DrawSphere3D(transform_.pos, 100.0f * param_.scale, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), true);

	// 重要：ブレンドモードをノーマル（不透明に戻しておく（他の描画が透けないようにするため）
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void AttackBase::Release(void)
{

	// 基底クラス解放
	ActorBase::Release();

}

void AttackBase::InitCollider(void)
{
	// 後々追加予定
}

void AttackBase::InitPost(void)
{
	alpha_ = 1.0f;
	fadeTimer_ = 0.0f;
	isFading_ = false;
}

void AttackBase::ChangeState(int state)
{
	// 同じ状態なら何もしない
	if (stateBase_ == state) return;

	stateBase_ = state;

	if (stateChanges_.find(state) != stateChanges_.end())
	{
		stateChanges_[state]();
	}
}

void AttackBase::ProcessFireBall(void)
{
	// 座標処理
	transform_.pos = VAdd(transform_.pos, VScale(moveDir_, param_.speed));

}

void AttackBase::ProcessWaveAttack(void)
{
	// 座標処理
	transform_.pos = VAdd(transform_.pos, VScale(moveDir_, param_.speed));

}

void AttackBase::ProcessArrowAttack(void)
{
}

void AttackBase::ProcessChargeAttack(void)
{
}

void AttackBase::ProcessAxeThrowAttack(void)
{
}
