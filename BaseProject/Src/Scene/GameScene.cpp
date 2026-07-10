#pragma once
#include <DxLib.h>
#include <vector>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "../Object/Common/AnimationController.h"
#include"../Manager/SoundManager.h"
#include "../Object/Actor/StageBase.h"
#include "../Object/Actor/Stage1.h"
#include "../Object/Actor/Stage2.h"
#include "../Object/Actor/Stage.h"
#include"../Renderer/EffectRenderer/Manager/EffectManager.h"
#include "../Object/Actor/SkyDome.h"
#include"../Ranking/GameData.h"
#include "../Object/Actor/IronBall.h"
#include "../Object/Actor/Charactor/Player.h"
#include "../Object/Actor/Charactor/Boss/BossManager.h"
#include "../Object/Actor/Attack/AttackManager.h"
#include "../Object/Actor/Charactor/Object/ObjectManager.h"
#include"../Ranking/Ranking.h"
#include"../Renderer/UIRenderer/UIElements/Clock.h"
#include "../Renderer/UIRenderer/Manager/UIManager.h"
#include "GameScene.h"
#include "../Application.h"
#include"../Manager/ItemManager.h"
#include<EffekseerForDXLib.h>
#include "../Manager/ServiceLocator.h"

GameScene::GameScene(void)
	:
	stage_(nullptr),
	skyDome_(nullptr),
	player_(nullptr),
	ironBall_(nullptr),
	clockUI_(nullptr),
	bossMng_(nullptr),
	objMng_(nullptr),
	attackMng_(nullptr),
	rank_(nullptr),
	itemMng_(nullptr),
	isPause_(false),
	pauseImg_(-1),
	isSousa_(false),
	sousaImg_(-1),
	mosPosX_(0),
	mosPosY_(0),
	isEnd_(false),
	isClear_(false),
	goalImg_(-1),
	ButtonUIImg_(-1),
	clearTime_(0),
	currentStageNum_(1),
	fadeAlpha_(0),
	fadeSpeed_(5),
	fadeState_(FadeState::NONE),
	stageState_(StageState::STAGE_1),
	SceneBase()
{
}

GameScene::~GameScene(void)
{
	ServiceLocator::GetSound().StopEvent(SOUND_ID::BGM_STAGE1);
	Release();
}

