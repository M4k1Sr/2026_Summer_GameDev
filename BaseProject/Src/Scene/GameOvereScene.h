#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class AnimationController;
class SkyDome;
class PostEffectManager;
<<<<<<< HEAD

=======
>>>>>>> nakanishi

class GameOvereScene : public SceneBase
{
public:

<<<<<<< HEAD



	// ゲームオーバー画像のX座標
	static constexpr int IMG_OVER_POS_X = 150;
	// ゲームオーバー画像のY座標
	static constexpr int IMG_OVER_POS_Y = 100;
=======
	enum class ANIM_TYPE
	{
		DESPAIR,
		MAX,
	};
>>>>>>> nakanishi

	GameOvereScene();

	~GameOvereScene();


	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;

	void IsPause(void);

private:

	//ポストエフェクト
	PostEffectManager* effect_;


	// アニメーション処理
	AnimationController* animationController_;

	// ニンゲン
	Transform player_;


	//ゲームオーバー画像
	int gameOverImg_;

	//ポーズ画面の選択ボックス座標
	static constexpr int DRAWBOX_SX = 400;

	static constexpr int DRAWBOX_EX = 1600;

	static constexpr int DRAWBOX_GAMEEND_SY = 600;

	static constexpr int DRAWBOX_GAMEEND_EY = 800;

	static constexpr int DRAWBOX_GAME_SY = 200;

	static constexpr int DRAWBOX_GAME_EY = 400;

<<<<<<< HEAD
	// ニンゲンのスケール
	static constexpr float PLAYER_SCALE = 4.5f;
	// ニンゲン座標
	static constexpr VECTOR PLAYER_POS = { -100.0f, -500.0f,400.0f };
	// ニンゲンの角度
	static constexpr VECTOR PLAYER_ANGLE = { 0.0f,  150.0f * DX_PI_F / 180.0f, 0.0f };
	static constexpr VECTOR PLAYER_LOCAL_ANGLE = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };


=======
	//プレイヤーの大きさ、座標、角度
	static constexpr VECTOR PLAYER_SIZE = { 1.5f, 1.5f, 1.5f };
	static constexpr VECTOR PLAYER_POS = { 0.0f, -150.0f, 0.0f };
	static constexpr VECTOR PLAYER_ROT = { 0.0f, DX_PI_F, 0.0f };
>>>>>>> nakanishi

	// アニメーション処理
	AnimationController* animationController_;

	// ポストエフェクト
	PostEffectManager* effect_;

	//プレイヤー関係
	VECTOR playerScl_;
	VECTOR playerPos_;
	VECTOR playerRot_;
	int playerId_;

	//背景画像のハンドルID
	int backImg_;

	//ポーズ画面
	bool isEnd_;

	//マウスカーソル取得用座標
	int mosPosX_, mosPosY_;

};

