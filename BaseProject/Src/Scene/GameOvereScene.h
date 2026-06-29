#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class AnimationController;
class SkyDome;
class PostEffectManager;


class GameOvereScene : public SceneBase
{
public:




	// ゲームオーバー画像のX座標
	static constexpr int IMG_OVER_POS_X = 150;
	// ゲームオーバー画像のY座標
	static constexpr int IMG_OVER_POS_Y = 100;

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


	//ゲームオーバー画像
	int gameOverImg_;

	//ポーズ画面の選択ボックス座標
	static constexpr int DRAWBOX_SX = 400;

	static constexpr int DRAWBOX_EX = 1600;

	static constexpr int DRAWBOX_GAMEEND_SY = 600;

	static constexpr int DRAWBOX_GAMEEND_EY = 800;

	static constexpr int DRAWBOX_GAME_SY = 200;

	static constexpr int DRAWBOX_GAME_EY = 400;



	//ポーズ画面
	bool isEnd_;

	//マウスカーソル取得用座標
	int mosPosX_, mosPosY_;

};

