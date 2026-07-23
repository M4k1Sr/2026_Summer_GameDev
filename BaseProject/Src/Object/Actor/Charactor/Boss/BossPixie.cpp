#include <DxLib.h>
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
#include "./BossPixie.h"
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


BossPixie::BossPixie(const BossBase::BossData& data)
	:
	isDead_(false),
	BossBase(data)
{
	ChangeAttackStrategy(std::make_unique<FireBallAttack>());
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
		resMng_.LoadModelDuplicate(ResourceManager::SRC::BOSS_PIXIE));

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
		ColliderBase::TAG::PIXIE, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// モデルとの衝突で使用するモデルコライダー
	ColliderModel* colModel = new ColliderModel(
		ColliderBase::TAG::PIXIE,
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
	animationController_->Add(static_cast<int>(ANIM_TYPE::SURPRISE), 40.0f, Application::PATH_MODEL + "Enemy/Pixie/Surprise.mv1");
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

	// フラグ初期化
	isUnaware_ = true;	// 未発見
	isAlerted_ = false;	// 発見時
	isEngaged_ = false;	// 発見後
	isSearching_ = false;// 捜索中
	
	isAttack_ = false;	// 非攻撃状態

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

	// 初期フェーズ
	phaseStep_ = (PHASE_STEP::PHASE_IDLE);

	health_ = new Health();
	health_->Init(1000);
}

void BossPixie::UpdateProcess(void)
{
	if (!isDead_)
	{
		// 索敵・注視関数
		Search();
		LookPlayer();
	}

	if (player_ == nullptr)
	{
	}

	// 状態別更新
	stateUpdate_();

<<<<<<< HEAD
	//if (CheckHitKey(KEY_INPUT_K)) {
	//	health_->TakeDamage(10);
	//}

	//if (CheckHitKey(KEY_INPUT_K)) {
	//	ChangeState(STATE::DOWN);
	//}


=======
	if (CheckHitKey(KEY_INPUT_K)) {
		health_->TakeDamage(10);
	}
>>>>>>> c705d334f18ebf93ee247c6d6c4234148284325c
}

void BossPixie::UpdateProcessPost(void)
{
	transform_.Update();

	// 一定量のダメージを受けたらフェーズが進む
	Phase();

	BossBase::UpdateProcessPost();

}

void BossPixie::DrawViewRange(void)
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

	//// 視野の描画
	//pos0.y = pos1.y = pos2.y = pos3.y = 10.0f;	// 地面の少し上
	//DrawTriangle3D(pos0, pos2, pos1, 0x0000ff, true);
	//DrawTriangle3D(pos0, pos1, pos3, 0x0000ff, true);
	//DrawLine3D(pos0, pos1, 0xffff00);
	//DrawLine3D(pos0, pos2, 0xffff00);
	//DrawLine3D(pos0, pos3, 0xffff00);

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

void BossPixie::Search(void)
{
	// プレイヤーの位置
	VECTOR playerPos = player_->GetTransform().pos;
	// プレイヤーまでの距離をベクトル演算
	VECTOR toPlayer = VSub(playerPos, transform_.pos);
	// プレイヤーまでの距離を求める
	float distance = VSize(toPlayer);

	// 今プレイヤーが見えているのか
	bool findPlayerNow = (distance < VIEW_RANGE);

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

void BossPixie::LookPlayer(void)
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

void BossPixie::ChangeState(STATE state)
{
	state_ = state;

	// 各状態の初期状態設定
	BossBase::ChangeState(static_cast<int>(state_));

}


void BossPixie::ChangeStateIdle(void)
{
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::IDLE), true);
	stateUpdate_ = std::bind(&BossPixie::UpdateIdle, this);
}

void BossPixie::ChangeStateSurprise(void)
{
	isAlerted_ = false;

	animationController_->Play(
		static_cast<int>(ANIM_TYPE::SURPRISE), false);
	stateUpdate_ = std::bind(&BossPixie::UpdateSurprise, this);
}

void BossPixie::ChangeStateCharge(void)
{
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::CHARGE), false);
	stateUpdate_ = std::bind(&BossPixie::UpdateCharge, this);
}

void BossPixie::ChangeStateThrow(void)
{
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::THROW), false);
	// 5発撃ちたい！
	throwCnt_ = 5; 
	// タイマーリセット
	attackTimer_ = 0;     

	stateUpdate_ = std::bind(&BossPixie::UpdateThrow, this);
}

void BossPixie::ChangeStateAttackWave(void)
{
	if (phaseStep_ == (PHASE_STEP::PHASE_TACTICAL) || phaseStep_ == (PHASE_STEP::PHASE_CLIMAX))
	{
		animationController_->Play(
			static_cast<int>(ANIM_TYPE::ATTACK_WAVE), false);

		// 24発撃ちたい！
		waveAttackCnt_ = 1;
		// タイマーリセット
		attackTimer_ = 0;
	}
	stateUpdate_ = std::bind(&BossPixie::UpdateAttackWave, this);
}

void BossPixie::ChangeStateAttackEnd(void)
{
	// 捜索フラグをここでtrueにすることで、発見後状態の未発見状態になる
	isSearching_ = true;

	animationController_->Play(
		static_cast<int>(ANIM_TYPE::ATTACK_END), false);
	stateUpdate_ = std::bind(&BossPixie::UpdateAttackEnd, this);
}

