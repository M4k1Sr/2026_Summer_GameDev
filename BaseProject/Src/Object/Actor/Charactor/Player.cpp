#include <DxLib.h>
#include "../../../Utility/AsoUtility.h"
#include "../../../Object/Common/Transform.h"
#include "../../../Manager/Camera.h"
#include "../../../Manager/InputManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Resource.h"
#include "../../../Manager/SoundManager.h"
#include "../Charactor/CharactorBase.h"
#include "../../../Object/Common/AnimationController.h"
#include "../../../Object/Actor/Charactor/Object/ObjectTile.h"
#include "../../../Object/Actor/Charactor/Object/ObjectBossGimmick.h"
#include "../../../Object/Actor/Charactor/Object/ObjectTarai.h"
#include "../../../Object/Actor/Charactor/Object/ObjectBossCage.h"
#include "../../../Object/Actor/Charactor/Object/ObjectManager.h"
#include "../../Collider/ColliderLine.h"
#include "../../Collider/ColliderCapsule.h"
#include "../../Collider/ColliderModel.h"
#include "../../../Renderer/UIRenderer/UIElements/StaminaUI.h"
#include "../../../Renderer/UIRenderer/UIElements/SweatUI.h"
#include "../../../Renderer/UIRenderer/Manager/UIManager.h"
#include "../../../Renderer/UIRenderer/Base/UIBase.h"
#include "../../../Application.h"
#include "Player.h"	
#include "../../../Manager/ServiceLocator.h"
#include "../Weapon/ItemAssets/Bomb.h"

Player::Player(void)
	:
	CharactorBase(),
	isGimmick_(false),
	currentCnt_(0),
	isClear_(false),
	isIronBallHit_(false),
	stamina_(15.0f),
	maxStamina_(15.0f),
	isDash_(false),
	isSlowWalk_(false)
{
	sweatPos_ = transform_.pos;
}

Player::~Player(void)
{
}

void Player::Draw(void)
{
	CharactorBase::Draw();

	ServiceLocator::GetUI().Draw();
	
	// プレイヤー座標
	//DrawFormatString(200, 60, GetColor(0, 0, 0), "Player Pos: X:%.1f, Y:%.1f, Z:%.1f", transform_.pos.x, transform_.pos.y, transform_.pos.z);

}

void Player::Release(void)
{
	// ジャンプ音停止
	ServiceLocator::GetSound().StopEvent(SOUND_ID::SE_JUMP);
	// 駆け足音停止
	ServiceLocator::GetSound().StopEvent(SOUND_ID::SE_MOVE);
	// ダッシュ音停止
	ServiceLocator::GetSound().StopEvent(SOUND_ID::SE_DASH);

}


bool Player::GetDeadFlag(void)
{
	return isDead_;
}

//プレイヤーの死亡判定
void Player::playerDead(void)
{
	//プレイヤーのy座標が-1000.0f以下になったら死亡とする
	if(transform_.pos.y < -1000.0f)
	{
		isDead_ = true;
	}
}

int Player::GetCurrentCnt(void) const
{
	return currentCnt_;
}

void Player::IsClear(void) 
{
	//if (transform_.pos.x > 5060 &&
	//	transform_.pos.x < 5235 &&
	//	transform_.pos.z > -790 &&
	//	transform_.pos.z < -490)
	//{
	//	isClear_ = true;
	//}
}

bool Player::GetClearFlag(void) const
{
	return isClear_;
}

void Player::InitLoad(void)
{
	// 基底クラスのリソースロード
	CharactorBase::InitLoad();

	// モデルのロード
	transform_.SetModel(
		resMng_.Load(ResourceManager::SRC::PLAYER).handleId_);

	// 武器用のコンポジット
	weapon_ = std::make_unique<WeaponComposite>();

	WeaponData bombData = {
		.item = ItemKind::BOMB,
		.damage = 10.0f,
		.pos = MV1GetFramePosition(transform_.modelId, 43),
		.rot = AsoUtility::VECTOR_ZERO,
		.scl = {BOMB_SCL, BOMB_SCL, BOMB_SCL},
		.localPos = BOMB_LOCAL_POS,
		.localRot = BOMB_LOCAL_ROT,
		.ownerModelId = transform_.modelId,
		.ownerFrameIndex = 43,
	};

	weapon_->Add(std::make_unique<Bomb>(bombData));
	
	weapon_->Load();

}

void Player::InitTransform(void)
{
	transform_.scl = VGet(PLAYER_SCALE, PLAYER_SCALE, PLAYER_SCALE);
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(PLAYER_LOCAL_ROT);
	transform_.pos = PLAYER_POS;
	transform_.Update();

}

