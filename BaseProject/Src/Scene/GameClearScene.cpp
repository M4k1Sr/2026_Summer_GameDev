#include "GameClearScene.h"
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include"../Manager/SceneManager.h"
#include "../Object/Common/Transform.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
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

GameClearScene::GameClearScene(void)
	:
	isEnd_(false),
	mosPosX_(0),
	mosPosY_(0)
{
	//サウンド
	SoundManager::GetInstance().LoadBank(BANK_ID::COMMON);
}

GameClearScene::~GameClearScene(void)
{
	// タイトルBGM停止
	SoundManager::GetInstance().StopEvent(SOUND_ID::SE_CLICK);
}

void GameClearScene::Init(void)
{
	sceMng_.GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	Ranking::GetIns().Load();

	Ranking::GetIns().AddScore(GameData::GetInstance().clearTime);

	Ranking::GetIns().Save();

	// Saveでclearされるので再読み込み
	Ranking::GetIns().Load();
}

void GameClearScene::Update(void)
{

	if (!isEnd_)
	{
		auto& ins = InputManager::GetInstance();

		//ゲームシーンへ遷移
		if (ins.IsTrgDown(KEY_INPUT_SPACE))
		{
			sceMng_.ChangeScene(SceneManager::SCENE_ID::STAGE_1);
		}

		//ポーズ画面へ
		if (ins.IsTrgDown(KEY_INPUT_ESCAPE))
		{
			isEnd_ = true;
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

	// タイトル
	DrawFormatString(700, 100, 0xffffff, "GAME CLEAR!");

	// クリアタイム表示
	DrawFormatString(
		650,
		160,
		0xffffff,
		"TIME %02d:%02d:%02d",
		minute,
		second,
		framePart
	);

	// ランキングタイトル
	DrawFormatString(100, 50, 0xffff00, "RANKING");

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
			100,
			100 + i * 60,
			0xffffff,
			"%2d位  %02d:%02d:%02d",
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

		DrawBox(DRAWBOX_SX, DRAWBOX_GAME_SY, DRAWBOX_EX, DRAWBOX_GAME_EY, 0xffffff, false);
		DrawFormatString(670, 270, 0xffffff, "ゲームを続けますか?");

		DrawBox(DRAWBOX_SX, DRAWBOX_GAMEEND_SY, DRAWBOX_EX, DRAWBOX_GAMEEND_EY, 0xffffff, false);
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
			if (GetMouseInput() & MOUSE_INPUT_LEFT)
			{
				SoundManager::GetInstance().PlayEvent(SOUND_ID::SE_CLICK);
				// Effekseerを終了する。
				Effkseer_End();
				DxLib_End();

			}
		}

	}
}


