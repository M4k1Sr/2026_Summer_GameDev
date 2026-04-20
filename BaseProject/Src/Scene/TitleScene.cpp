#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Object/Common/Transform.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "../Manager/Camera.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/Actor/SkyDome.h"
#include "../Application.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
	:
	imgTitle_(-1),
	imgPush_(-1),
	bigPlanet_(),
	rollPlanet_(),
	player_(),
	animationController_(nullptr),
	skyDome_(nullptr),
	isEnd_(false),
	SceneBase()
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	// 画像読み込み
	imgTitle_ = resMng_.Load(ResourceManager::SRC::TITLE).handleId_;

	imgPush_ = resMng_.Load(ResourceManager::SRC::PUSH_SPACE).handleId_;

	//// 定点カメラ
	sceMng_.GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);


	// メイン惑星
	bigPlanet_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::PIT_FALL_PLANET));
	bigPlanet_.scl = {0.7,0.4,0.4};
	bigPlanet_.quaRot = Quaternion::Identity();
	bigPlanet_.quaRotLocal = Quaternion(1.0f, 1.0f, 0.0f, 0.0f);;
	bigPlanet_.pos = { 0.0f, -500.0f,400.0f };
	bigPlanet_.Update();

	

	// アニメーションコントローラー
	animationController_ = 
		new AnimationController(player_.modelId);
	animationController_->Add(0, 20.0f,Application::PATH_MODEL + "Player/Run.mv1");
	animationController_->Play(0, true);

	// スカイドーム
	skyDome_ = new SkyDome(empty_);
	skyDome_->Init();

}

void TitleScene::Update(void)
{

	auto& ins = InputManager::GetInstance();

	//ゲームシーンへ遷移
		if (ins.IsTrgDown(KEY_INPUT_SPACE))
		{
			sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
		}

		//ポーズ画面へ
		if (ins.IsTrgDown(KEY_INPUT_ESCAPE))
		{
			isEnd_ = true;
		}


		rollPlanet_.quaRot = rollPlanet_.quaRot.Mult(
			Quaternion::Euler(0.0f, 0.0f, AsoUtility::Deg2RadF(-1.0f)));

		// 回転惑星の更新
		rollPlanet_.Update();

		animationController_->Update();

		skyDome_->Update();

}

void TitleScene::Draw(void)
{

	// スカイドーム
	skyDome_->Draw();

	// モデル描画
	// 惑星モデル
	MV1DrawModel(bigPlanet_.modelId);
	MV1DrawModel(rollPlanet_.modelId);

	// ニンゲンモデル
	MV1DrawModel(player_.modelId);

	// タイトル描画
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, IMG_TITLE_POS_Y, 1.0f, 0.0f, imgTitle_, true);
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, IMG_PUSH_POS_Y, 1.0f, 0.0f, imgPush_, true);

	//ポーズ画面
	IsPause();
	

}	

void TitleScene::Release(void)
{
	// アニメーションコントローラー解放
	animationController_->Release();
	delete animationController_;

	// スカイドーム解放
	skyDome_->Release();
	delete skyDome_;
}


void TitleScene::IsPause(void)
{
	if (isEnd_)
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




	}
}