void Player::InitCollider(void)
{
	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::PLAYER, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
		ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

		// 主に壁や木などの衝突で使用するカプセルコライダ
		ColliderCapsule* colCapsule = new ColliderCapsule(
			ColliderBase::TAG::PLAYER, &transform_,
			COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS,
			COL_CAPSULE_RADIUS);
		ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);

}	

void Player::InitAnimation(void)
{
	// アニメーションコントローラー
	animationController_ =
		new AnimationController(transform_.modelId);

	// アニメーション追加
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), 20.0f, Application::PATH_MODEL + "Player/Idle.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::RUN), 35.0f, Application::PATH_MODEL + "Player/Run.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::FAST_RUN), 35.0f, Application::PATH_MODEL + "Player/FastRun.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::JUMP), 40.0f, Application::PATH_MODEL + "Player/Jump.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::PUSH), 50.0f, Application::PATH_MODEL + "Player/Push.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::THROW), 30.0f, Application::PATH_MODEL + "Player/Throw.mv1");
	// アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::IDLE), true);

}

void Player::InitPost(void)
{
	InitUI();
}

void Player::UpdateProcess(void)
{
	isGravity_ = true;

	// 移動操作
	ProcessMove();

	// ジャンプ処理
	ProcessJump();

	//プレイや死亡判定
	playerDead();

	//プレイヤーのクリア判定
	IsClear();

	// ギミック処理
	ProcessPush();

	// アイテム投擲処理
	ProcessThrow();

}

void Player::UpdateProcessPost(void)
{
}

void Player::InitUI(void)
{
	ServiceLocator::GetUI().AddUIBase(new StaminaUI(&stamina_,&maxStamina_,Vector2(850,500)));
}

void Player::ProcessMove(void)
{
	auto& ins = InputManager::GetInstance();

	if (!isDash_ && stamina_ < maxStamina_)
	{
		stamina_ += STAMINA_DASH_DECREASE * scnMng_.GetDeltaTime();
	}


	// 移動量・方向・ダッシュフラグの初期化
	movePow_ = AsoUtility::VECTOR_ZERO;
	VECTOR dir = AsoUtility::VECTOR_ZERO;
	isDash_ = false;
	isSlowWalk_ = false;

	// 入力処理(キーボード / パッド)
	if (GetJoypadNum() == 0)
	{
		if (ins.IsNew(KEY_INPUT_W)) { dir = AsoUtility::DIR_F; }
		if (ins.IsNew(KEY_INPUT_A)) { dir = AsoUtility::DIR_L; }
		if (ins.IsNew(KEY_INPUT_S)) { dir = AsoUtility::DIR_B; }
		if (ins.IsNew(KEY_INPUT_D)) { dir = AsoUtility::DIR_R; }


		if (ins.IsNew(KEY_INPUT_LSHIFT) && stamina_ > 0) { isDash_ = true; }
	}
	else
	{
		InputManager::JOYPAD_IN_STATE padState = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
		dir = ins.GetDirectionXZAKey(padState.AKeyLX, padState.AKeyLY);

		isDash_ = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L_TRIGGER);
	}

	// 移動量・ベクトルの計算

	float inputLength = VSize(dir);
	bool hasInput = inputLength > 0.0f;

	if (hasInput)
	{
		// 移動スピードの決定
		moveSpeed_ = isDash_ ? SPEED_DASH : SPEED_MOVE;

		// カメラのY軸回転に合わせて移動方向を計算
		Quaternion cameraRot = scnMng_.GetCamera()->GetQuaRotY();
		moveDir_ = Quaternion::PosAxis(cameraRot, dir);
		movePow_ = VScale(moveDir_, moveSpeed_);

		if (inputLength > 1.0f)
		{
			inputLength = 1.0f;
		}

		animationController_->SetPlaySpeed(inputLength);

	}
	else
	{
		// 入力がない場合は通常速度に戻しておく
		animationController_->SetPlaySpeed(1.0f);
	}

	// 動くタイルへの追従と座標の更新
	
	// まず自身の移動量（movePow_）分、座標を進める
	transform_.pos = VAdd(transform_.pos, movePow_);
	transform_.pos = VAdd(transform_.pos, VScale(jumpPow_, scnMng_.GetDeltaTime())); // ジャンプ力も反映

	// タイルに追従する
	if (!isJump_ && objMng_ != nullptr)
	{
		ObjectTile* tile = objMng_->GetTileAt(transform_.pos);
		if (tile != nullptr)
		{
			// タイルの上にしっかり乗っている場合のみ追従
			if (transform_.pos.y >= tile->GetPos().y)
			{
				// タイルの速度分、座標を追従させる
				transform_.pos = VAdd(transform_.pos, tile->GetVelocity());
			}
		}
	}

	if (hasInput && !isJump_)
	{
		if (isDash_)
		{
			if (stamina_ <= maxStamina_) {
				// ダッシュはスタミナ減少
				stamina_ -= STAMINA_DASH_DECREASE * scnMng_.GetDeltaTime();
				if (stamina_ <= 0.0f)
				{
					isDash_ = false;
				}
			}

			// ダッシュに切り替わった瞬間に歩き音を止める
			if (ServiceLocator::GetSound().IsPlaying(SOUND_ID::SE_MOVE))
			{
				ServiceLocator::GetSound().StopEvent(SOUND_ID::SE_MOVE);
			}

			// ダッシュ音が鳴っていなければ再生
			if (!ServiceLocator::GetSound().IsPlaying(SOUND_ID::SE_DASH))
			{
				ServiceLocator::GetSound().PlayEvent(SOUND_ID::SE_DASH);
			}
		}
		else
		{
			// 歩きに切り替わった瞬間にダッシュ音を止める
			if (ServiceLocator::GetSound().IsPlaying(SOUND_ID::SE_DASH))
			{
				ServiceLocator::GetSound().StopEvent(SOUND_ID::SE_DASH);
			}

			// 歩き音が鳴っていなければ再生
			if (!ServiceLocator::GetSound().IsPlaying(SOUND_ID::SE_MOVE))
			{
				ServiceLocator::GetSound().PlayEvent(SOUND_ID::SE_MOVE);
			}
		}
	}
	else
	{
		// 停止時は両方確実に止める
		if (ServiceLocator::GetSound().IsPlaying(SOUND_ID::SE_MOVE))
		{
			ServiceLocator::GetSound().StopEvent(SOUND_ID::SE_MOVE);
		}
		if (ServiceLocator::GetSound().IsPlaying(SOUND_ID::SE_DASH))
		{
			ServiceLocator::GetSound().StopEvent(SOUND_ID::SE_DASH);
		}
	}

	//アニメーション制御
	if (isJump_)
	{
		if (currentAnimType_ != static_cast<int>(ANIM_TYPE::JUMP))
		{
			animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP), false);
			currentAnimType_ = static_cast<int>(ANIM_TYPE::JUMP);
		}
	}
	else
	{
		int nextAnimType = hasInput ? (isDash_ ? static_cast<int>(ANIM_TYPE::FAST_RUN) : static_cast<int>(ANIM_TYPE::RUN))
			: static_cast<int>(ANIM_TYPE::IDLE);

		// 前回と違うアニメーションの時だけ Play を呼ぶ
		if (currentAnimType_ != nextAnimType)
		{
			animationController_->Play(nextAnimType, true);
			currentAnimType_ = nextAnimType;
		}
	}
}

