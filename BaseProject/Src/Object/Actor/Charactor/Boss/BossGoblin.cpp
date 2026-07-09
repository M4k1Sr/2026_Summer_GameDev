#include <DxLib.h>
#include <memory>
#include "./BossBase.h"
#include "../../Attack/StrategyAttack.h"
#include "../../../../Utility/AsoUtility.h"
#include "../../../../Utility/MatrixUtility.h"
#include "../../../../Common/Quaternion.h"
#include "../../../../Manager/InputManager.h"
#include "../../../../Manager/SceneManager.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/Resource.h"
#include "../../../../Manager/SoundManager.h"
#include "../../../../Object/Common/Transform.h"
#include "./BossGoblin.h"
#include "../../../../Object/Common/Health.h"
#include "../../../../Object/Common/AnimationController.h"
#include "../Player.h"
#include "../../../Collider/ColliderLine.h"
#include "../../../Collider/ColliderCapsule.h"
#include "../../../Collider/ColliderModel.h"
#include "../../../../Application.h"
#include "../Object/ObjectBossGimmick.h"
#include "../Object/ObjectManager.h"
#include "../../../../Manager/ServiceLocator.h"
#include "../../Weapon/HitBox.h"


BossGoblin::BossGoblin(const BossBase::BossData& data)
	:
	isDead_(false),
	BossBase(data)
{
	idleTimer_ = 0.0f;
}

BossGoblin::~BossGoblin(void)
{
}

void BossGoblin::InitLoad(void)
{
	// 基底クラスのリソースロード
	BossBase::InitLoad();

	// モデル読み込み
	transform_.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::BOSS_GOBLIN));

	// 武器用のコンポジット
	weapon_ = std::make_unique<WeaponComposite>();

	WeaponData clubData = {
		.type = WeaponType::ONE_HAND,
		.kind = WeaponKind::Club,
		.damage = 10.0f,
		.criticalRate = 0.05f,
		.criticalBonus = 1.5f,
		.pos = MV1GetFramePosition(transform_.modelId, 43),
		.rot = AsoUtility::VECTOR_ZERO,
		.scl = {WEAPON_SCL, WEAPON_SCL, WEAPON_SCL},
		.localPos = WEAPON_LOCAL_POS,
		.localRot = WEAPON_LOCAL_ROT,
		.ownerModelId = transform_.modelId,
		.ownerFrameIndex = 43,
	};

	weapon_->Add(std::make_unique<HitBox>(clubData));

	weapon_->Load();

}

void BossGoblin::InitTransform(void)
{

	// モデルの大きさ、回転、座標の初期化
	transform_.scl = VGet(SCALE, SCALE, SCALE);
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.Update();
}

void BossGoblin::InitCollider(void)
{

	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::GOBLIN, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// モデルとの衝突で使用するモデルコライダー
	ColliderModel* colModel = new ColliderModel(
		ColliderBase::TAG::GOBLIN,
		&transform_);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), colModel);
}

void BossGoblin::InitAnimation(void)
{
	//モデルアニメーション制御の初期化
	animationController_ =
		new AnimationController(transform_.modelId);

	// アニメーション追加
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), 20.0f, Application::PATH_MODEL + "Enemy/Goblin/Idle.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::YAWN), 20.0f, Application::PATH_MODEL + "Enemy/Goblin/Yawn.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE_JUMP), 20.0f, Application::PATH_MODEL + "Enemy/Goblin/IdleJump.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::SIT), 20.0f, Application::PATH_MODEL + "Enemy/Goblin/Sit.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::WALK), 20.0f, Application::PATH_MODEL + "Enemy/Goblin/Walk.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::RUN), 20.0f, Application::PATH_MODEL + "Enemy/Goblin/Run.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::PATROL), 20.0f, Application::PATH_MODEL + "Enemy/Goblin/Patrol.mv1");
	
	animationController_->Add(static_cast<int>(ANIM_TYPE::SURPRISE), 40.0f, Application::PATH_MODEL + "Enemy/Goblin/Surprised.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::THREAT), 20.0f, Application::PATH_MODEL + "Enemy/Goblin/Threat.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK), 20.0f, Application::PATH_MODEL + "Enemy/Goblin/Attack.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::THROW), 30.0f, Application::PATH_MODEL + "Enemy/Goblin/Throw.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ANGRY), 20.0f, Application::PATH_MODEL + "Enemy/Goblin/Angry.mv1");
	
	animationController_->Add(static_cast<int>(ANIM_TYPE::DAMAGE), 20.0f, Application::PATH_MODEL + "Enemy/Goblin/Damage.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::DOWN), 20.0f, Application::PATH_MODEL + "Enemy/Goblin/Down.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::END), 20.0f, Application::PATH_MODEL + "Enemy/Goblin/End.mv1");

	// アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::IDLE), true);

}

