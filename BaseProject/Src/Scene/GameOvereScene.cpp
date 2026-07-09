#include "GameOvereScene.h"
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include"../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Object/Common/Transform.h"
#include"../Manager/SoundManager.h"
#include "../Manager/InputManager.h"
<<<<<<< HEAD
#include "../Manager/ResourceManager.h"
#include "../Renderer/PostEffectRenderer/Src/Manager/PostEffectManager.h"
=======
>>>>>>> nakanishi
#include "../Manager/Resource.h"
#include "../Manager/Camera.h"
#include "../Object/Common/AnimationController.h"
#include "../Renderer/PostEffectRenderer/Src/Manager/PostEffectManager.h"
#include "../Object/Actor/SkyDome.h"
#include "../Application.h"
#include "TitleScene.h"
#include<EffekseerForDXLib.h>
#include "../Manager/ServiceLocator.h"

GameOvereScene::GameOvereScene()
	:
	player_(),
	isEnd_(false),
	mosPosX_(0),
	mosPosY_(0),
<<<<<<< HEAD
	effect_(nullptr),
	gameOverImg_(-1)
=======
	playerPos_(AsoUtility::VECTOR_ZERO),
	playerId_(-1),
	animationController_(nullptr)
>>>>>>> nakanishi
{
}

GameOvereScene::~GameOvereScene()
{
	Release();
}

void GameOvereScene::Init(void)
{
<<<<<<< HEAD

	// ニンゲン
	player_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));
	player_.scl = VGet(PLAYER_SCALE, PLAYER_SCALE, PLAYER_SCALE);
	player_.quaRot = Quaternion::Euler(PLAYER_ANGLE);
	player_.quaRotLocal = Quaternion::Euler(PLAYER_LOCAL_ANGLE);
	player_.pos = PLAYER_POS;
	player_.Update();

	// アニメーションコントローラー
	animationController_ =
		new AnimationController(player_.modelId);
	animationController_->Add(0, 20.0f, Application::PATH_MODEL + "Player/Sitting.mv1");
	animationController_->Play(0, true);

	//ゲームオーバー画像
	gameOverImg_ = resMng_.Load(ResourceManager::SRC::GameOverImg).handleId_;
=======
	//プレイヤー関連
	playerId_ = MV1LoadModel("Data/Model/Player/Player.mv1");
	playerScl_ = PLAYER_SIZE;
	playerPos_ = PLAYER_POS;
	playerRot_ = PLAYER_ROT;
	MV1SetScale(playerId_, playerScl_);
	MV1SetPosition(playerId_, playerPos_);
	MV1SetRotationXYZ(playerId_, playerRot_);

	//背景画像
	backImg_ = LoadGraph("Data/Image/GameOverSceneBack2.png");

	// アニメーションコントローラー
	animationController_ =
		new AnimationController(playerId_);

	// アニメーション追加
	animationController_->Add(static_cast<int>(ANIM_TYPE::DESPAIR), 20.0f, Application::PATH_MODEL + "Player/Despair.mv1");
	
	// アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::DESPAIR), true);

>>>>>>> nakanishi
	// 定点カメラ
	sceMng_.GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	// BGM再生
	ServiceLocator::GetSound().PlayEvent(SOUND_ID::BGM_GAMEOVER, true);

	// ポストエフェクト
	effect_ = new PostEffectManager();
	effect_->Init();
	effect_->Load();
}

void GameOvereScene::Update(void)
{


	if (!isEnd_)
	{
		auto& ins = InputManager::GetInstance();

		//ゲームシーンへ遷移
		if (ins.IsTrgDown(KEY_INPUT_SPACE)||
			ins.IsPadBtnTrgDown(
				InputManager::JOYPAD_NO::PAD1,
				InputManager::JOYPAD_BTN::DOWN))
		{
			//SoundManager::GetInstance().PlayEvent(SOUND_ID::SE_CLICK);
			ServiceLocator::GetSound().PlayEvent(SOUND_ID::SE_CLICK);
			sceMng_.ChangeScene(SceneManager::SCENE_ID::STAGE_1);
		}

		if (ins.IsTrgDown(KEY_INPUT_0) ||
			ins.IsPadBtnTrgDown(
				InputManager::JOYPAD_NO::PAD1,
				InputManager::JOYPAD_BTN::RIGHT) )
		{
			//SoundManager::GetInstance().PlayEvent(SOUND_ID::SE_CLICK);
			ServiceLocator::GetSound().PlayEvent(SOUND_ID::SE_CLICK);

			sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		}

		//ポーズ画面へ
		if (ins.IsTrgDown(KEY_INPUT_ESCAPE) ||
			ins.IsPadBtnTrgDown(
				InputManager::JOYPAD_NO::PAD1,
				InputManager::JOYPAD_BTN::START))
		{
			isEnd_ = true;
		}

		effect_->Update();
	}

	effect_->Update();
}

void GameOvereScene::Draw(void)
{
<<<<<<< HEAD
	
	//プレイヤー
	MV1DrawModel(player_.modelId);
=======
	//背景画像
	DrawRotaGraph(Application::SCREEN_SIZE_X/2, 
		Application::SCREEN_SIZE_Y/2,
		2, 0, backImg_, true);

	//プレイヤー
	MV1DrawModel(playerId_);

	// ポストエフェクト描画
	effect_->Draw(SceneManager::GetInstance().GetMainScreen());
>>>>>>> nakanishi

	// ポストエフェクト描画
	effect_->Draw(SceneManager::GetInstance().GetMainScreen());

	////ゲームシーンへ遷移
	//DrawFormatString(670, 270, 0xffffff, "リトライ : SPACE");

	////タイトルへ戻る
	//DrawFormatString(670, 670, 0xffffff, "タイトル : 0");

	//タイトル画像
	DrawGraph(IMG_OVER_POS_X, IMG_OVER_POS_Y, gameOverImg_, true);


	//ポーズ画d
	IsPause();
}

void GameOvereScene::Release(void)
{
	MV1DeleteModel(playerId_);
	DeleteGraph(backImg_);

	// アニメーション解放
	if (animationController_ != nullptr)
	{
		animationController_->Release();
		delete animationController_;
	}

	ServiceLocator::GetSound().StopEvent(SOUND_ID::BGM_GAMEOVER);
	ServiceLocator::GetSound().StopEvent(SOUND_ID::SE_CLICK);

}

void GameOvereScene::IsPause(void)
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
				ServiceLocator::GetSound().PlayEvent(SOUND_ID::SE_CLICK);
				isEnd_ = false;
			}
		}
		//ゲームを終了する
		else if (exitGame)
		{

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			DrawBox(DRAWBOX_SX, DRAWBOX_GAMEEND_SY, DRAWBOX_EX, DRAWBOX_GAMEEND_EY, 0xffffff, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			//マウスの左クリックを検知したらゲーム終了
			if (GetMouseInput() & MOUSE_INPUT_LEFT ||
				ins.IsPadBtnTrgDown(
					InputManager::JOYPAD_NO::PAD1,
					InputManager::JOYPAD_BTN::DOWN))
			{
				//SoundManager::GetInstance().PlayEvent(SOUND_ID::SE_CLICK);
				ServiceLocator::GetSound().PlayEvent(SOUND_ID::SE_CLICK);
				// Effekseerを終了する
				Effkseer_End();
				DxLib_End();
			}
		}

	}
}
