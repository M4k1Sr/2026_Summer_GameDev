#include <DxLib.h>
#include <vector>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/Actor/Stage.h"
#include "../Object/Actor/SkyDome.h"
#include "../Object/Actor/Charactor/Player.h"
#include "../Object/Actor/Charactor/Object/ObjectManager.h"
#include "GameScene.h"
#include "../Application.h"

GameScene::GameScene(void)
	:
	stage_(nullptr),
	skyDome_(nullptr),
	player_(nullptr),
	objMng_(nullptr),
	isPause_(false),
	pauseImg_(-1),
	isSousa_(false),
	sousaImg_(-1),
	mousePos_X(0),
	mousePos_Y(0),
	SceneBase()
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{

   
	
	// ステージ初期化
	stage_ = new Stage();
	stage_->Init();

	// プレイヤー初期化
	player_ = new Player();
	player_->Init();

	// スカイドーム初期化
	skyDome_ = new SkyDome(player_->GetTransform());
	skyDome_->Init();

	// オブジェクト初期化
	objMng_ = new ObjectManager();
	objMng_->Init();

	// ステージモデルのコライダーをプレイヤーに登録
	const ColliderBase* stageCollider =
		stage_->GetOwnCollider(static_cast<int>(Stage::COLLIDER_TYPE::MODEL));
	player_->AddHitCollider(stageCollider);

	// オブジェクト(全て)のコライダーを登録
	const std::vector<ObjectBase*>& objects = objMng_->GetObjects();
	for (const auto& obj : objects)
	{
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

	// カメラモード変更
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(&player_->GetTransform());
	camera->AddHitCollider(stageCollider);
	camera->ChangeMode(Camera::MODE::FOLLOW);

}

void GameScene::Update(void)
{

	auto& ins = InputManager::GetInstance();

	//ESC押下時ポーズ画面に遷移
	if (ins.IsTrgDown(KEY_INPUT_ESCAPE) )
	{
		isPause_ = !isPause_;  // ポーズのON/OFF切り替え
	}

	//ポーズ画面中はゲームを静止させる
	if (!isPause_)
	{
		//マウスポインタを非表示にする
		SetMouseDispFlag(false);

		// ステージ更新
		stage_->Update();

		// プレイヤー更新
		player_->Update();

		// スカイドーム更新
		skyDome_->Update();

		// オブジェクト更新
		objMng_->Update();
	}
}

void GameScene::Draw(void)
{
	// スカイドーム描画
	skyDome_->Draw();

	// ステージ描画
	stage_->Draw();

	// プレイヤー描画
	player_->Draw();

	// オブジェクト描画
	objMng_->Draw();

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

	// プレイヤー解放
	player_->Release();
	delete skyDome_;

	// オブジェクト解放
	objMng_->Release();
	delete objMng_;

	//ポーズ画像解放
	DeleteGraph(pauseImg_);

	//操作画像解放
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

		DrawBox(400,200,1600,400, 0xffffff, false);
		DrawFormatString(670, 270, 0xffffff, "ゲームを続けますか?");

		DrawBox(400, 600, 1600, 800, 0xffffff, false);
		DrawFormatString(670, 670, 0xffffff, "ゲームを終了しますか?");

		//マウスポインタを表示状態にする
		SetMouseDispFlag(TRUE);

		
	}
}