void GameScene::Init(void)
{
	// オブジェクト初期化
	objMng_ = new ObjectManager();
	objMng_->Init();

	// プレイヤー初期化
	player_ = new Player();
	player_->Init();
	player_->SetObjectManager(objMng_);

	//ステージの状態ごとの初期化
	switch (stageState_)
	{
	case StageState::STAGE_1:
		stage_ = new Stage1();
		stage_->Init();
		break;

	case StageState::STAGE_2:
		stage_ = new Stage2();
		stage_->Init();
		break;
	}

	//画像ロード
	goalImg_ = resMng_.Load(ResourceManager::SRC::GOAL).handleId_;
	if(goalImg_ == -1)
	{
		// 画像のロードに失敗した場合のエラーハンドリング
		MessageBoxA(nullptr, "ゴール画像のロードに失敗しました。", "エラー", MB_OK | MB_ICONERROR);
	}

	rank_->CreateIns();

	// ステージモデルのコライダーをプレイヤーに登録
	const ColliderBase* stageCollider =
		stage_->GetOwnCollider(static_cast<int>(StageBase::COLLIDER_TYPE::MODEL));
	player_->AddHitCollider(stageCollider);

	// スカイドーム初期化
	skyDome_ = new SkyDome(player_->GetTransform());
	skyDome_->Init();

	// オブジェクト(全て)のコライダーを登録
	const std::vector<ObjectBase*>& objects = objMng_->GetObjects();
	for (const auto& obj : objects)
	{
		// オブジェクト追加
		obj->SetObjectManager(objMng_);

		// オブジェクトがモデルコライダーを持っていれば登録
		const ColliderBase* objectCollider = 
		obj->GetOwnCollider(static_cast<int>(ObjectBase::COLLIDER_TYPE::MODEL));
		if (objectCollider != nullptr)
		{
			player_->AddHitCollider(objectCollider);
		}

	}

	// ステージモデルのコライダーをオブジェクトに登録
	objMng_->AddHitCollider(stageCollider);


	// 攻撃処理初期化
	attackMng_ = new AttackManager(objMng_);	// オブジェクトマネージャを渡して攻撃オブジェクト生成
	attackMng_->Init();

	// ボス初期化
	bossMng_ = new BossManager();
	bossMng_->SetPlayer(player_);
	bossMng_->Init();

	// ボス(全て)のコライダーを登録
	const std::vector<BossBase*>& bosses = bossMng_->GetBosses();
	for (const auto& boss : bosses)
	{
		boss->SetObjectManager(objMng_);	// オブジェクトマネージャを渡してオブジェクト生成
		boss->SetAttackManager(attackMng_);	// 攻撃マネージャを渡して攻撃オブジェクト生成

		// ボスがモデルコライダーを持っていれば登録
		const ColliderBase* bossCollider =
			boss->GetOwnCollider(static_cast<int>(ObjectBase::COLLIDER_TYPE::MODEL));
		if (bossCollider != nullptr)
		{
			player_->AddHitCollider(bossCollider);
		}
	}

	// ステージモデルのコライダーをボスに登録
	bossMng_->AddHitCollider(stageCollider);

	// 鉄球モデル
	ironBall_ = new IronBall();
	ironBall_->Init();

	// 鉄球のコライダをプレイヤーに登録（鉄球を検出させる）
	{
		const auto& ironColls = ironBall_->GetOwnColliders();
		for (const auto& kv : ironColls)
		{
			if (kv.second != nullptr)
			{
				player_->AddHitCollider(kv.second);
			}
		}
	}

	ButtonUIImg_ = resMng_.Load(ResourceManager::SRC::PUSH_BUTTON).handleId_;

	// UIモデル
	clockUI_ = new Clock();

	// カメラモード変更
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(&player_->GetTransform());
	camera->AddHitCollider(stageCollider);
	camera->ChangeMode(Camera::MODE::SCROLL_FOLLOW);

}

