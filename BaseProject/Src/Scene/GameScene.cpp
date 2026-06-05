#pragma once
#include <DxLib.h>
#include <vector>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "../Object/Common/AnimationController.h"
#include"../Manager/SoundManager.h"
#include "../Object/Actor/Stage.h"
#include "../Object/Actor/SkyDome.h"
#include"../Ranking/GameData.h"
#include "../Object/Actor/IronBall.h"
#include "../Object/Actor/Charactor/Player.h"
#include "../Object/Actor/Charactor/Boss/BossManager.h"
#include "../Object/Actor/Attack/AttackManager.h"
#include "../Object/Actor/Charactor/Object/ObjectManager.h"
#include"../Ranking/Ranking.h"
#include"../Object/UI/UI.h"
#include "GameScene.h"
#include "../Application.h"
#include"../Manager/ItemManager.h"
#include<EffekseerForDXLib.h>

GameScene::GameScene(void)
	:
	stage_(nullptr),
	skyDome_(nullptr),
	player_(nullptr),
	ironBall_(nullptr),
	ui_(nullptr),
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
	clearTime_(0),
	SceneBase()
{
	//サウンド
	SoundManager::GetInstance().LoadBank(BANK_ID::COMMON);
}

GameScene::~GameScene(void)
{
	// タイトルBGM停止
	SoundManager::GetInstance().StopEvent(SOUND_ID::SE_CLICK);
}

void GameScene::Init(void)
{
	// ステージ初期化
	stage_ = new Stage();
	stage_->Init();

	// オブジェクト初期化
	objMng_ = new ObjectManager();
	objMng_->Init();


	// プレイヤー初期化
	player_ = new Player();
	player_->Init();
	player_->SetObjectManager(objMng_);

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
		stage_->GetOwnCollider(static_cast<int>(Stage::COLLIDER_TYPE::MODEL));
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

	// UIモデル
	ui_ = new UI();
	ui_->Init();

	// カメラモード変更
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(&player_->GetTransform());
	camera->AddHitCollider(stageCollider);
	camera->ChangeMode(Camera::MODE::SCROLL_FOLLOW);

}

void GameScene::Update(void)
{

	auto const& ins = InputManager::GetInstance();

	// ESC押下時ポーズ画面に遷移
	if (ins.IsTrgDown(KEY_INPUT_ESCAPE))
	{
		isPause_ = !isPause_;	// ポーズのON/OFFの切り替え
	}

	// ポーズ画面中はゲームを静止させる
	if (!isPause_)
	{
		//クリアタイム加算
		clearTime_++;
		// マウスポインタを非表示にする
		SetMouseDispFlag(false);
		stage_->Update();
		skyDome_->Update();
		player_->Update();
		bossMng_->Update();
		objMng_->Update();
		attackMng_->Update();
		ironBall_->Update();
		ui_->Update();

		// ===========================================================
		// ★ ボスの生存状態と距離によるカメラモードの自動切り替え
		// ===========================================================
		Camera* camera = SceneManager::GetInstance().GetCamera();
		const std::vector<BossBase*>& bosses = bossMng_->GetBosses();

		BossBase* nearestBoss = nullptr;
		float minDistanceSq = 1500.0f * 1500.0f; // ロックオンを開始する距離の2乗（好みに合わせて調整してください）

		VECTOR playerPos = player_->GetTransform().pos;

		// 出現しているすべてのボスをチェック
		for (auto* boss : bosses)
		{
			if (boss == nullptr) continue;

			// ★ ボスが既に死亡している場合はロックオンしない（スキップする）
			if (boss->GetIsDead()) continue;

			// プレイヤーとボスの距離の2乗を計算
			float distSq = VSquareSize(VSub(boss->GetTransform().pos, playerPos));

			// 一番近いボスを記憶
			if (distSq < minDistanceSq)
			{
				minDistanceSq = distSq;
				nearestBoss = boss;
			}
		}

		// 条件を満たす「生きているボス」が近くにいればロックオン状態にする
		if (nearestBoss != nullptr)
		{
			camera->SetLockOnTarget(&nearestBoss->GetTransform());
			camera->ChangeMode(Camera::MODE::LOCK_ON);
		}
		else
		{
			// 周囲にボスがいない、またはロックオンしていたボスが死んだら元のカメラに戻す
			camera->ChangeMode(Camera::MODE::SCROLL_FOLLOW);
		}
		// ===========================================================
		
		// ゲームクリア判定
		IsClear();
	}


	//ゲームオーバー判定
	isEnd_ = ui_->GetIsGameOver();
	isEnd_ = player_->GetDeadFlag();

	//ゲームオーバーシーンへ遷移
	if (isEnd_ )
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}

}

void GameScene::Draw(void)
{
	// スカイドーム描画
	skyDome_->Draw();

	// ステージ描画
	stage_->Draw();

	// 鉄球描画
	ironBall_->Draw();

	// オブジェクト描画
	objMng_->Draw();


	//デバッグ用ゴール
	DrawBillboard3D(VGet(5060.0f, 0.0f, -490.0f),
		0.5f,                           // 中心X
		0.5f,                           // 中心Y
		400.0f,                         // サイズ
		0.0f,                           // 回転
		goalImg_,                       // 画像
		TRUE);
    

	// プレイヤー描画
	player_->Draw();
	
	//ボス描画
	bossMng_->Draw();

	// 攻撃描画
	attackMng_->Draw();

	// UI描画
	ui_->Draw();

	////ポーズ画面
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

	ui_->Release();
	delete ui_;

	ironBall_->Release();
	delete ironBall_;

	// ItemManager解放
	itemMng_->Release();
	delete itemMng_;

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
		DrawFormatString(670, 670, 0xffffff, "ゲームを終了しますか?");

		//マウスポインタを表示状態にする
		SetMouseDispFlag(TRUE);

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
			if (GetMouseInput() & MOUSE_INPUT_LEFT)
			{
				SoundManager::GetInstance().PlayEvent(SOUND_ID::SE_CLICK);
				isPause_ = false;
			}
		}
		//ゲームを終了する
		else if (exitGame)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			DrawBox(DRAWBOX_SX, DRAWBOX_GAMEEND_SY, DRAWBOX_EX, DRAWBOX_GAMEEND_EY, 0xffffff, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			//マウスの左クリックを検知したらゲーム終了
			if (GetMouseInput() & MOUSE_INPUT_LEFT)
			{
				SoundManager::GetInstance().PlayEvent(SOUND_ID::SE_CLICK);
				// Effekseerを終了する
				Effkseer_End();
				DxLib_End();
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

	isClear_ = player_->GetClearFlag();

	if(isClear_)
	{
		GameData::GetInstance().clearTime = clearTime_;
		sceMng_.ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);
	}
}
