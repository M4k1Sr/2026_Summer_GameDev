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
	stateUpdate_(nullptr)
{
	// モデル初期座標の設定
	transform_.pos = startPos_;

	// モデルのハンドルIDを設定
	// modelHandle_ = MV1DuplicateModel(ResourceManagerからマスターのハンドルを取得);
	// MV1SetScale(modelHandle_, VGet(data.scale, data.scale, data.scale));

}

AttackBase::~AttackBase(void)
{
}

void AttackBase::Update()
{
	// 共通の処理
	// 指定された方向に毎フレーム移動
	transform_.pos = VAdd(transform_.pos, VScale(moveDir_, speed_));
	
	// 2. 弾の種類（type_）に応じた処理
	// 随時追加
	switch (type_)
	{
		case TYPE::FIRE_BALL:
			// 火の玉の動く処理
			ProcessFireBall();
			break;
		case TYPE::WAVE_ATTACK:
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
	if (VSize(VSub(transform_.pos, startPos_)) > maxDistance_)
	{
		isDead_ = true;
	}

	// モデル制御更新
	transform_.Update();

}

void AttackBase::Draw(void)
{
	// 基底クラスの描画処理
	ActorBase::Draw();

	DrawSphere3D(transform_.pos, 10.0f * scale_, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), true);
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
}

void AttackBase::ProcessWaveAttack(void)
{
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