void GameScene::Update(void)
{

	auto const& ins = InputManager::GetInstance();
	
	// ESC(START)押下時ポーズ画面に遷移 
	if (ins.IsTrgDown(KEY_INPUT_ESCAPE) ||
		ins.IsPadBtnTrgDown(
			InputManager::JOYPAD_NO::PAD1,
			InputManager::JOYPAD_BTN::START))
	{
		isPause_ = !isPause_;	// ポーズのON/OFFの切り替え
	}

	// ポーズ画面中はゲームを静止させる
	if (!isPause_)
	{
		// ★ 1. まずフェードの更新を行う
		bool isFadeFinished = UpdateFade();

		// ★ 2. フェードアウト完了時のステージ切り替え処理
		if (isFadeFinished && fadeAlpha_ == 255)
		{
			if (stageState_ == StageState::STAGE_1)
			{
				// 1. 旧ステージモデルを安全に削除
				if (stage_ != nullptr)
				{
					stage_->Release();
					delete stage_;
					stage_ = nullptr; // deleteした後はnullptrを入れておくのが安全
				}

				// 2. 新ステージの生成
				stage_ = new Stage2();
				stage_->Init();

				// 3. プレイヤーの移動
				VECTOR stage2StartPos = VGet(-800.0f, 0.0f, 700.0f);
				player_->SetPosition(stage2StartPos);

				// 4. コライダーの再登録
				const ColliderBase* stageCollider = stage_->GetOwnCollider(static_cast<int>(StageBase::COLLIDER_TYPE::MODEL));

				// ※ 本来はここで player_ や camera の「古いコライダー登録」をクリアする関数を呼ぶのがベストです
				player_->AddHitCollider(stageCollider);
				objMng_->AddHitCollider(stageCollider);
				bossMng_->AddHitCollider(stageCollider);

				// カメラのコライダーを更新
				Camera* camera = SceneManager::GetInstance().GetCamera();
				// camera->ClearHitCollider(); // もし古いコライダーを外す関数があれば呼ぶ
				camera->AddHitCollider(stageCollider);

				// ステージ状態をSTAGE_2にして、フェードインを開始する
				stageState_ = StageState::STAGE_2;
				StartFade(FadeState::FADE_IN, 5);
			}
		}

		// ★ 3. 【重要】フェード中（NONE 以外）は、以下のゲーム本編の更新をスキップする！
		if (fadeState_ != FadeState::NONE)
		{
			// フェード中（暗転・明転アニメーション中）はゲームを動かさない
			return;
		}

		// --- ここから下は通常時（FadeState::NONE）のみ実行される ---
		//エフェクト
	//	EffectManager::GetInstance().Update();

		//クリアタイム加算
		clearTime_++;
		SetMouseDispFlag(false);

		skyDome_->Update();
		player_->Update();
		//ui_->Update();

		switch (stageState_)
		{
		case GameScene::StageState::STAGE_1:
			stage_->Update();
			bossMng_->Update();
			objMng_->Update();
			attackMng_->Update();
			ironBall_->Update();
			break;
		case GameScene::StageState::STAGE_2:
			stage_->Update();
			break;
		}

		clockUI_->Update();

		// ===========================================================
		//  ボスの生存状態と距離によるカメラモードの自動切り替え
		// ===========================================================
		Camera* camera = SceneManager::GetInstance().GetCamera();

		// 1. まず最優先で「タライが落ちてきているか」をチェック
		if (objMng_->IsTaraiFalling() && bossMng_->GetBosses().size() > 0 && bossMng_->GetBosses().front() != nullptr)
		{
			// 最初のボス、または一番近いボスをターゲットに設定
			BossBase* boss = bossMng_->GetBosses().front();
			camera->SetLockOnTarget(&boss->GetTransform());
			camera->ChangeMode(Camera::MODE::BOSS_FRONT);
		}
		else
		{
			// 2. タライが落ちていない場合は、これまでのロックオン判定を行う
			const std::vector<BossBase*>& bosses = bossMng_->GetBosses();
			BossBase* nearestBoss = nullptr;
			float minDistanceSq = 2500.0f * 2500.0f;
			VECTOR playerPos = player_->GetTransform().pos;

			for (auto* boss : bosses)
			{
				if (boss == nullptr) continue;
				if (boss->GetIsDead()) continue;
				float distSq = VSquareSize(VSub(boss->GetTransform().pos, playerPos));
				if (distSq < minDistanceSq)
				{
					minDistanceSq = distSq;
					nearestBoss = boss;
				}
			}

			if (nearestBoss != nullptr)
			{
				camera->SetLockOnTarget(&nearestBoss->GetTransform());
				camera->ChangeMode(Camera::MODE::LOCK_ON);
			}
			else
			{
				camera->ChangeMode(Camera::MODE::SCROLL_FOLLOW);
			}
		}
		// ===========================================================
		
		// ゲームクリア判定
		IsClear();
	}


	//ゲームオーバー判定
	isEnd_ = clockUI_->GetIsGameOver();
	isEnd_ = player_->GetDeadFlag();

	//ゲームオーバーシーンへ遷移
	if (isEnd_ )
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}

}

