#include <DxLib.h>
#include "../../../../Utility/AsoUtility.h"
#include "../../../../Manager/InputManager.h"
#include "../../../../Manager/SceneManager.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/Resource.h"
#include "../../../../Object/Common/Transform.h"
#include "../../../Collider/ColliderLine.h"
#include "../../../Collider/ColliderCapsule.h"
#include "../../../Collider/ColliderModel.h"
#include "../../../../Object/Common/AnimationController.h"
#include "../../../../Application.h"
#include "./BossBase.h"
#include "./BossPixie.h"

BossPixie::BossPixie(const BossBase::BossData& data)
	:
	BossBase(data)
{
}

BossPixie::~BossPixie(void)
{
}

void BossPixie::InitLoad(void)
{
	// 基底クラスのリソースロード
	BossBase::InitLoad();

	// モデル読み込み
	transform_.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::OBJECT_BOSS_PIXIE));

}

void BossPixie::InitTransform(void)
{

	// モデルの大きさ、回転、座標の初期化
	transform_.scl = VGet(SCALE, SCALE, SCALE);
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.Update();
}

void BossPixie::InitCollider(void)
{

	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::BOX, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// モデルとの衝突で使用するモデルコライダー
	ColliderModel* colModel = new ColliderModel(
		ColliderBase::TAG::BOX,
		&transform_);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), colModel);
}

void BossPixie::InitAnimation(void)
{
	//モデルアニメーション制御の初期化
	animationController_ = new AnimationController(transform_.modelId);

	//// アニメーション追加
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), 20.0f, Application::PATH_MODEL + "Player/Idle.mv1");
	//animationController_->Add(static_cast<int>(ANIM_TYPE::RUN), 30.0f, Application::PATH_MODEL + "Player/Run.mv1");
	//animationController_->Add(static_cast<int>(ANIM_TYPE::FAST_RUN), 30.0f, Application::PATH_MODEL + "Player/FastRun.mv1");
	//animationController_->Add(static_cast<int>(ANIM_TYPE::JUMP), 60.0f, Application::PATH_MODEL + "Player/JumpRising.mv1");

		// アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::IDLE), true);

}

void BossPixie::InitPost(void)
{
	// 基底クラスの初期化後処理

	// 初期遷移状態初期処理登録
	stateChanges_.emplace(static_cast<int>(STATE::NONE),
		std::bind(&BossPixie::ChangeStateNone, this));

	stateChanges_.emplace(static_cast<int>(STATE::IDLE),
		std::bind(&BossPixie::ChangeStateIdle, this));

	stateChanges_.emplace(static_cast<int>(STATE::SURPRISE),
		std::bind(&BossPixie::ChangeStateSurprise, this));

	stateChanges_.emplace(static_cast<int>(STATE::CHARGE),
		std::bind(&BossPixie::ChangeStateCharge, this));

	stateChanges_.emplace(static_cast<int>(STATE::THROW),
		std::bind(&BossPixie::ChangeStateThrow, this));

	stateChanges_.emplace(static_cast<int>(STATE::ATTACK_WAVE),
		std::bind(&BossPixie::ChangeStateAttackWave, this));

	stateChanges_.emplace(static_cast<int>(STATE::ATTACK_END),
		std::bind(&BossPixie::ChangeStateAttackEnd, this));

	stateChanges_.emplace(static_cast<int>(STATE::DAMAGE),
		std::bind(&BossPixie::ChangeStateDamage, this));

	stateChanges_.emplace(static_cast<int>(STATE::DOWN),
		std::bind(&BossPixie::ChangeStateDown, this));

	stateChanges_.emplace(static_cast<int>(STATE::END),
		std::bind(&BossPixie::ChangeStateEnd, this));

	// 初期状態設定
	ChangeState(STATE::IDLE);

}

void BossPixie::UpdateProcess(void)
{
	// 状態別更新
	stateUpdate_();
}

void BossPixie::UpdateProcessPost(void)
{
	transform_.Update();

	BossBase::UpdateProcessPost();

}

void BossPixie::ChangeState(STATE state)
{
	state_ = state;

	// 各状態の初期状態設定
	BossBase::ChangeState(static_cast<int>(state_));

}

void BossPixie::ChangeStateNone(void)
{
}

void BossPixie::ChangeStateIdle(void)
{
}

void BossPixie::ChangeStateSurprise(void)
{
}

void BossPixie::ChangeStateCharge(void)
{
}

void BossPixie::ChangeStateThrow(void)
{
}

void BossPixie::ChangeStateAttackWave(void)
{
}

void BossPixie::ChangeStateAttackEnd(void)
{
}

void BossPixie::ChangeStateDamage(void)
{
}

void BossPixie::ChangeStateDown(void)
{
}

void BossPixie::ChangeStateEnd(void)
{
}

void BossPixie::UpdateNone(void)
{
}

void BossPixie::UpdateIdle(void)
{
}

void BossPixie::UpdateSurprise(void)
{
}

void BossPixie::UpdateCharge(void)
{
}

void BossPixie::UpdateThrow(void)
{
}

void BossPixie::UpdateAttackWave(void)
{
}

void BossPixie::UpdateAttackEnd(void)
{
}

void BossPixie::UpdateDamage(void)
{
}

void BossPixie::UpdateDown(void)
{
}

void BossPixie::UpdateEnd(void)
{
}