void BossGoblin::InitPost(void)
{
	// 基底クラスの初期化後処理
	stateTimer_ = 0.0f;
	stateTime_ = 0.0f;

	// フラグ初期化
	isUnaware_ = true;	// 未発見
	isAlerted_ = false;	// 発見時
	isEngaged_ = false;	// 発見後
	isSearching_ = false;// 捜索中

	isAttack_ = false;	// 非攻撃状態

	// 初期遷移状態初期処理登録
	stateChanges_.emplace(static_cast<int>(STATE::IDLE),
		std::bind(&BossGoblin::ChangeStateIdle, this));
	stateChanges_.emplace(static_cast<int>(STATE::YAWN),
		std::bind(&BossGoblin::ChangeStateYawn, this));
	stateChanges_.emplace(static_cast<int>(STATE::IDLE_JUMP),
		std::bind(&BossGoblin::ChangeStateIdleJump, this));
	stateChanges_.emplace(static_cast<int>(STATE::SIT),
		std::bind(&BossGoblin::ChangeStateSit, this));
	stateChanges_.emplace(static_cast<int>(STATE::WALK),
		std::bind(&BossGoblin::ChangeStateWalk, this));
	stateChanges_.emplace(static_cast<int>(STATE::RUN),
		std::bind(&BossGoblin::ChangeStateRun, this));
	stateChanges_.emplace(static_cast<int>(STATE::PATROL),
		std::bind(&BossGoblin::ChangeStatePatrol, this));
	stateChanges_.emplace(static_cast<int>(STATE::SURPRISE),
		std::bind(&BossGoblin::ChangeStateSurprise, this));
	stateChanges_.emplace(static_cast<int>(STATE::THREAT),
		std::bind(&BossGoblin::ChangeStateThreat, this));
	stateChanges_.emplace(static_cast<int>(STATE::ATTACK),
		std::bind(&BossGoblin::ChangeStateAttack, this));
	stateChanges_.emplace(static_cast<int>(STATE::THROW),
		std::bind(&BossGoblin::ChangeStateThrow, this));
	stateChanges_.emplace(static_cast<int>(STATE::ANGRY),
		std::bind(&BossGoblin::ChangeStateAngry, this));
	stateChanges_.emplace(static_cast<int>(STATE::DAMAGE),
		std::bind(&BossGoblin::ChangeStateDamage, this));
	stateChanges_.emplace(static_cast<int>(STATE::DOWN),
		std::bind(&BossGoblin::ChangeStateDown, this));
	stateChanges_.emplace(static_cast<int>(STATE::END),
		std::bind(&BossGoblin::ChangeStateEnd, this));

	// 初期状態設定
	ChangeState(STATE::IDLE);

	// 初期フェーズ
	phaseStep_ = (PHASE_STEP::PHASE_IDLE);

	health_ = new Health();
	health_->Init(1000);
}

void BossGoblin::UpdateProcess(void)
{
	if (!isDead_)
	{
		// 索敵・注視関数
		Search();
		//LookPlayer();
	}

	if (player_ == nullptr)
	{
	}

	// 時間取得
	idleTimer_ = SceneManager::GetInstance().GetDeltaTime();

	// 状態別更新
	stateUpdate_();

}

void BossGoblin::UpdateProcessPost(void)
{
	transform_.Update();

	// 一定量のダメージを受けたらフェーズが進む
	Phase();

	BossBase::UpdateProcessPost();

}