void GameScene::Draw(void)
{
	//// スカイドーム描画
	//skyDome_->Draw();

	// ステージごとの描画
	switch (stageState_)
	{
	case GameScene::StageState::STAGE_1:
		stage_->Draw();
		ironBall_->Draw();
		objMng_->Draw();
		bossMng_->Draw();
		attackMng_->Draw();
		DrawBillboard3D(VGet(5060.0f, 0.0f, -490.0f), 0.5f, 0.5f, 400.0f, 0.0f, goalImg_, TRUE);
		break;
	case GameScene::StageState::STAGE_2:
		stage_->Draw();
		break;
	}
	
	if(bossMng_->IsBossDead()){
		//DrawBillboard3D(VGet(5060.0f, 0.0f, -490.0f), 0.5f, 0.5f, 400.0f, 0.0f, goalImg_, TRUE);
	}

	// 鉄球描画
	ironBall_->Draw();
	
	// オブジェクト描画
	objMng_->Draw();

	// プレイヤー描画
	player_->Draw();	

	// プレイヤーが近くにあるボスギミックを取得
	ObjectBossGimmick* bossGimmick =
		objMng_->GetBossGimmick(player_->GetTransform().pos);

	if (bossGimmick != nullptr)
	{
		VECTOR pos = bossGimmick->GetTransform().pos;

		DrawBillboard3D(
			VAdd(pos, VGet(0.0f, 250.0f, 0.0f)), // ボタンの少し上
			0.5f,
			0.5f,
			150.0f,
			0.0f,
			ButtonUIImg_,
			TRUE);
	}
    
	// UI描画
	clockUI_->Draw();

	// 作成したフェード関数を呼び出す（UIの上に黒を被せる）
	DrawFade();

	//カメラのデバッグ用座標表示
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->DrawDebug();

	//ポーズ画面
	IsPause();
	
}

void GameScene::Release(void)
{
	// ステージ解放
	stage_->Release();
	delete stage_;

	// スカイドーム解放
	skyDome_->Release();
	delete skyDome_;

	// オブジェクト解放
	objMng_->Release();
	delete objMng_;


	// プレイヤー解放
	player_->Release();
	delete player_;

	// ボス解放
	bossMng_->Release();
	delete bossMng_;

	// 攻撃処理解放
	attackMng_->Release();
	delete attackMng_;

	

	ironBall_->Release();
	delete ironBall_;

	//// ItemManager解放
	//itemMng_->Release();
	//delete itemMng_;

	DeleteGraph(pauseImg_);

	DeleteGraph(sousaImg_);
}

void GameScene::IsPause(void)
{

	if (isPause_)
	{
		// 透過背景
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		SetFontSize(64);

		DrawBox(400, 200, 1600, 400, 0xffffff, false);
		DrawFormatString(670, 270, 0xffffff, "ゲームを続けますか?");

		DrawBox(400, 600, 1600, 800, 0xffffff, false);
		DrawFormatString(670, 670, 0xffffff, "タイトルに戻りますか?");

		GetMousePoint(&mosPosX_, &mosPosY_);

		//マウスポインタを表示状態にする
		SetMouseDispFlag(TRUE);

		//パッド取得
		auto& ins = InputManager::GetInstance();

		auto pad =
			ins.GetJPadInputState(
				InputManager::JOYPAD_NO::PAD1);

		// 左スティック取得
		float dx = pad.AKeyLX / 1000.0f;
		float dy = pad.AKeyLY / 1000.0f;

		// デッドゾーン
		if (fabsf(dx) < 0.2f)
		{
			dx = 0.0f;
		}

		if (fabsf(dy) < 0.2f)
		{
			dy = 0.0f;
		}

		// カーソル速度
		const int speed = 15;

		// カーソル移動
		mosPosX_ += static_cast<int>(dx * speed);
		mosPosY_ += static_cast<int>(dy * speed);

		// 画面外に出さない
		mosPosX_ = std::clamp(
			mosPosX_,
			0,
			Application::SCREEN_SIZE_X);

		mosPosY_ = std::clamp(
			mosPosY_,
			0,
			Application::SCREEN_SIZE_Y);

		// 実際のマウスカーソルを移動
		SetMousePoint(
			mosPosX_,
			mosPosY_);

		//マウスポインタの座標を取得
		GetMousePoint(&mosPosX_, &mosPosY_);

		

		//この中にマウスカーソルがあるかを判定
		bool continueGame =
			(mosPosX_ >= DRAWBOX_SX && mosPosX_ <= DRAWBOX_EX &&
				mosPosY_ >= DRAWBOX_GAME_SY && mosPosY_ <= DRAWBOX_GAME_EY);

		bool exitGame =
			(mosPosX_ >= DRAWBOX_SX && mosPosX_ <= DRAWBOX_EX &&
				mosPosY_ >= DRAWBOX_GAMEEND_SY && mosPosY_ <= DRAWBOX_GAMEEND_EY);
		//マウスカーソルがあるときの処理
			//ゲームを続ける
		if (continueGame)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			DrawBox(DRAWBOX_SX, DRAWBOX_GAME_SY, DRAWBOX_EX, DRAWBOX_GAME_EY, 0xffffff, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			//マウスの左クリックを検知したらゲーム続行
			if (GetMouseInput() & MOUSE_INPUT_LEFT ||
				ins.IsPadBtnTrgDown(
					InputManager::JOYPAD_NO::PAD1,
					InputManager::JOYPAD_BTN::DOWN))
			{
				//SoundManager::GetInstance().PlayEvent(SOUND_ID::SE_CLICK);
				ServiceLocator::GetSound().PlayEvent(SOUND_ID::SE_CLICK);
				isPause_ = false;
			}
		}
		//タイトルに戻る
		else if (exitGame)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			DrawBox(DRAWBOX_SX, DRAWBOX_GAMEEND_SY, DRAWBOX_EX, DRAWBOX_GAMEEND_EY, 0xffffff, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			//マウスの左クリックを検知したらタイトルに戻る
			if (GetMouseInput() & MOUSE_INPUT_LEFT ||
				ins.IsPadBtnTrgDown(
					InputManager::JOYPAD_NO::PAD1,
					InputManager::JOYPAD_BTN::DOWN))
			{
				//SoundManager::GetInstance().PlayEvent(SOUND_ID::SE_CLICK);
				ServiceLocator::GetSound().StopEvent(SOUND_ID::SE_CLICK);
				// Effekseerを終了する
				sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
			}
		}
	}


}

