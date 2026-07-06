#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class AnimationController;
class SkyDome;
class PostEffectManager;

class GameOvereScene : public SceneBase
{
public:

	enum class ANIM_TYPE
	{
		DESPAIR,
		MAX,
	};

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

	//ポーズ画面の選択ボックス座標
	static constexpr int DRAWBOX_SX = 400;

	static constexpr int DRAWBOX_EX = 1600;

	static constexpr int DRAWBOX_GAMEEND_SY = 600;

	static constexpr int DRAWBOX_GAMEEND_EY = 800;

	static constexpr int DRAWBOX_GAME_SY = 200;

	static constexpr int DRAWBOX_GAME_EY = 400;

	//プレイヤーの大きさ、座標、角度
	static constexpr VECTOR PLAYER_SIZE = { 1.5f, 1.5f, 1.5f };
	static constexpr VECTOR PLAYER_POS = { 0.0f, -150.0f, 0.0f };
	static constexpr VECTOR PLAYER_ROT = { 0.0f, DX_PI_F, 0.0f };

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