void BossGoblin::DrawViewRange(void)
{
	// 三角形の視野
	float viewRad = AsoUtility::Deg2RadF(VIEW_ANGLE);

	// オイラー角に変換
	VECTOR angles_ = Quaternion::ToEuler(transform_.quaRot);
	VECTOR localAngles_ = Quaternion::ToEuler(transform_.quaRotLocal);

	// 向き角度から方向を取得
	MATRIX mat = MGetIdent();
	VECTOR totalAngles = VAdd(angles_, localAngles_);
	mat = MatrixUtility::GetMatrixRotateXYZ(totalAngles);

	// 前方方向
	VECTOR forward = VTransform(AsoUtility::DIR_B, mat);

	// 右方向
	MATRIX rightMat = MMult(mat, MGetRotY(AsoUtility::Deg2RadF(VIEW_ANGLE)));
	VECTOR right = VTransform(AsoUtility::DIR_B, rightMat);

	// 左方向
	MATRIX leftMat = MMult(mat, MGetRotY(AsoUtility::Deg2RadF(-VIEW_ANGLE)));
	VECTOR left = VTransform(AsoUtility::DIR_B, leftMat);

	// 自分の位置
	VECTOR pos0 = transform_.pos;

	// 正面の位置
	VECTOR pos1 = VAdd(pos0, VScale(forward, VIEW_RANGE));

	// 正面から半時計周り
	VECTOR pos2 = VAdd(pos0, VScale(left, VIEW_RANGE));

	// 正面から時計回り
	VECTOR pos3 = VAdd(pos0, VScale(right, VIEW_RANGE));

	// 視野の描画
	pos0.y = pos1.y = pos2.y = pos3.y = 10.0f;	// 地面の少し上
	DrawTriangle3D(pos0, pos2, pos1, 0x0000ff, true);
	DrawTriangle3D(pos0, pos1, pos3, 0x0000ff, true);
	DrawLine3D(pos0, pos1, 0xffff00);
	DrawLine3D(pos0, pos2, 0xffff00);
	DrawLine3D(pos0, pos3, 0xffff00);

	std::string phaseName = "";

	// 列挙型を分かりやすい文字列に変換する
	switch (phaseStep_)
	{
	case PHASE_STEP::PHASE_IDLE:     phaseName = "IDLE (未発見)"; break;
	case PHASE_STEP::PHASE_ENCOUNT:  phaseName = "ENCOUNT (序盤)"; break;
	case PHASE_STEP::PHASE_TACTICAL: phaseName = "TACTICAL (中盤)"; break;
	case PHASE_STEP::PHASE_CLIMAX:   phaseName = "CLIMAX (終盤)"; break;
	case PHASE_STEP::PHASE_DEAD:     phaseName = "DEAD (死亡)"; break;
	default:                         phaseName = "UNKNOWN (エラー)"; break;
	}
	//DrawFormatString(300, 100, GetColor(255, 0, 0), "Boss Phase: %s", phaseName.c_str());

	//	DrawFormatString(300, 100, GetColor(255, 0, 0), "Boss Phase: %s", phaseName.c_str());

	//DrawFormatString(300, 100, GetColor(255, 0, 0), "Boss Phase: %s", phaseName.c_str());


}

void BossGoblin::Search(void)
{
	// プレイヤーの位置
	VECTOR playerPos = player_->GetTransform().pos;
	// プレイヤーまでの距離をベクトル演算
	VECTOR toPlayer = VSub(playerPos, transform_.pos);
	// プレイヤーまでの距離を求める
	float distance = VSize(toPlayer);

	// 今プレイヤーが見えているのか
	bool findPlayerNow = (distance < VIEW_RANGE);


	if (findPlayerNow) {
		LookPlayer();
	}

	// 視野範囲
	// 未発見状態
	if (state_ == STATE::IDLE && findPlayerNow)
	{
		// ボスの視野範囲に入った	
		isAlerted_ = true;
		// プレイヤーの所在に気づいている(二回目以降はずっと気づいている状態なのでfalse)
		isUnaware_ = false;
	}

	// 発見後状態の未発見状態
	if (isSearching_ && findPlayerNow)
	{
		// ボスの視野範囲に入った
		isAlerted_ = true;
	}
}

void BossGoblin::LookPlayer(void)
{
	//プレイヤー（相手）の座標を取得
	VECTOR playerPos = player_->GetTransform().pos; //プレイヤー座標

	//ベクトルを求める
	VECTOR diff = VSub(playerPos, transform_.pos);
	diff.y = 0.0f;

	//ベクトルを正規化(これで方向を取得する)
	moveDir_ = VNorm(diff);

	// オイラー角に変換
	VECTOR angles_ = Quaternion::ToEuler(transform_.quaRotLocal);

	//Y軸回転の計算（XZ平面上の角度を求める）
	angles_.y = atan2(moveDir_.x, moveDir_.z);

	//回転はY軸のみ
	angles_.x = angles_.z = 0.0f;

	//向きを設定
	MV1SetRotationXYZ(transform_.modelId, angles_);

}

void BossGoblin::ChangeState(STATE state)
{
	state_ = state;

	// 各状態の初期状態設定
	BossBase::ChangeState(static_cast<int>(state_));

}

void BossGoblin::ChangeStateIdle(void)
{
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::IDLE), true);
	stateUpdate_ = std::bind(&BossGoblin::UpdateIdle, this);

}

void BossGoblin::ChangeStateYawn(void)
{
}

void BossGoblin::ChangeStateIdleJump(void)
{
}

void BossGoblin::ChangeStateSit(void)
{
}

void BossGoblin::ChangeStateWalk(void)
{
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::WALK), true);
	stateUpdate_ = std::bind(&BossGoblin::UpdateWalk, this);

}

void BossGoblin::ChangeStateRun(void)
{
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::RUN), true);
	stateUpdate_ = std::bind(&BossGoblin::UpdateRun, this);

}