void BossPixie::ChangeStateDamage(void)
{
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::DAMAGE), false);

	stateUpdate_ = std::bind(&BossPixie::UpdateDamage, this);
}

void BossPixie::ChangeStateDown(void)
{
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::DOWN), false);

	stateUpdate_ = std::bind(&BossPixie::UpdateDown, this);
}

void BossPixie::ChangeStateEnd(void)
{
	// サウンド停止
	ServiceLocator::GetSound().StopEvent(SOUND_ID::SE_ENEMY_FIRE);

	stateUpdate_ = std::bind(&BossPixie::UpdateEnd, this);
}

void BossPixie::UpdateIdle(void)
{
	// 発見したら驚き状態に遷移
	if (isAlerted_) {
		ChangeState(STATE::SURPRISE);
	}
}

void BossPixie::UpdateSurprise(void)
{

	if (animationController_->IsEnd())
	{
		ChangeState(STATE::CHARGE);
	}
	
}

void BossPixie::UpdateCharge(void)
{
	if (animationController_->IsEnd())
	{
		if (phaseStep_ == (PHASE_STEP::PHASE_ENCOUNT))
		{
			ChangeState(STATE::THROW);
		}
		else if (phaseStep_ == (PHASE_STEP::PHASE_TACTICAL))
		{
			ChangeState(STATE::ATTACK_WAVE);
		}
		else if (phaseStep_ == (PHASE_STEP::PHASE_CLIMAX))
		{
			ChangeState(STATE::ATTACK_WAVE);
		}
	}
}

void BossPixie::UpdateThrow(void)
{
	// 攻撃関数を実行
	// 撃つ弾が残っているか
	if (throwCnt_ > 0)
	{
		// カウンタ進行
		attackTimer_++;
		if (attackTimer_ >= 50) {
			if (currentAttack_) {
				currentAttack_->ExecuteAttack(*this);
				//SoundManager::GetInstance().PlayEvent(SOUND_ID::SE_ENEMY_FIRE);
			}
			attackTimer_ = 0; // タイマーリセット
			throwCnt_--;	  // 残弾数を減らす
		}
	}


	isAttack_ = true;

	if (throwCnt_ <= 0 && animationController_->IsEnd())
	{
		// 非攻撃状態に戻す
		isAttack_ = false;
		ChangeState(STATE::ATTACK_END);
	}

}

void BossPixie::UpdateAttackWave(void)
{
	// 攻撃関数を実行
	// 撃つ弾が残っているか
	if (waveAttackCnt_ > 0)
	{
		// カウンタ進行
		attackTimer_++;
		if (attackTimer_ >= 50) {
			if (currentAttack_) {
				currentAttack_->ExecuteAttack(*this);
				ServiceLocator::GetSound().PlayEvent(SOUND_ID::SE_ENEMY_FIRE);
			}
			attackTimer_ = 0; // タイマーリセット
			waveAttackCnt_--;	  // 残弾数を減らす
		}
	}


	isAttack_ = true;


	if (animationController_->IsEnd())
	{
		ChangeState(STATE::ATTACK_END);
	}
}

void BossPixie::UpdateAttackEnd(void)
{
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::CHARGE);
	}
}

void BossPixie::UpdateDamage(void)
{
	// ダメージはギミック作ってから
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::CHARGE);
	}

	// 死亡判定
	if (phaseStep_ == PHASE_STEP::PHASE_DEAD) 
	{
		Dead();
		ChangeState(STATE::DOWN);
	}

}

void BossPixie::UpdateDown(void)
{
}

void BossPixie::UpdateEnd(void)
{
}

void BossPixie::Phase(void)
{

	int damageCnt = player_->GetCurrentCnt();

	// 処理を始める前に今のフェーズを保存
	PHASE_STEP oldPhase = phaseStep_;

	// 前のフレームよりカウントが増えていたら、ダメージ状態へ遷移
	if (damageCnt > lastDamageCnt_)
	{
		ChangeState(STATE::DAMAGE);
	}
	// 今の値を保存して次のフレームの比較に使う
	lastDamageCnt_ = damageCnt;


	// もし未発見フェーズはIDLE状態	
	if (!isUnaware_) {
		phaseStep_ = PHASE_STEP::PHASE_IDLE;
	}

	// HP量に応じてフェーズが進む
	if (damageCnt >= 3) {
		phaseStep_ = PHASE_STEP::PHASE_DEAD;
	}
	else if (damageCnt >= 2) {
		phaseStep_ = PHASE_STEP::PHASE_CLIMAX;
	}
	else if (damageCnt >= 1) {
		phaseStep_ = PHASE_STEP::PHASE_TACTICAL;
	}
	else if (damageCnt >= 0) {
		phaseStep_ = PHASE_STEP::PHASE_ENCOUNT;
	}


	if (phaseStep_ != oldPhase)
	{
		if (phaseStep_ == PHASE_STEP::PHASE_TACTICAL)
		{
			// 中盤フェーズに入ったら、攻撃を「波攻撃」に変更する！
			ChangeAttackStrategy(std::make_unique<WaveAttack>());
		}
		else if (phaseStep_ == PHASE_STEP::PHASE_ENCOUNT)
		{
			// 序盤は火の玉
			ChangeAttackStrategy(std::make_unique<FireBallAttack>());
		}
	}
}


void BossPixie::Dead(void)
{
	if (phaseStep_ == PHASE_STEP::PHASE_DEAD)
	{
		isDead_ = true;
	}
}