void Player::ProcessJump(void)
{

	auto& ins = InputManager::GetInstance();
	float deltaTime = scnMng_.GetDeltaTime();

	// 入力状態の取得
	// 押した瞬間
	bool isJumpTrg = ins.IsTrgDown(KEY_INPUT_SPACE) ||
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN);

	// 押しっぱなしの継続状態 (Press / New ※InputManagerの仕様に合わせて継続判定の関数にしてください)
	bool isJumpStay = ins.IsNew(KEY_INPUT_SPACE) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN);

	// 初期ジャンプ処理（押した瞬間）
	if (isJumpTrg && !isJump_)
	{
		ServiceLocator::GetSound().PlayEvent(SOUND_ID::SE_JUMP);

		// 接地状態から跳ね上がる瞬間の初速を与える
		jumpPow_ = VScale(AsoUtility::DIR_U, POW_JUMP_INIT);

		isJump_ = true;
		stepJump_ = 0.0f; // タイマーリセット

		// アニメーション再生
		animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP), false);

	}

	// 持続ジャンプ処理
	if (isJump_)
	{
		if (isJumpStay && stepJump_ < TIME_JUMP_INPUT)
		{
			// 時間を進める
			stepJump_ += deltaTime;

			// ボタンを押し続けている間は、ふわっと浮き上がる持続上昇力を加算
			float jumpSpeed = POW_JUMP_KEEP * deltaTime;
			jumpPow_ = VAdd(jumpPow_, VScale(AsoUtility::DIR_U, jumpSpeed));
		}
		else
		{
			// ボタンを離す、または受付時間を過ぎたら持続タイマーを最大にして加算を打ち切る
			stepJump_ = TIME_JUMP_INPUT;
		}


	}

}