void BossGoblin::ChangeStatePatrol(void)
{
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::PATROL), true);
	stateUpdate_ = std::bind(&BossGoblin::UpdatePatrol, this);

}

void BossGoblin::ChangeStateSurprise(void)
{
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::SURPRISE), true);
	stateUpdate_ = std::bind(&BossGoblin::UpdateSurprise, this);

}

void BossGoblin::ChangeStateThreat(void)
{
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::THREAT), true);
	stateUpdate_ = std::bind(&BossGoblin::UpdateThreat, this);

}

void BossGoblin::ChangeStateAttack(void)
{
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::ATTACK), true);
	stateUpdate_ = std::bind(&BossGoblin::UpdateAttack, this);

}

void BossGoblin::ChangeStateThrow(void)
{
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::THROW), true);
	stateUpdate_ = std::bind(&BossGoblin::UpdateThrow, this);

}

void BossGoblin::ChangeStateAngry(void)
{
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::ANGRY), true);
	stateUpdate_ = std::bind(&BossGoblin::UpdateAngry, this);

}

void BossGoblin::ChangeStateDamage(void)
{
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::DAMAGE), true);
	stateUpdate_ = std::bind(&BossGoblin::UpdateDamage, this);

}

void BossGoblin::ChangeStateDown(void)
{
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::DOWN), true);
	stateUpdate_ = std::bind(&BossGoblin::UpdateDown, this);

}

void BossGoblin::ChangeStateEnd(void)
{
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::END), true);
	stateUpdate_ = std::bind(&BossGoblin::UpdateEnd, this);

}

void BossGoblin::UpdateIdle(void)
{
	if ((idleTimer_ / 5.0f) == 0)
	{
		ChangeState(STATE::YAWN);
	}
}

void BossGoblin::UpdateYawn(void)
{

}

void BossGoblin::UpdateIdleJump(void)
{

}

void BossGoblin::UpdateSit(void)
{

}

void BossGoblin::UpdateWalk(void)
{
}

void BossGoblin::UpdateRun(void)
{
}

void BossGoblin::UpdatePatrol(void)
{
}

void BossGoblin::UpdateSurprise(void)
{
}

void BossGoblin::UpdateThreat(void)
{
}

void BossGoblin::UpdateAttack(void)
{
}

void BossGoblin::UpdateThrow(void)
{
}

void BossGoblin::UpdateAttackEnd(void)
{
}

void BossGoblin::UpdateCheer(void)
{
}

void BossGoblin::UpdateAngry(void)
{
}

void BossGoblin::UpdateDamage(void)
{
}

void BossGoblin::UpdateDown(void)
{
}

void BossGoblin::UpdateEnd(void)
{
}
void BossGoblin::Phase(void)
{

	//int damageCnt = player_->GetCurrentCnt();

	//// 処理を始める前に今のフェーズを保存
	//PHASE_STEP oldPhase = phaseStep_;

	//// ★【追加】前のフレームよりカウントが増えていたら、ダメージ状態へ遷移
	//if (damageCnt > lastDamageCnt_)
	//{
	//	ChangeState(STATE::DAMAGE);
	//}
	//// 今の値を保存して次のフレームの比較に使う
	//lastDamageCnt_ = damageCnt;


	//// もし未発見フェーズはIDLE状態	
	//if (!isUnaware_) {
	//	phaseStep_ = PHASE_STEP::PHASE_IDLE;
	//}

	//// HP量に応じてフェーズが進む
	//if (damageCnt >= 3) {
	//	phaseStep_ = PHASE_STEP::PHASE_DEAD;
	//}
	//else if (damageCnt >= 2) {
	//	phaseStep_ = PHASE_STEP::PHASE_CLIMAX;
	//}
	//else if (damageCnt >= 1) {
	//	phaseStep_ = PHASE_STEP::PHASE_TACTICAL;
	//}
	//else if (damageCnt >= 0) {
	//	phaseStep_ = PHASE_STEP::PHASE_ENCOUNT;
	//}


	//if (phaseStep_ != oldPhase)
	//{
	//	if (phaseStep_ == PHASE_STEP::PHASE_TACTICAL)
	//	{
	//		// 中盤フェーズに入ったら、攻撃を「波攻撃」に変更する！
	//		ChangeAttackStrategy(std::make_unique<WaveAttack>());
	//	}
	//	else if (phaseStep_ == PHASE_STEP::PHASE_ENCOUNT)
	//	{
	//		// 序盤は火の玉
	//		ChangeAttackStrategy(std::make_unique<FireBallAttack>());
	//	}
	//	}
}


	void BossGoblin::Dead(void)
{
	if (phaseStep_ == PHASE_STEP::PHASE_DEAD)
	{
		isDead_ = true;
	}
}