void GameScene::Score(void)
{
	//クリアタイムをスコアとして保存
	//clearTime_ = 
}

int GameScene::GetScore(void)
{
	return clearTime_;
}

void GameScene::ItemDrop(void)
{
	//一旦プレイヤーの場所にアイテムを出す
	//itemMng_->SpawnItem(ItemManager::ITEM_TYPE::KEY, player_->GetTransform().pos);


}

void GameScene::IsClear(void)
{

	//isClear_ = player_->GetClearFlag();

	//if(isClear_ && bossMng_->IsBossDead())
	//switch (stageState_)
	//{
	//case StageState::STAGE_1:
	//	isClear_ = player_->GetClearFlag();
	//	if (isClear_)
	//	{
	//		isClear_ = false;
	//		GameData::GetInstance().clearTime = clearTime_;

	//		// ★ ステージ切り替えのためのフェードアウトを開始
	//		StartFade(FadeState::FADE_OUT, 5);
	//	}
	//	break;

	//case StageState::STAGE_2:
	//	break;
	//}

}

void GameScene::StartFade(FadeState state, int speed)
{
	fadeState_ = state;
	fadeSpeed_ = speed;
	if (state == FadeState::FADE_OUT) fadeAlpha_ = 0;
	if (state == FadeState::FADE_IN)  fadeAlpha_ = 255;
}

bool GameScene::UpdateFade(void)
{
	if (fadeState_ == FadeState::NONE) return false;

	if (fadeState_ == FadeState::FADE_OUT)
	{
		fadeAlpha_ += fadeSpeed_;
		if (fadeAlpha_ >= 255)
		{
			fadeAlpha_ = 255;
			fadeState_ = FadeState::NONE;
			return true; // フェードアウト完了
		}
	}
	else if (fadeState_ == FadeState::FADE_IN)
	{
		fadeAlpha_ -= fadeSpeed_;
		if (fadeAlpha_ <= 0)
		{
			fadeAlpha_ = 0;
			fadeState_ = FadeState::NONE;
			return true; // フェードイン完了
		}
	}
	return false;
}

void GameScene::DrawFade(void) const
{
	if (fadeAlpha_ > 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeAlpha_);
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
