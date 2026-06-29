#include "GameClearScene.h"
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include"../Manager/SceneManager.h"
#include "../Object/Common/Transform.h"
#include "../Manager/InputManager.h"
#include"../Manager/SoundManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include"../Ranking/Ranking.h"
#include "../Ranking/GameData.h"
#include "../Manager/Camera.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/Actor/SkyDome.h"
#include "../Application.h"
#include "TitleScene.h"
#include<EffekseerForDXLib.h>
#include "../Manager/ServiceLocator.h"

GameClearScene::GameClearScene(void)
	:
	isEnd_(false),
	mosPosX_(0),
	mosPosY_(0),
	gameClearImg_(-1)
{
}

GameClearScene::~GameClearScene(void)
{
	Release();
}

void GameClearScene::Init(void)
{

	// 画像読み込み
	gameClearImg_ = resMng_.Load(ResourceManager::SRC::GameClearImg).handleId_;

	sceMng_.GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	Ranking::GetIns().Load();

	Ranking::GetIns().AddScore(GameData::GetInstance().clearTime);

	Ranking::GetIns().Save();

	// Saveでclearされるので再読み込み
	Ranking::GetIns().Load();

	// BGM再生
	ServiceLocator::GetSound().PlayEvent(SOUND_ID::BGM_GAMECLEAR, true);

}

void GameClearScene::Update(void)
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
			sceMng_.ChangeScene(SceneManager::SCENE_ID::STAGE_1);
		}

		//ポーズ画面へ
		if (ins.IsTrgDown(KEY_INPUT_ESCAPE) ||
			ins.IsPadBtnTrgDown(
				InputManager::JOYPAD_NO::PAD1,
				InputManager::JOYPAD_BTN::START))
		{
			isEnd_ = true;
		}


		if (ins.IsTrgDown(KEY_INPUT_0) ||
			ins.IsPadBtnTrgDown(
				InputManager::JOYPAD_NO::PAD1,
				InputManager::JOYPAD_BTN::RIGHT))
		{
			//SoundManager::GetInstance().PlayEvent(SOUND_ID::SE_CLICK);
			ServiceLocator::GetSound().PlayEvent(SOUND_ID::SE_CLICK);

			sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}

}
void GameClearScene::Draw(void)
{

	SetFontSize(64);
	
	// クリアタイム取得
	int frame = GameData::GetInstance().clearTime;

	//クリアタイムを分、秒、フレームに変換
	int minute = frame / 3600;
	int second = (frame / 60) % 60;
	int framePart = frame % 60;

	//タイトル画像
	// DrawGraph(IMG_CLEAR_POS_X, IMG_CLEAR_POS_Y, gameClearImg_, true);


	// クリアタイム表示
	DrawFormatString(
		900,
		150,
		0xffffff,
		"TIME %02d:%02d:%02d",
		minute,
		second,
		framePart
	);

	// ランキングタイトル
	DrawFormatString(200, 150, 0xffff00, "RANKING");

	// ランキング取得
	const auto& ranking = Ranking::GetIns().GetRankingList();

	for (int i = 0; i < (int)ranking.size() && i < 10; i++)
	{
		if (ranking[i].score == -1)
		{
			continue;
		}

		int rankMinute = ranking[i].score / 3600;
		int rankSecond = (ranking[i].score / 60) % 60;
		int rankFrame = ranking[i].score % 60;

		DrawFormatString(
			300,
			400 + i * 60,
			0xffffff,
			"%2d位        %02d:%02d:%02d",
			i + 1,
			rankMinute,
			rankSecond,
			rankFrame
		);
	}

	// ポーズ画面
	IsPause();
}

void GameClearScene::Release(void)
{
	// BGM停止
	ServiceLocator::GetSound().StopEvent(SOUND_ID::BGM_GAMECLEAR);
	ServiceLocator::GetSound().StopEvent(SOUND_ID::SE_CLICK);

}

void GameClearScene::IsPause(void)
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