void Player::ProcessPush(void)
{
	auto& ins = InputManager::GetInstance();

	// プレイヤーがギミック付近にいる場合
	if (objMng_ != nullptr)
	{

		// ボスの攻撃スイッチギミック
		ObjectBossGimmick* bossGimmick = objMng_->GetBossGimmick(transform_.pos);

		//// タイルの判定
		ObjectTarai* tarai = objMng_->GetTarai(transform_.pos);
		ObjectBossCage* bossCage = objMng_->GetBossCage(transform_.pos);

		if (bossGimmick != nullptr)
		{

			// ギミック処理
			bool isHitKeyNew = ins.IsPress(KEY_INPUT_R)
				|| ins.IsPadBtnPress(
					InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT);

			if (isHitKeyNew) {

				// デルタタイムで秒数をカウンタする
				gimmickCnt_ += scnMng_.GetDeltaTime();

				// アニメーション再生
				animationController_->Play(
					static_cast<int>(ANIM_TYPE::PUSH));

				if (gimmickCnt_ > 5.0f) {

					// ギミック動作オン
					bossGimmick->SetFlag(true);
					currentCnt_++;
					bossGimmick->SetCnt(currentCnt_);
					gimmickCnt_ = 0.0f;	// カウンタをリセットし、再度ギミックを動作させるために準備

					// ギミックすべて(3つ)がオン状態になったらタライのフラグをtrueにする
					// タライギミック作動
					if (tarai) {
						tarai->SetFlag(true);
					}

					// ボスケージ作動
					if (bossCage && currentCnt_ == 5){
						bossCage->SetFlag(true);
					}
				}
				else {
					bossGimmick->SetFlag(false);
				}
			}
			else {
				gimmickCnt_ = 0.0f;	// カウンタをリセットし、再度ギミックを動作させるために準備
			}
			
		}
	}
}

// アイテム投擲処理
void Player::ProcessThrow(void)
{
	auto& ins = InputManager::GetInstance();

	// ギミック処理
	bool isHitKeyNew = ins.IsPress(KEY_INPUT_E)
		|| ins.IsPadBtnPress(
			InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT);

	if (isHitKeyNew) {
		// アニメーション再生
		animationController_->Play(
			static_cast<int>(ANIM_TYPE::THROW));
	}
}

void Player::CollisionReserve(void)
{
	// アニメーションごとの線分調整
	if (animationController_->GetPlayType() == static_cast<int>(ANIM_TYPE::JUMP))
	{
		// ジャンプ中は線分を伸ばす
		if (ownColliders_.count(static_cast<int>(COLLIDER_TYPE::LINE)) != 0)
		{
			ColliderLine* colLine = dynamic_cast<ColliderLine*>(
				ownColliders_.at(static_cast<int>(COLLIDER_TYPE::LINE)));
			colLine->SetLocalPosStart(COL_LINE_JUMP_START_LOCAL_POS);
			colLine->SetLocalPosEnd(COL_LINE_JUMP_END_LOCAL_POS);
		}
	}
	else
	{
		// 通常時の線分に戻す
		if (ownColliders_.count(static_cast<int>(COLLIDER_TYPE::LINE)) != 0)
		{
			ColliderLine* colLine = dynamic_cast<ColliderLine*>(
				ownColliders_.at(static_cast<int>(COLLIDER_TYPE::LINE)));
			colLine->SetLocalPosStart(COL_LINE_START_LOCAL_POS);
			colLine->SetLocalPosEnd(COL_LINE_END_LOCAL_POS);
		}
	}


	// アニメーションごとのカプセル調整
	if (animationController_->GetPlayType() == static_cast<int>(ANIM_TYPE::JUMP))
	{
		// ジャンプ中は線分を伸ばす
		if (ownColliders_.count(static_cast<int>(COLLIDER_TYPE::CAPSULE)) != 0)
		{
			ColliderCapsule* colCapsule = dynamic_cast<ColliderCapsule*>(
				ownColliders_.at(static_cast<int>(COLLIDER_TYPE::CAPSULE)));
			colCapsule->SetLocalPosTop(COL_CAPSULE_TOP_JUMP_LOCAL_POS);
			colCapsule->SetLocalPosDown(COL_CAPSULE_DOWN_JUMP_LOCAL_POS);
		}
	}
	else
	{
		// 通常時のカプセルに戻す
		if (ownColliders_.count(static_cast<int>(COLLIDER_TYPE::CAPSULE)) != 0)
		{
			ColliderCapsule* colCapsule = dynamic_cast<ColliderCapsule*>(
				ownColliders_.at(static_cast<int>(COLLIDER_TYPE::CAPSULE)));
			colCapsule->SetLocalPosTop(COL_CAPSULE_TOP_LOCAL_POS);
			colCapsule->SetLocalPosDown(COL_CAPSULE_DOWN_LOCAL_POS);
			colCapsule->SetRadius(COL_CAPSULE_RADIUS);
		}
	}
}