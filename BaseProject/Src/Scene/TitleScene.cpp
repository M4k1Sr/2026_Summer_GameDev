#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include "TitleScene.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Common/Transform.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "../Manager/Camera.h"
#include "../Manager/SoundManager.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/Actor/SkyDome.h"
#include "../Renderer/PostEffectRenderer/Src/Manager/PostEffectManager.h"
#include "../Application.h"
#include "../Manager/ServiceLocator.h"


TitleScene::TitleScene(void)
	:
	imgTitle_(-1),
	imgPush_(-1),
	bigPlanet_(),
	rollPlanet_(),
	player_(),
	effect_(nullptr),
	animationController_(nullptr),
	skyDome_(nullptr),
	isEnd_(false),
	mosPosX_(0),
	mosPosY_(0),
	wallImg_(-1),
	isBgmPlay_(false),
	pauseSelect_(PauseSelect::CONTINUE),
	SceneBase()
{
}

TitleScene::~TitleScene(void)
{
	Release();
}

void TitleScene::Init(void)
{
	// 画像読み込み
	imgTitle_ = resMng_.Load(ResourceManager::SRC::TITLE).handleId_;

	imgPush_ = resMng_.Load(ResourceManager::SRC::PUSH_SPACE).handleId_;

	imgBackGround_ = resMng_.Load(ResourceManager::SRC::BACKGROUND).handleId_;

	// 定点カメラ
	sceMng_.GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	//タイトル牢屋
	prison_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::PRISON));

	prison_.quaRot = Quaternion::Euler(PRISON_ANGLE);
	prison_.scl = VGet(PRISON_SCALE, PRISON_SCALE, PRISON_SCALE);
	prison_.pos = PRISON_POS;
	prison_.Update();

	//タイトル壁
	wallImg_ = resMng_.Load(ResourceManager::SRC::WALL).handleId_;

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
	animationController_->Add(0, 20.0f,Application::PATH_MODEL + "Player/Sitting.mv1");
	animationController_->Play(0, true);

	// スカイドーム
	skyDome_ = new SkyDome(empty_);
	skyDome_->Init();

	// BGM再生
	ServiceLocator::GetSound().PlayEvent(SOUND_ID::BGM_TITLE);

	// ポストエフェクト
	effect_ = new PostEffectManager();
	effect_->Init();
	effect_->Load();
}

void TitleScene::Update(void)
{
	if (!isEnd_)
	{
		//マウスポインタを非表示状態にする
		SetMouseDispFlag(FALSE);

		auto& ins = InputManager::GetInstance();

		//ゲームシーンへ遷移
		if (ins.IsTrgDown(KEY_INPUT_SPACE)||
			ins.IsPadBtnTrgDown(
			InputManager::JOYPAD_NO::PAD1,
			InputManager::JOYPAD_BTN::DOWN))
		{

			ServiceLocator::GetSound().PlayEvent(SOUND_ID::SE_CLICK);

			sceMng_.ChangeScene(SceneManager::SCENE_ID::STAGE_1);

		}

		//ポーズ画面へ
		if (ins.IsTrgDown(KEY_INPUT_ESCAPE)||
			ins.IsPadBtnTrgDown(
			InputManager::JOYPAD_NO::PAD1,
			InputManager::JOYPAD_BTN::START))
		{
			isEnd_ = true;
		}

		rollPlanet_.quaRot = rollPlanet_.quaRot.Mult(
			Quaternion::Euler(0.0f, 0.0f, AsoUtility::Deg2RadF(-1.0f)));

		rollPlanet_.Update();

		animationController_->Update();

		skyDome_->Update();

		effect_->Update();

	}
}

void TitleScene::Draw(void)
{
	//背景画像
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		1.0f, 0.0f, imgBackGround_, true);

	// 真上からまっすぐ照らす
	SetLightDirection(VGet(0.0f, -1.0f, 0.0f));

	// 牢屋モデルを描画する
	MV1DrawModel(prison_.modelId);

	//プレイヤー
	MV1DrawModel(player_.modelId);

	// ポストエフェクト描画
	effect_->Draw(SceneManager::GetInstance().GetMainScreen());

	//タイトル画像
	DrawGraph(IMG_TITLE_POS_X, IMG_TITLE_POS_Y, imgTitle_, true);


	//DrawRotaGraph(0, 0, 1.0f, 0.0f, wallImg_, true);

	// 1. 大きさ「40」のフォントハンドルを作成（太さは標準、フォントタイプはDX_FONTTYPE_NORMAL）
	int debugFontHandle = CreateFontToHandle(NULL, 40, 1, DX_FONTTYPE_NORMAL);

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

	// BGM停止
	ServiceLocator::GetSound().StopEvent(SOUND_ID::BGM_TITLE);
	ServiceLocator::GetSound().StopEvent(SOUND_ID::SE_CLICK);
	ServiceLocator::GetSound().StopEvent(SOUND_ID::SE_CURSOR);

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
			if(!isBgmPlay_)
			{
				//カーソルがあったときに音を鳴らす
				//SoundManager::GetInstance().PlayEvent(SOUND_ID::SE_CURSOR);
				ServiceLocator::GetSound().PlayEvent(SOUND_ID::SE_CURSOR);
				isBgmPlay_ = true;
			}
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
			if (!isBgmPlay_)
			{
				//カーソルがあったときに音を鳴らす
				//SoundManager::GetInstance().PlayEvent(SOUND_ID::SE_CURSOR);
				ServiceLocator::GetSound().PlayEvent(SOUND_ID::SE_CURSOR);
				isBgmPlay_ = true;
			}
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
		else
		{
			//カーソルがないときは音を鳴らさない
			isBgmPlay_ = false;
		}
		
	}

}