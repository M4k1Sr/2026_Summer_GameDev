#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class AnimationController;
class SkyDome;

class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void) override;

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;

	// ポーズ判定
	void IsPause(void);

private:

	// アニメーション処理
	AnimationController* animationController_;

	// 惑星
	Transform bigPlanet_;

	// 回転惑星
	Transform rollPlanet_;

	//タイトル檻
	Transform cage_;

	// ニンゲン
	Transform player_;

	//壁画像
	Transform wall_;

	// スカイドーム
	SkyDome* skyDome_;

	// スカイドーム用の空Transform
	Transform empty_;

	// タイトル画像のX座標
	static constexpr int IMG_TITLE_POS_X = 150;
	// タイトル画像のY座標
	static constexpr int IMG_TITLE_POS_Y = -100;
	// PushSpace画像のY座標
	static constexpr int IMG_PUSH_POS_Y = 500;

	// 回転惑星のスケール
	static constexpr float ROLL_PLANET_SCALE = 0.7f;
	// 回転惑星の座標
	static constexpr VECTOR ROLL_PLANET_POS = { -250.0f, -100.0f, -100.0f };
	// 回転惑星の角度
	static constexpr VECTOR ROLL_PLANET_ANGLE = { 90.0f * DX_PI_F / 180.0f, 0.0f, 0.0f};


	// ニンゲンのスケール
	static constexpr float PLAYER_SCALE = 4.5f;
	// ニンゲン座標
	static constexpr VECTOR PLAYER_POS = { -100.0f, -500.0f,400.0f };
	// ニンゲンの角度
	static constexpr VECTOR PLAYER_ANGLE = {0.0f,  150.0f * DX_PI_F / 180.0f, 0.0f };
	static constexpr VECTOR PLAYER_LOCAL_ANGLE = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };

	bool isPause_;
	int pauseImg_;
	bool isSousa_;
	int sousaImg_;
	int mosPosX_, mosPosY_;

	static constexpr int DRAWBOX_SX = 400;
	static constexpr int DRAWBOX_EX = 1600;
	static constexpr int DRAWBOX_GAMEEND_SY = 600;
	static constexpr int DRAWBOX_GAMEEND_EY = 800;
	static constexpr int DRAWBOX_GAME_SY = 200;
	static constexpr int DRAWBOX_GAME_EY = 400;

	// タイトル画像
	int imgTitle_;

	// プッシュ画像
	int imgPush_;

	bool isEnd_;

	//タイトル壁
	int wallImg_;

	//音の処理
	bool isBgmPlay_;

};
