#include <DxLib.h>
#include "../../../../Utility/AsoUtility.h"
#include "../../../../Manager/InputManager.h"
#include "../../../../Manager/SceneManager.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/Resource.h"
#include "../../../../Object/Common/Transform.h"
#include "../../../../Object/Common/AnimationController.h"
#include "../Player.h"
#include "../../../Collider/ColliderLine.h"
#include "../../../Collider/ColliderCapsule.h"
#include "../../../Collider/ColliderModel.h"
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
	animationController_ = 
		new AnimationController(transform_.modelId);

	// アニメーション追加
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), 20.0f, Application::PATH_MODEL + "Enemy/Pixie/Idle.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::SURPRISE), 20.0f, Application::PATH_MODEL + "Enemy/Pixie/Surprise.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::CHARGE), 20.0f, Application::PATH_MODEL + "Enemy/Pixie/Charge.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::THROW), 30.0f, Application::PATH_MODEL + "Enemy/Pixie/Throw.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK_WAVE), 20.0f, Application::PATH_MODEL + "Enemy/Pixie/Attack_Wave.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK_END), 20.0f, Application::PATH_MODEL + "Enemy/Pixie/Attack_End.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::DAMAGE), 20.0f, Application::PATH_MODEL + "Enemy/Pixie/Damage.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::DOWN), 20.0f, Application::PATH_MODEL + "Enemy/Pixie/Down.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::END), 20.0f, Application::PATH_MODEL + "Enemy/Pixie/End.mv1");

	// アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::IDLE), true);

}

void BossPixie::InitPost(void)
{
	// 基底クラスの初期化後処理
	stateTimer_ = 0.0f;
	stateTime_ = 0.0f;



	// 初期遷移状態初期処理登録

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
	if (player_ == nullptr)
	{
	}

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


void BossPixie::ChangeStateIdle(void)
{
	stateUpdate_ = std::bind(&BossPixie::UpdateIdle, this);

}

void BossPixie::ChangeStateSurprise(void)
{
	stateUpdate_ = std::bind(&BossPixie::UpdateSurprise, this);

}

void BossPixie::ChangeStateCharge(void)
{
	stateUpdate_ = std::bind(&BossPixie::UpdateCharge, this);

}

void BossPixie::ChangeStateThrow(void)
{
	stateUpdate_ = std::bind(&BossPixie::UpdateThrow, this);

}

void BossPixie::ChangeStateAttackWave(void)
{
	stateUpdate_ = std::bind(&BossPixie::UpdateAttackWave, this);

}

void BossPixie::ChangeStateAttackEnd(void)
{
	stateUpdate_ = std::bind(&BossPixie::UpdateAttackEnd, this);

}

void BossPixie::ChangeStateDamage(void)
{
	stateUpdate_ = std::bind(&BossPixie::UpdateDamage, this);

}

void BossPixie::ChangeStateDown(void)
{
	stateUpdate_ = std::bind(&BossPixie::UpdateDown, this);

}

void BossPixie::ChangeStateEnd(void)
{
	stateUpdate_ = std::bind(&BossPixie::UpdateEnd, this);

}

void BossPixie::UpdateIdle(void)
{
	transform_.pos = player_->GetTransform().pos;

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
